
#pragma once

#include <vector>
#include <string>

using namespace std::literals;

typedef unsigned char UC;
typedef unsigned int UL;

static_assert( sizeof(UC) == 1 );
static_assert( sizeof(UL) == 4 );

typedef std::vector<UC> BVec;

struct HSV
{
	UC h, s, v;
};

struct RGB
{
	UC r, g, b;
};

static_assert( sizeof(RGB) == 3 );
static_assert( sizeof(HSV) == 3 );

inline std::string base = "./img/"s;



