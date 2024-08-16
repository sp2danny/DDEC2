
#include <algorithm>
#include <fstream>
#include <chrono>
#include <cassert>
#include <iostream>

#include "fod.hpp"

static UL make_4b(const char* str)
{
	UL k = 0;
	k |= UL(UC(str[0])) <<  0;
	k |= UL(UC(str[1])) <<  8;
	k |= UL(UC(str[2])) << 16;
	k |= UL(UC(str[3])) << 24;
	return k;
}

void Crypt::clear()
{
	for (auto& x : keys)
		x.seed(0);
}

Crypt::Crypt(std::string key)
{
	while (key.length()%4)
		key += ' ';

	auto s = key.c_str();

	int n = std::ssize(key);
	keys.clear();
	while (n > 0)
	{
		keys.emplace_back();
		keys.back().seed(make_4b(s));
		n -= 4;
		s += 4;
	}
	kidx = 0;
}

UL Crypt::next()
{
	UL ret = keys[kidx]();
	kidx = (kidx + 1) % std::ssize(keys);
	return ret;
}

void Crypt::loadup_big(int size, UC* block)
{
	assert(size >= 0);
	assert(size <= MAXBLK);
	assert(block);

	int n = std::ssize(keys);
	for (int ks = 0; ks < n; ++ks)
	{
		kidx = ks;
		loadup_scramble(size, block);
		loadup_xorpass(size, block);
	}
}

void Crypt::loadup_scramble(int size, UC* block)
{
	assert(size >= 0);
	assert(size <= MXBLK);
	assert(block);

	if (size == MXBLK) [[likely]]
	{
		for (int i = 0; i < MXBLK; ++i)
		{
			UL k = next() & MASK;
			std::swap(block[i], block[k]);
		}
	} else {
		for (int i = 0; i < size; ++i)
		{
			UL k = next() % size;
			std::swap(block[i], block[k]);
		}
	}
}

void Crypt::loadup_xorpass(int size, UC* block)
{
	assert(size >= 0);
	assert(size <= maxblock());
	assert(block);
	
	for (int i = 0; i < size; ++i)
	{
		UC k = next() & 0xff;
		block[i] ^= k;
	}
}

void Crypt::decrypt(UC* block, int size)
{
	assert(block);
	assert(size >= 0);

	while (size)
	{
		if (size >= MXBLK) [[likely]]
		{
			loadup_big(MXBLK, block);
			size -= MXBLK;
			block += MXBLK;
		} else {
			loadup_big(size, block);
			break;
		}
	}
}

