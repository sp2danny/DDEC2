
#pragma once

#include <vector>
#include <string>

using namespace std::literals;

typedef unsigned char UC;
typedef unsigned int UL;

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

inline std::string base = "C:/tmp/DDEC/bmp/"s;

#define abstract =0

//inline constexpr UL max_dframe_nibble_count = 466944;

//static_assert( max_dframe_nibble_count == ((2+25+25+100)*64*48) );

