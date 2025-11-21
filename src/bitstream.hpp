
#pragma once

import std;

#include "core.hpp"

struct bitsource
{
	virtual ~bitsource() = default;
	virtual bool have(int bitcount) =0;
	virtual UL get(int bitcount) =0;
	signed getS(int bitcount);
};

struct bittarget
{
	virtual ~bittarget() = default;
	virtual void put(UL bits, int bitcount) =0;
	virtual void done() =0;
};

struct bitvector : bittarget
{
	virtual void put(UL bits, int bitcount) override;
	virtual void done() override;
	BVec vec;
	UL bsf = 0;
	UC cnt = 0;
	void write(std::ostream&);
};

struct debv : bitsource, bittarget
{
	virtual bool have(int bitcount) override;
	virtual UL get(int bitcount) override;
	virtual void put(UL bits, int bitcount) override;
	virtual void done() override;

	std::deque<UC> vec;
	UL  bsf_in = 0;
	int cnt_in = 0;
	UL  bsf_ut = 0;
	int cnt_ut = 0;
};

struct streamsource : bitsource
{
	streamsource(std::istream& in) : in(in) {}
	virtual bool have(int bitcount) override;
	virtual UL get(int bitcount) override;
	void make(int bitcount);
	std::istream& in;
	UL  bsf = 0;
	int cnt = 0;
};

struct nibble_channel : bitsource, bittarget
{
	virtual bool have(int bitcount) override;
	virtual UL get(int bitcount) override;
	virtual void put(UL bits, int bitcount) override;
	virtual void done() override;

	std::deque<UC> nibbles;
};

struct cyclic_nibble_channel //: bitsource, bittarget
{
	cyclic_nibble_channel();
	static const int N = 16*1024;
	static const UL mask = N-1;
	bool have(int bitcount);// override;
	UL get(int bitcount);// override;
	void put(UL bits, int bitcount);// override;
	void done();// override;

	//std::vector<UC> nibbles;
	std::array<UC, N> nibbles;
	int inp=0, utp=0, sz=0;

	int size() const;
	UC& operator[](int);
	UC operator[](int) const;
};



