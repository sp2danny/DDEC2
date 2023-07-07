
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

	TP t1, t2;
	UL i;

	Frame* curr;
	Frame* prev;

	bool did_delta = false, want_more = true;
}

#define OFS(fn) unmove(std::ofstream{fn, std::fstream::binary | std::fstream::out})
#define IFS(fn) unmove(std::ifstream{fn, std::fstream::binary | std::fstream::in})

using hrc = std::chrono::high_resolution_clock;

void FromFrameSF(const Frame& frame, sf::Image& image)
{
	UL W = frame.w*10;
	UL H = frame.h*10;
	sf::Color c;

	for (UL y = 0; y < H; ++y)
	{
		for (UL x = 0; x < W; ++x)
		{
			auto bl_x = UC(x / 10);
			auto bl_y = UC(y / 10);
			auto& bl = frame.pix(bl_x, bl_y);
			auto bl_ix = x % 10;
			auto bl_iy = y % 10;
			HSV hsv = bl.Pix(bl_ix, bl_iy);
			hsv.h = hsv.h << (8 - HBIT);
			hsv.s = hsv.s << (8 - SBIT);
			hsv.v = hsv.v << (8 - VBIT);
			auto rgb = HsvToRgb(hsv);
			c.r = rgb.r; c.g = rgb.g; c.b = rgb.b;
			image.setPixel(x, H-y-1, c);
		}
	}
}


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

	bool pause = false;

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

		FromFrameSF(*curr, sfimg);

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


