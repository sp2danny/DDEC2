
#pragma once

#include <chrono>

using hrc = std::chrono::high_resolution_clock;

typedef std::chrono::time_point<hrc> TP;
typedef std::chrono::duration<double,std::ratio<1,1>> Dur;

struct PerfTimer
{
	Dur streamin{};
	Dur decrypt{};
	Dur decompress{};
	Dur makeframe{};
};

inline PerfTimer* pt = nullptr;

