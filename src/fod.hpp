
#pragma once

#include <random>
#include <vector>
#include <string>
#include <string_view>

typedef unsigned char UC;
typedef unsigned int UL;

static_assert( sizeof(UC) == 1 );
static_assert( sizeof(UL) == 4 );

typedef std::vector<UC> BVec;

#include "mersenne-twister.hpp"

typedef MT Eng;

// typedef std::mt19937 Eng;

struct Crypt
{
	Crypt(std::string);
	Crypt(const Crypt&) = default;
	Crypt(Crypt&&) = default;
	
	~Crypt() { clear(); }

	static constexpr int maxblock() { return MXBLK; }

	void decrypt (UC* block, int size);
	
	void clear();

private:
	static const int MXBLK = 1<<16;
	static const int MASK = MXBLK-1;

private:
	UL next();

	void loadup_big      (int size, UC* block);
	void loadup_scramble (int size, UC* block);
	void loadup_xorpass  (int size, UC* block);

private:
	std::vector<Eng> keys;
	int kidx;
};

