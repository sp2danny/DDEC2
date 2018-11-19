
#include "stdafx.h"

#include <chrono>
#include <iomanip>
#include <stdexcept>
#include <string_view>
#include <iostream>
#include <fstream>
#include <memory>

#include <boost/filesystem.hpp>

#include "frame.hpp"
#include "lzv.hpp"
#include "Crypt.h"
#include "perftimer.h"

namespace fs = boost::filesystem;

/*extern*/ std::vector<std::string> Params;

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

bool hasparam(std::string_view str)
{
	for(auto&& x : Params)
		if (x==str)
			return true;
	return false;
}

int paramnum(std::string_view str)
{
	int i = 0;
	for (auto&& x : Params)
	{
		if (x == str)
			return i;
		++i;
	}
	return -1;
}

void Main()
{

	using namespace std;

	DiffFrame df;

	// -bmp -md -sad -mf
	// -stc -ld -mf
	// -sfc -mf -pfb -t

	//Params = { "-bmp", "-md", "-sad", "-mf" };
	//Params = { "-stc", "-ld", "-mf" };
	//Params = { "-sfc", "-mf", "-pfb", "-t" };
	//Params = { "-sfc", "-mf", "-pfb", "-key", "abc" };
	//Params = { "-stc", "-bmp", "-md", "-mf", "-src", "src3", "-sad", "-pfb", "-start", "9998", "-max", "200", "-dig", "5" };


	//Params = { "-md", "-src", "src3", "-bmp", "-dig", "5", "-start", "9998", "-sad", "-mf", "-fr" };
	//Params = { "-stc", "-ld", "-dig", "5", "-start", "9998", "-mf", "-key", "abcd1234ABCD____"  };
	//Params = { "-sfc", "-mf", "-pfb", "-max", "1600", "-t"};



	// 1280x720
	//Params = { "-md", "-src", "src4", "-bmp", "-dig", "5", "-start", "10000", "-sad", "-mf", "-fr", "-w", "128", "-h", "72", "-max", "215", "-t" };
	//Params = { "-stc", "-ld", "-dig", "5", "-start", "10000", "-mf", "-w", "128", "-h", "72"};
	//Params = { "-sfc", "-mf", "-pfb", "-w", "128", "-h", "72" };


	// 640x360
	Params = { 
		"-md", "-src", "src", "-bmp", "-dig", "5", "-name", "img_", "-start", "0", "-mf", "-fr", "-w", "128", "-h", "72", "-t", "-stc",
		"-errlim", "3500000"//, "-max", "500",
		"-fast", "-zds", "65000", "-key", "qwerty123456"
	};

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

	if (hasparam("-base"))
	{
		int i = paramnum("-base");
		base = Params[i + 1];
		base += "/";
	}

	if (!fs::exists(base + "stage"))               fs::create_directory(base + "stage");
	if (!fs::exists(base + "stage/01_loaded"))     fs::create_directory(base + "stage/01_loaded");
	if (!fs::exists(base + "stage/02_toframe"))    fs::create_directory(base + "stage/02_toframe");
	if (!fs::exists(base + "stage/03_mkdiff"))     fs::create_directory(base + "stage/03_mkdiff");
	if (!fs::exists(base + "stage/04_post"))       fs::create_directory(base + "stage/04_post");
	if (!fs::exists(base + "stream"))              fs::create_directory(base + "stream");

	std::string key = "abcdef123456"s;
	std::string crfn = base + "stream/64.czs"s;

	if (hasparam("-key"))
	{
		int i = paramnum("-key");
		key = Params[i+1];
	}

	if (hasparam("-sn"))
	{
		int i = paramnum("-sn");
		crfn = Params[i + 1];
	}

	string src = "src2";
	if (hasparam("-src"))
	{
		int i = paramnum("-src");
		src = Params[i + 1];
	}

	string dig = "%04d";
	if (hasparam("-dig"))
	{
		int i = paramnum("-dig");
		dig = "%0" + Params[i + 1] + "d";
	}

	UL DICTSZ = 65'000;
	if (hasparam("-zds"))
	{
		int i = paramnum("-zds");
		DICTSZ = atoi(Params[i + 1].c_str());
	}

	UL start = 0;
	if (hasparam("-start"))
	{
		int i = paramnum("-start");
		start = atoi(Params[i + 1].c_str());
	}

	UL maxcnt = (UL)-1;
	if (hasparam("-max"))
	{
		int i = paramnum("-max");
		maxcnt = atoi(Params[i + 1].c_str());
	}
	auto nam = "img"s;
	if (hasparam("-name"))
	{
		int i = paramnum("-name");
		nam = Params[i + 1];
	}


	UC W = 64, H = 48;
	if (hasparam("-w"))
	{
		int i = paramnum("-w");
		W = (UC)atoi(Params[i + 1].c_str());
	}
	if (hasparam("-h"))
	{
		int i = paramnum("-h");
		H = (UC)atoi(Params[i + 1].c_str());
	}

	UL errlim = 165ul * W * H;
	if (hasparam("-errlim"))
	{
		int i = paramnum("-errlim");
		errlim = atoi(Params[i + 1].c_str());
	}

	fr1.resize(W, H);
	fr2.resize(W, H);
	df.resize(W, H);

	unique_ptr<encrypt_target> cr_t;
	unique_ptr<decrypt_source> cr_s;
	unique_ptr<std::iostream> crypt_stream;

	if (STREAM_TO_CRYPT && STREAM_FROM_CRYPT)
	{
		std::string err = "-stc and -sfc are exclusive";
		std::cerr << err << endl;
		throw err;
	}

	if (STREAM_TO_CRYPT)
	{
		crypt_stream = make_unique<std::fstream>(crfn, std::fstream::binary | std::fstream::out);
		cr_t = make_unique<encrypt_target>(key, *crypt_stream);
	}

	if (STREAM_FROM_CRYPT)
	{
		crypt_stream = make_unique<std::fstream>(crfn, std::fstream::binary | std::fstream::in);
		cr_s = make_unique<decrypt_source>(key, *crypt_stream);
	}

	ofstream fr;
	if (FRAME_REPORT)
	{
		fr.open("FrameReport.txt", std::fstream::out);
	}

	TP t1, t2;
	PerfTimer perft;
	if (TIME_IT)
	{
		t1 = hrc::now();
		//pt = &perft;
	}

	UL i;
	for (i = 0; i<maxcnt; ++i)
	{
		[[maybe_unused]] Frame* curr = (i % 2) ? &fr1 : &fr2;
		[[maybe_unused]] Frame* prev = (i % 2) ? &fr2 : &fr1;

		bool did_delta = i;

		auto num = to_string_f(dig.c_str(), i+start);

		cout << num << "\r";

		RGB_Image img;

		if (LOAD_FROM_BMP)
		{
			auto fn = base + src + "/" + nam + num + ".bmp"s;
			std::ifstream ifs{fn, std::fstream::binary | std::fstream::in};
			if (!ifs) break;
			LoadBMP(img, ifs);
		}

		if (SAVE_AFTER_LOAD)
		{
			auto fn = base + "stage/01_loaded/img"s + num + ".bmp"s;
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
				auto fn = base + "stage/02_toframe/img"s + num + ".f"s;
				bv.write(OFS(fn));
			}
			{
				RGB_Image tmp;
				FromFrame(*curr, tmp);
				auto fn = base + "stage/02_toframe/img"s + num + ".bmp"s;
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
			(fr << "Frame " << num << " error " << setw(10) << right << acc << "  " << (did_delta ? "" : "(kf)") << endl).flush();
		if (SAVE_AFTER_DIFF)
		{
			if (did_delta)
			{
				bitvector bv;
				df.Save(bv);
				bv.done();
				auto fn = base + "stage/03_mkdiff/img"s + num + ".df"s;
				bv.write(OFS(fn));
			} else {
				bitvector bv;
				curr->Save(bv);
				bv.done();
				auto fn = base + "stage/03_mkdiff/img"s + num + ".f"s;
				bv.write(OFS(fn));
			}
		}

		if (LOAD_DIFF)
		{
			auto fn = base + "stage/03_mkdiff/img"s + num + ".f"s;
			std::ifstream in{fn, std::fstream::binary | std::fstream::in};
			if (in)
			{
				streamsource ss(in);
				bool ok = curr->Load(ss);
				if (!ok) break;
				did_delta = false;
			} else {
				fn = base + "stage/03_mkdiff/img"s + num + ".df"s;
				in.open(fn, std::fstream::binary | std::fstream::in);
				if (!in) break;
				streamsource ss(in);
				bool ok = df.Load(ss);
				if (!ok) break;
				did_delta = true;
			}
		}

		if (STREAM_TO_CRYPT)
		{
			if (i)
			{
				if (did_delta)
				{
					cr_t->put(1,1);
					nibble_channel nc;
					df.Save(nc);
					nc.done();
					lzv_encoder lz(DICTSZ);
					lz.encode(nc, *cr_t);
				} else {
					cr_t->put(0, 1);
					curr->Save(*cr_t);
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
					lzv_decoder_template<decltype(*cr_s)> lz(DICTSZ, *cr_s);
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

		if (POST_FRAME_BMP)
		{
			FromFrame(*curr, img);
			auto fn = base + "stage/04_post/img"s + num + ".bmp"s;
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
		cout << "\n FPS : " << (i/s) << endl;

		cout << "streamin     " << perft.streamin   .count() << " s\n";
		cout << "decrypt      " << perft.decrypt    .count() << " s\n";
		cout << "decompress   " << perft.decompress .count() << " s\n";
		cout << "makeframe    " << perft.makeframe  .count() << " s\n";
		cout << "rest         " << (dur-(perft.streamin+perft.decrypt+perft.decompress+perft.makeframe)).count() << " s\n";
		cout << "longest seq  " << longest_sequence << "\n";
		cout << "MNCL         " << maximum_nibble_channel_load << "\n";
		fgetc(stdin);
	}

}


int main()
{
	Main();
}



