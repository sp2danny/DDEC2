

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <string_view>
#include <filesystem>

extern "C" { 
	#include <libavcodec/avcodec.h> 
	#include <libavformat/avformat.h>
}

int usage()
{
	std::cerr << "usage: fft [s|f] source target";
	return -1;
}

void tostream([[maybe_unused]] const std::string& , [[maybe_unused]] const std::string& )
{
	// bla
}

void tofile([[maybe_unused]] const std::string& src, [[maybe_unused]] const std::string& dst)
{
	// bla
}

int main(int argc, char** argv)
{
	std::vector<std::string> arg;
	for (int i=1; i<argc; ++i)
		arg.push_back(argv[i]);

	AVFormatContext *pFormatContext = avformat_alloc_context();
	if (!pFormatContext) {
		std::cerr << ("ERROR could not allocate memory for Format Context");
		return -1;
	}
		
	if (arg.size() != 3)
		return usage();
	if (arg[0] == "s") {
		tostream(arg[1], arg[2]);
	} else if (arg[0] == "f") {
		tofile(arg[1], arg[2]);
	} else
		return usage();
}

