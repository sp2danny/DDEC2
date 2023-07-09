
#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <filesystem>

using namespace std::literals;

typedef unsigned char UC;
typedef unsigned int UL;

static_assert( sizeof(UC) == 1 );
static_assert( sizeof(UL) == 4 );

typedef std::vector<UC> BVec;

#pragma pack(push, 1)

struct HSV
{
	UC h, s, v;
};

struct RGB
{
	UC r, g, b;
};

#pragma pack(pop)

static_assert( sizeof(RGB) == 3 );
static_assert( sizeof(HSV) == 3 );

inline std::filesystem::path base = "."s;

inline std::vector<std::string> Params;

inline bool hasparam(std::string_view str)
{
	for(auto&& x : Params)
		if (x==str)
			return true;
	return false;
}

inline int paramnum(std::string_view str)
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

inline auto paramlookup(std::string_view str)
{
	struct { bool ok; int index; } res = { false, 0 };
	if (Params.empty()) return res;
	int i = 0;
	for (auto&& x : Params)
	{
		if (x == str) {
			res = {true, i};
			break;
		}
		++i;
	}
	return res;
}
