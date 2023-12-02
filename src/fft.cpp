

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <string_view>
#include <filesystem>
//#include <format>

extern "C" { 
	#include <libavcodec/avcodec.h> 
	#include <libavformat/avformat.h>
}

int usage()
{
	std::cerr << "usage: fft [ss|sf] source [ts|tf|td] target" << std::endl;
	return -1;
}

template<typename... Args>
constexpr void logging(const char* fmt, Args&&... args)
{
	fprintf(stderr, fmt, args...);
	fprintf(stderr, "\n");
}

static int decode_packet(AVPacket *pPacket, AVCodecContext *pCodecContext, AVFrame *pFrame);
// save a frame into a .pgm file
static void save_gray_frame(unsigned char *buf, int wrap, int xsize, int ysize, char *filename);

enum ST { none=0, stream, file, display };
ST fr{none}, to{none};
std::string sn, tn;

int main(int argc, char** argv)
{
	using namespace std::literals;

	if ((argc==2) && (argv[1]=="mkf"s))
	{
		constexpr int meg = 1024 * 1024;
		char arr[meg];
		int i;
		for (i=0; i<meg; ++i)
			arr[i] = (char)(i&255);
		i=0;
		while (true)
		{
			FILE *f;
			std::string fn = "file-"s + std::to_string(i++);
			f = fopen(fn.c_str(),"w");
			auto res = fwrite(arr, 1, meg, f);
			fclose(f);
			if (res != meg) break;
		}
		return 0;
	}

	std::vector<std::string> arg;
	for (int i=1; i<argc; ++i)
		arg.push_back(argv[i]);

	AVFormatContext *pFormatContext = avformat_alloc_context();
	if (!pFormatContext) {
		std::cerr << ("ERROR could not allocate memory for Format Context");
		return -1;
	}

	bool hs=false, ht=false;

	int i, n = std::ssize(arg);

	for (i=0; i<n; ++i)
	{
		auto&& a = arg[i];
		if (a=="ss"s) {
			if (++i >= n) return usage();
			if (hs) return usage();
			hs = true;
			fr = stream;
			sn = arg[i];
			continue;
		}
		if (a=="sf"s) {
			if (++i >= n) return usage();
			if (hs) return usage();
			hs = true;
			fr = file;
			sn = arg[i];
			continue;
		}
		if (a=="ts"s) {
			if (++i >= n) return usage();
			if (ht) return usage();
			ht = true;
			to = stream;
			tn = arg[i];
			continue;
		}
		if (a=="tf"s) {
			if (++i >= n) return usage();
			if (ht) return usage();
			ht = true;
			to = file;
			tn = arg[i];
			continue;
		}
		if (a=="td"s) {
			if (ht) return usage();
			ht = true;
			to = display;
			continue;
		}
		return usage();
	}

	if (!hs) return usage();
	if (!ht) return usage();

	if (fr==file)
	{

		if (avformat_open_input(&pFormatContext, sn.c_str(), nullptr, nullptr) != 0) {
			logging("ERROR could not open the file");
			return -1;
		}

		// now we have access to some information about our file
		// since we read its header we can say what format (container) it's
		// and some other information related to the format itself.
		logging("format %s, duration %lld us, bit_rate %lld", pFormatContext->iformat->name, pFormatContext->duration, pFormatContext->bit_rate);

		logging("finding stream info from format");
		// read Packets from the Format to get stream information
		// this function populates pFormatContext->streams
		// (of size equals to pFormatContext->nb_streams)
		// the arguments are:
		// the AVFormatContext
		// and options contains options for codec corresponding to i-th stream.
		// On return each dictionary will be filled with options that were not found.
		// https://ffmpeg.org/doxygen/trunk/group__lavf__decoding.html#gad42172e27cddafb81096939783b157bb
		if (avformat_find_stream_info(pFormatContext, nullptr) < 0) {
			logging("ERROR could not get the stream info");
			return -1;
		}

		// the component that knows how to enCOde and DECode the stream
		// it's the codec (audio or video)
		// http://ffmpeg.org/doxygen/trunk/structAVCodec.html
		AVCodec *pCodec = nullptr;
		// this component describes the properties of a codec used by the stream i
		// https://ffmpeg.org/doxygen/trunk/structAVCodecParameters.html
		AVCodecParameters *pCodecParameters = nullptr;
		int video_stream_index = -1;

		for (int i = 0; i < int(pFormatContext->nb_streams); i++)
		{
			AVCodecParameters *pLocalCodecParameters =  nullptr;
			pLocalCodecParameters = pFormatContext->streams[i]->codecpar;
			logging("AVStream->time_base before open coded %d/%d", pFormatContext->streams[i]->time_base.num, pFormatContext->streams[i]->time_base.den);
			logging("AVStream->r_frame_rate before open coded %d/%d", pFormatContext->streams[i]->r_frame_rate.num, pFormatContext->streams[i]->r_frame_rate.den);
			logging("AVStream->start_time %" PRId64, pFormatContext->streams[i]->start_time);
			logging("AVStream->duration %" PRId64, pFormatContext->streams[i]->duration);

			logging("finding the proper decoder (CODEC)");

			AVCodec *pLocalCodec = nullptr;

			// finds the registered decoder for a codec ID
			// https://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga19a0ca553277f019dd5b0fec6e1f9dca
			pLocalCodec = avcodec_find_decoder(pLocalCodecParameters->codec_id);

			if (pLocalCodec==nullptr) {
				logging("ERROR unsupported codec! (%x)", pLocalCodecParameters->codec_id);
				// In this example if the codec is not found we just skip it
				continue;
			}

			// when the stream is a video we store its index, codec parameters and codec
			if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_VIDEO) {
				if (video_stream_index == -1) {
					video_stream_index = i;
					pCodec = pLocalCodec;
					pCodecParameters = pLocalCodecParameters;
				}

				logging("Video Codec: resolution %d x %d", pLocalCodecParameters->width, pLocalCodecParameters->height);
			} else if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_AUDIO) {
				logging("Audio Codec: %d channels, sample rate %d", pLocalCodecParameters->channels, pLocalCodecParameters->sample_rate);
			}

			// print its name, id and bitrate
			logging("\tCodec %s ID %d bit_rate %lld", pLocalCodec->name, pLocalCodec->id, pLocalCodecParameters->bit_rate);
		}

		if (video_stream_index == -1) {
			logging("File %s does not contain a video stream!", sn.c_str());
			return -1;
		}

		// https://ffmpeg.org/doxygen/trunk/structAVCodecContext.html
		AVCodecContext *pCodecContext = avcodec_alloc_context3(pCodec);
		if (!pCodecContext)
		{
			logging("failed to allocated memory for AVCodecContext");
			return -1;
		}

		// Fill the codec context based on the values from the supplied codec parameters
		// https://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gac7b282f51540ca7a99416a3ba6ee0d16
		if (avcodec_parameters_to_context(pCodecContext, pCodecParameters) < 0)
		{
			logging("failed to copy codec params to codec context");
			return -1;
		}

		// Initialize the AVCodecContext to use the given AVCodec.
		// https://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ga11f785a188d7d9df71621001465b0f1d
		if (avcodec_open2(pCodecContext, pCodec, nullptr) < 0)
		{
			logging("failed to open codec through avcodec_open2");
			return -1;
		}

		// https://ffmpeg.org/doxygen/trunk/structAVFrame.html
		AVFrame *pFrame = av_frame_alloc();
		if (!pFrame)
		{
			logging("failed to allocate memory for AVFrame");
			return -1;
		}
		// https://ffmpeg.org/doxygen/trunk/structAVPacket.html
		AVPacket *pPacket = av_packet_alloc();
		if (!pPacket)
		{
			logging("failed to allocate memory for AVPacket");
			return -1;
		}

		int response = 0;
		int how_many_packets_to_process = 10'000;

		// fill the Packet with data from the Stream
		// https://ffmpeg.org/doxygen/trunk/group__lavf__decoding.html#ga4fdb3084415a82e3810de6ee60e46a61
		while (av_read_frame(pFormatContext, pPacket) >= 0)
		{
			// if it's the video stream
			if (pPacket->stream_index == video_stream_index) {
				logging("AVPacket->pts %" PRId64, pPacket->pts);
				response = decode_packet(pPacket, pCodecContext, pFrame);
				if (response < 0)
					break;
				// stop it, otherwise we'll be saving hundreds of frames
				if (--how_many_packets_to_process <= 0) break;
			}
			// https://ffmpeg.org/doxygen/trunk/group__lavc__packet.html#ga63d5a489b419bd5d45cfd09091cbcbc2
			av_packet_unref(pPacket);
		}

		logging("releasing all the resources");

		avformat_close_input(&pFormatContext);
		av_packet_free(&pPacket);
		av_frame_free(&pFrame);
		avcodec_free_context(&pCodecContext);

	}

}



static int decode_packet(AVPacket *pPacket, AVCodecContext *pCodecContext, AVFrame *pFrame)
{
	// Supply raw packet data as input to a decoder
	// https://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga58bc4bf1e0ac59e27362597e467efff3
	int response = avcodec_send_packet(pCodecContext, pPacket);

	if (response < 0) {
		logging("Error while sending a packet to the decoder: %d", response);
		return response;
	}

	while (response >= 0)
	{
		// Return decoded output data (into a frame) from a decoder
		// https://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga11e6542c4e66d3028668788a1a74217c
		response = avcodec_receive_frame(pCodecContext, pFrame);
		if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
			break;
		} else if (response < 0) {
			logging("Error while receiving a frame from the decoder: %d", response);
			return response;
		}

		if (response >= 0) {
			logging(
				"Frame %d (type=%c, size=%d bytes, format=%d) pts %d key_frame %d [DTS %d]",
				pCodecContext->frame_number,
				av_get_picture_type_char(pFrame->pict_type),
				pFrame->pkt_size,
				pFrame->format,
				pFrame->pts,
				pFrame->key_frame,
				pFrame->coded_picture_number
			);

			char frame_filename[1024];
			snprintf(frame_filename, sizeof(frame_filename), "frame/%s-%04d.pgm", "frame", pCodecContext->frame_number);
			// Check if the frame is a planar YUV 4:2:0, 12bpp
			// That is the format of the provided .mp4 file
			// RGB formats will definitely not give a gray image
			// Other YUV image may do so, but untested, so give a warning
			if (pFrame->format != AV_PIX_FMT_YUV420P)
			{
				logging("Warning: the generated file may not be a grayscale image, but could e.g. be just the R component if the video format is RGB");
			}
			// save a grayscale frame into a .pgm file
			save_gray_frame(pFrame->data[0], pFrame->linesize[0], pFrame->width, pFrame->height, frame_filename);
		}
	}
	return 0;
}

static void save_gray_frame(unsigned char *buf, int wrap, int xsize, int ysize, char *filename)
{
	FILE *f;
	int i;
	f = fopen(filename,"w");
	// writing the minimal required header for a pgm file format
	// portable graymap format -> https://en.wikipedia.org/wiki/Netpbm_format#PGM_example
	fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);

	// writing line by line
	for (i = 0; i < ysize; i++)
		fwrite(buf + i * wrap, 1, xsize, f);
	fclose(f);
}


