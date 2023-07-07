
#include <chrono>
#include <iomanip>
#include <stdexcept>
#include <string_view>
#include <iostream>
#include <fstream>
#include <memory>
#include <filesystem>

#include <SFML/Graphics.hpp>

#include "frame.hpp"
#include "lzv.hpp"
#include "Crypt.h"
#include "bitmap.hpp"

namespace
{
	Frame fr1;
	Frame fr2;
	
	DiffFrame df;

	std::string key = "abcdef123456"s;
	auto crfn = base / "stream/64.czs"s;

	UL DICTSZ = 65'000;

	UC W = 64, H = 48;

	std::unique_ptr<decrypt_source> cr_s;
	std::unique_ptr<std::iostream> crypt_stream;

	[[maybe_unused]] TP t1, t2;
	UL i;

	[[maybe_unused]] Frame* curr;
	[[maybe_unused]] Frame* prev;

	[[maybe_unused]] bool did_delta = false;
	RGB_Image img;
	
	[[maybe_unused]] bool want_more = true;
}

#define OFS(fn) unmove(std::ofstream{fn, std::fstream::binary | std::fstream::out})
#define IFS(fn) unmove(std::ifstream{fn, std::fstream::binary | std::fstream::in})

using hrc = std::chrono::high_resolution_clock;

void Main()
{
	if (auto [ok, idx] = paramlookup("-key"); ok)
	{
		key = Params[idx+1];
	}
	if (auto [ok, idx] = paramlookup("-zds"); ok)
	{
		DICTSZ = atoi(Params[idx + 1].c_str());
	}
	if (auto [ok, idx] = paramlookup("-w"); ok)
	{
		W = (UC)std::stoi(Params[idx + 1]);
	}
	if (auto [ok, idx] = paramlookup("-h"); ok)
	{
		H = (UC)std::stoi(Params[idx + 1]);
	}
	bool want_bmp = hasparam("-bmp");
	if (want_bmp) {
		if (!std::filesystem::exists("./stage/00_play"))
			std::filesystem::create_directory("./stage/00_play");
	}

	const int WW = W*10, HH = H*10;

	sf::RenderWindow window(sf::VideoMode(WW, HH), "ddec2 player");

	fr1.resize(W, H);
	fr2.resize(W, H);
	df.resize(W, H);

	if (auto [ok, idx] = paramlookup("-in"); ok)
	{
		crfn = Params[idx + 1];
	}
	std::cout << "attempting to load " << crfn << std::endl;

	crypt_stream = std::make_unique<std::fstream>(crfn, std::fstream::binary | std::fstream::in);
	cr_s = std::make_unique<decrypt_source>(key, *crypt_stream);

	t1 = hrc::now();
	i = 0;

	sf::Image sfimg;
	sfimg.create(WW, HH);

	sf::Texture tex;
	tex.create(WW, HH);

	[[maybe_unused]] bool pause = false;

	while (want_more && window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
				want_more = false;
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Space)
					pause = !pause;
				if (event.key.code == sf::Keyboard::Escape)
					want_more = false;
			}
        }
		
		if (!want_more) break;
		
		if (pause) {
			usleep(100);
			continue;
		}

		curr = (i % 2) ? &fr1 : &fr2;
		prev = (i % 2) ? &fr2 : &fr1;

		if (i)
		{
			if (!cr_s->have(1)) { want_more=false; break; }
			did_delta = cr_s->get(1);
		}
		if (did_delta)
		{
			lzv_decoder_template lz(DICTSZ, *cr_s);
			bool ok = df.LoadT(lz);
			if (!ok) { want_more=false; break; }
			df.mkFrame(*prev);
			assignFrame(*curr, df);
		} else {
			bool ok = curr->Load(*cr_s);
			if (!ok) { want_more=false; break; }
		}

		++i;
		std::cout << i << "\r" << std::flush;

		RGB_Image img;
		FromFrame(*curr, img);
		
		if (want_bmp)
		{
			auto fn = "./stage/00_play/img-"s + std::to_string(i) + ".bmp"s;
			std::ofstream ofs(fn, std::fstream::binary | std::fstream::out);
			SaveBMP(img, ofs);
		}

		sf::Color pix;
		pix.a = 255;
		for(auto y = 0; y < HH; ++y)
		{
			for(auto x = 0; x < WW; ++x)
			{
				pix.r = img.pix[x + y*WW].r;
				pix.g = img.pix[x + y*WW].g;
				pix.b = img.pix[x + y*WW].b;
				sfimg.setPixel(x, HH-y-1, pix);
			}
		}

		tex.update(sfimg);

		while (true)
		{
			t2 = hrc::now();
			if ((t2-t1) > 35ms) break;
		}
		t1 = t2;

		window.clear();
        window.draw(sf::Sprite(tex));
		window.display();
	}
}

int main(int argc, char** argv)
{
	Params.insert(Params.begin(), argv+1, argv+argc);
	Main();
}


