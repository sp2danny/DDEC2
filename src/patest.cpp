
#include <iostream>
#include <cstdio>
#include <random>
#include <map>
#include <utility>

// #include "portaudio.h"

void patest()
{
	/*
	std::freopen("/dev/null", "a", stderr);
	
	auto err = Pa_Initialize();
	if (err != paNoError) {
		std::cout << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
	}

	std::cout << "Hello PA World\n";

	err = Pa_Terminate();
	if (err != paNoError) {
		std::cout << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
	}
	*/
}

namespace {

std::random_device r; 
std::default_random_engine e1{r()};

}

int fair_dice()
{
	static std::uniform_int_distribution<int> uniform_dist{1, 6};
	return uniform_dist(e1);
}

int unfair_dice()
{
	//return 6;

	static std::uniform_int_distribution<int> uniform_dist{1, 10};

	switch (uniform_dist(e1))
	{
		case 1:
			return 1;
		case 2:
			return 2;
		case 3:
			return 3;
		case 4:
		case 5:
			return 4;
		case 6:
		case 7:
			return 5;
		case 8:
		case 9:
		case 10:
			return 6;
		default:
			std::unreachable();
	}
}

int dice_add_mod(int d, int delta)
{
	d += delta;
	d -= 1;
	d %= 6;
	d += 1;
	return d;
}

int forward_difference(int d1, int d2)
{
	int add = 1;
	while (true)
	{
		if (dice_add_mod(d1, add) == d2)
			return add;
		++add;
	}
}

int main()
{
	std::map<int, int> m;
	
	for (int i=0; i<100000; ++i)
	{
		int d_1_1 = unfair_dice();
		int d_1_2 = unfair_dice();
		int d_1_3 = unfair_dice();
		int d_1_4 = unfair_dice();
		int d_1_5 = unfair_dice();
		int d_1_6 = unfair_dice();
		int d_1_7 = unfair_dice();
		int d_1_8 = unfair_dice();
		
		#define func forward_difference
		
		int d_2_1 = func(d_1_1, d_1_2);
		int d_2_2 = func(d_1_3, d_1_4);
		int d_2_3 = func(d_1_5, d_1_6);
		int d_2_4 = func(d_1_7, d_1_8);

		int d_3_1 = func(d_2_1, d_2_2);
		int d_3_2 = func(d_2_3, d_2_4);

		[[maybe_unused]]
		int d_4 = func(d_3_1, d_3_2);
		
		#undef func

		m[d_4] += 1;
	}
	
	for (auto&& p : m)
	{
		std::cout << p.first << " : " << p.second << std::endl;
	}

}


