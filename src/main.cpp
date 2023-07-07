
#include <chrono>
#include <iomanip>
#include <stdexcept>
#include <string_view>
#include <iostream>
#include <fstream>
#include <memory>

#include "frame.hpp"
#include "lzv.hpp"
#include "Crypt.h"
#include "perftimer.h"

#include <filesystem>
namespace fs = std::filesystem;

template<typename... Ts>
std::string to_string_f(const char* fmt, const Ts& ...ts)
{
	static char buff[256];
	sprintf(buff, fmt, ts...);
	return {buff};
}

template<typename T>
T& unmove(T&& v) { return v; }

namespace
{
	Frame fr1;
	Frame fr2;
}

#define OFS(fn) unmove(std::ofstream{fn, std::fstream::binary | std::fstream::out})
#define IFS(fn) unmove(std::ifstream{fn, std::fstream::binary | std::fstream::in})

using hrc = std::chrono::high_resolution_clock;

void Main(int argc, char** argv)
{
	std::cout << "Starting..." << std::endl; 
	
	DiffFrame df;

	// -bmp -md -sad -mf
	// -stc -ld -mf
	// -sfc -mf -pfb -t
	
	// -w 128 -h 96 -start 1 -zds 6500 -key qwerty123456 -t -base . -mf

	// 1280x960 in
	/*Params = { 
		"-bmp", "-src", "img", "-dig", "5", "-name", "img_",
		"-md", "-stc", "-errlim", "3500000", "-fast", //, "-max", "500",
		 "-fr"//, "-pfb", "-sal", "-saf", "-sad", "-sll"
	};*/

	// 1280x960 ut
	//Params = { 
	//	"-sfc", "-pfb" //, "-sad", "-sll"
	//};

	Params.insert(Params.begin(), argv+1, argv+argc);

	bool STREAM_TO_CRYPT    = hasparam("-stc");
	bool STREAM_FROM_CRYPT  = hasparam("-sfc");
	bool TIME_IT            = hasparam("-t");
	bool LOAD_FROM_BMP      = hasparam("-bmp");
	bool SAVE_AFTER_LOAD    = hasparam("-sal");
	bool SAVE_AFTER_FRAME   = hasparam("-saf");
	bool MAKE_DIFF          = hasparam("-md");
	bool SAVE_AFTER_DIFF    = hasparam("-sad");
	bool LOAD_DIFF          = hasparam("-ld");
	bool MAKE_FRAME         = hasparam("-mf");
	bool POST_FRAME_BMP     = hasparam("-pfb");
	bool FRAME_REPORT       = hasparam("-fr");
	bool FAST_DIFF          = hasparam("-fast");
	bool SAVE_LOAD_LZV      = hasparam("-sll");

	if (auto [ok, idx] = paramlookup("-base"); ok)
	//if (hasparam("-base"))
	{
		//int i = paramnum("-base");
		base = Params[idx + 1];
	}

	if (!fs::exists(base / "stage"))               fs::create_directory(base / "stage");
	if (!fs::exists(base / "stage/01_loaded"))     fs::create_directory(base / "stage/01_loaded");
	if (!fs::exists(base / "stage/02_toframe"))    fs::create_directory(base / "stage/02_toframe");
	if (!fs::exists(base / "stage/03_mkdiff"))     fs::create_directory(base / "stage/03_mkdiff");
	if (!fs::exists(base / "stage/04_post"))       fs::create_directory(base / "stage/04_post");
	if (!fs::exists(base / "stream"))              fs::create_directory(base / "stream");

	std::string key = "abcdef123456"s;
	std::string crfn = base / "stream/64.czs"s;

	if (auto [ok, idx] = paramlookup("-key"); ok)
	{
		key = Params[idx+1];
	}

	if (auto [ok, idx] = paramlookup("-sn"); ok)
	{
		crfn = Params[idx + 1];
	}

	std::string src = "src2";
	if (auto [ok, idx] = paramlookup("-src"); ok)
	{
		src = Params[idx + 1];
	}

	std::string dig = "%04d";
	if (auto [ok, idx] = paramlookup("-dig"); ok)
	{
		dig = "%0" + Params[idx + 1] + "d";
	}

	UL DICTSZ = 65'000;
	if (auto [ok, idx] = paramlookup("-zds"); ok)
	{
		DICTSZ = std::stoi(Params[idx + 1]);
	}

	UL start = 0;
	if (auto [ok, idx] = paramlookup("-start"); ok)
	{
		start = std::stoi(Params[idx + 1]);
	}

	UL maxcnt = (UL)-1;
	if (auto [ok, idx] = paramlookup("-max"); ok)
	{
		maxcnt = std::stoi(Params[idx + 1]);
	}
	auto nam = "img"s;
	if (auto [ok, idx] = paramlookup("-name"); ok)
	{
		nam = Params[idx + 1];
	}

	UC W = 64, H = 48;
	if (auto [ok, idx] = paramlookup("-w"); ok)
	{
		W = (UC)std::stoi(Params[idx + 1]);
	}
	if (auto [ok, idx] = paramlookup("-h"); ok)
	{
		H = (UC)std::stoi(Params[idx + 1]);
	}

	UL errlim = 165ul * W * H;
	if (auto [ok, idx] = paramlookup("-errlim"); ok)
	{
		errlim = std::stoi(Params[idx + 1]);
	}

	fr1.resize(W, H);
	fr2.resize(W, H);
	df.resize(W, H);

	std::unique_ptr<encrypt_target> cr_t;
	std::unique_ptr<decrypt_source> cr_s;
	std::unique_ptr<std::iostream> crypt_stream;

	if (STREAM_TO_CRYPT && STREAM_FROM_CRYPT)
	{
		std::string err = "-stc and -sfc are exclusive";
		std::cerr << err << std::endl;
		throw err;
	}

	if (STREAM_TO_CRYPT)
	{
		crypt_stream = std::make_unique<std::fstream>(crfn, std::fstream::binary | std::fstream::out);
		cr_t = std::make_unique<encrypt_target>(key, *crypt_stream);
	}

	if (STREAM_FROM_CRYPT)
	{
		crypt_stream = std::make_unique<std::fstream>(crfn, std::fstream::binary | std::fstream::in);
		cr_s = std::make_unique<decrypt_source>(key, *crypt_stream);
	}

	std::ofstream fr;
	if (FRAME_REPORT)
	{
		fr.open("FrameReport.txt", std::fstream::out);
	}

	TP t1, t2;
	PerfTimer perft;
	if (TIME_IT)
	{
		t1 = hrc::now();
		pt = &perft;
	}

	UL i;
	for (i = 0; i<maxcnt; ++i)
	{
		[[maybe_unused]] Frame* curr = (i % 2) ? &fr1 : &fr2;
		[[maybe_unused]] Frame* prev = (i % 2) ? &fr2 : &fr1;

		bool did_delta = i;

		auto num = to_string_f(dig.c_str(), i+start);

		std::cout << num << "\r" << std::flush;

		RGB_Image img;

		if (LOAD_FROM_BMP)
		{
			auto fn = base / src / (nam + num + ".bmp"s);
			std::ifstream ifs{fn, std::fstream::binary | std::fstream::in};
			if (!ifs) break;
			LoadBMP(img, ifs);
		}

		if (SAVE_AFTER_LOAD)
		{
			auto fn = base / ("stage/01_loaded/img"s + num + ".bmp"s);
			SaveBMP(img, OFS(fn));
		}

		if (LOAD_FROM_BMP)
			ToFrame(img, *curr);

		if (SAVE_AFTER_FRAME)
		{
			{
				bitvector bv;
				curr->Save(bv);
				bv.done();
				auto fn = base / ("stage/02_toframe/img"s + num + ".f"s);
				bv.write(OFS(fn));
			}
			{
				RGB_Image tmp;
				FromFrame(*curr, tmp);
				auto fn = base / ("stage/02_toframe/img"s + num + ".bmp"s);
				SaveBMP(tmp, OFS(fn));
			}
		}

		UL acc = 0;
		if (MAKE_DIFF && i)
		{
			if (FAST_DIFF)
				acc = df.mkDiffFast(*prev, *curr);
			else
				acc = df.mkDiff(*prev, *curr);
			if (acc >= errlim)
				did_delta = false;
		}
		if (FRAME_REPORT)
			fr << "Frame " << num << " error " << std::setw(10) << std::right << acc << (did_delta ? "" : " (kf)") << std::endl << std::flush;
		if (SAVE_AFTER_DIFF)
		{
			if (did_delta)
			{
				bitvector bv;
				df.Save(bv);
				bv.done();
				auto fn = base / ("stage/03_mkdiff/img"s + num + ".df"s);
				bv.write(OFS(fn));
			} else {
				bitvector bv;
				curr->Save(bv);
				bv.done();
				auto fn = base / ("stage/03_mkdiff/img"s + num + ".f"s);
				bv.write(OFS(fn));
			}
		}

		if (LOAD_DIFF)
		{
			auto fn = base / ("stage/03_mkdiff/img"s + num + ".f"s);
			std::ifstream in{fn, std::fstream::binary | std::fstream::in};
			if (in)
			{
				streamsource ss(in);
				bool ok = curr->Load(ss);
				if (!ok) break;
				did_delta = false;
			} else {
				fn = base / ("stage/03_mkdiff/img"s + num + ".df"s);
				in.open(fn, std::fstream::binary | std::fstream::in);
				if (!in) break;
				streamsource ss(in);
				bool ok = df.Load(ss);
				if (!ok) break;
				did_delta = true;
			}
		}
		
		debv bv;

		if (STREAM_TO_CRYPT)
		{
			if (i)
				cr_t->put(did_delta, 1);

			if (did_delta)
			{
				nibble_channel nc;
				df.Save(nc);
				nc.done();
				{
					lzv_encoder lz(DICTSZ);
					lz.encode(nc, *cr_t);
				}
				if (SAVE_LOAD_LZV)
				{
					lzv_encoder lz(DICTSZ);
					lz.encode(nc, bv);
					bv.done();
				}
			} else {
				curr->Save(*cr_t);
			}
		}

		if (STREAM_FROM_CRYPT)
		{
			if (i)
			{
				if (!cr_s->have(1)) break;
				did_delta = cr_s->get(1);
				if (did_delta)
				{
					lzv_decoder_template lz(DICTSZ, *cr_s);
					bool ok = df.LoadT(lz);
					if (!ok) break;
				} else {
					bool ok = curr->Load(*cr_s);
					if (!ok) break;
				}
			} else {
				bool ok = curr->Load(*cr_s);
				if (!ok) break;
			}
		}

		if (MAKE_FRAME)
		{
			if (i && did_delta)
			{
				df.mkFrame(*prev);
				assignFrame(*curr, df);
			}
		}
		
		//	df.mkFrame(*prev);
		//	assignFrame(*curr, df);

		if (MAKE_FRAME && POST_FRAME_BMP && SAVE_LOAD_LZV && did_delta)
		{
			lzv_decoder_template lz(DICTSZ, bv);
			DiffFrame df_2;
			df_2.LoadT(lz);
			Frame f_2;
			df_2.mkFrame(*prev);
			assignFrame(f_2, df);
			FromFrame(f_2, img);
			auto fn = base / ("stage/04_post/"s + num + "-delz.bmp"s);
			SaveBMP(img, OFS(fn));
		}

		if (POST_FRAME_BMP)
		{
			FromFrame(*curr, img);
			auto fn = base / ("stage/04_post/"s + num + "-img.bmp"s);
			SaveBMP(img, OFS(fn));
		}
	}

	if (STREAM_TO_CRYPT)
	{
		cr_t->done();
		crypt_stream.reset();
	}

	extern UL longest_sequence;
	extern UL maximum_nibble_channel_load;

	if (TIME_IT)
	{
		t2 = hrc::now();
		auto dur = Dur(t2-t1);
		double s = dur.count();
		std::cout << "\n\n FPS : " << (i/s) << std::endl;

		std::cout << "streamin     " << perft.streamin   .count() << " s\n";
		std::cout << "decrypt      " << perft.decrypt    .count() << " s\n";
		std::cout << "decompress   " << perft.decompress .count() << " s\n";
		std::cout << "makeframe    " << perft.makeframe  .count() << " s\n";
		std::cout << "rest         " << (dur-(perft.streamin+perft.decrypt+perft.decompress+perft.makeframe)).count() << " s\n";
		std::cout << "longest seq  " << longest_sequence << "\n";
		std::cout << "MNCL         " << maximum_nibble_channel_load << std::endl;
		fgetc(stdin);
	}
}

int main(int argc, char** argv)
{
	Main(argc, argv);
}



