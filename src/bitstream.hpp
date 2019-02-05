
#pragma once

#include "core.hpp"

#include <iosfwd>
#include <array>
#include <deque>

struct bitsource
{
	virtual ~bitsource() = default;
	virtual bool have(UC bitcount) =0;
	virtual UL get(UC bitcount) =0;
	signed long getS(UC bitcount);
};

struct bittarget
{
	virtual ~bittarget() = default;
	virtual void put(UL bits, UC bitcount) =0;
	virtual void done() =0;
};

struct bitvector : bittarget
{
	virtual void put(UL bits, UC bitcount) override;
	virtual void done() override;
	BVec vec;
	UL bsf = 0;
	UC cnt = 0;
	void write(std::ostream&);
};

struct debv : bitsource, bittarget
{
	virtual bool have(UC bitcount) override;
	virtual UL get(UC bitcount) override;
	virtual void put(UL bits, UC bitcount) override;
	virtual void done() override;

	std::deque<UC> vec;
	UL bsf_in = 0;
	UC cnt_in = 0;
	UL bsf_ut = 0;
	UC cnt_ut = 0;
};

struct streamsource : bitsource
{
	streamsource(std::istream& in) : in(in) {}
	virtual bool have(UC bitcount) override;
	virtual UL get(UC bitcount) override;
	void make(UC bitcount);
	std::istream& in;
	UL bsf = 0;
	UC cnt = 0;
};

struct nibble_channel : bitsource, bittarget
{
	virtual bool have(UC bitcount) override;
	virtual UL get(UC bitcount) override;
	virtual void put(UL bits, UC bitcount) override;
	virtual void done() override;

	std::deque<UC> nibbles;
};

struct cyclic_nibble_channel //: bitsource, bittarget
{
	cyclic_nibble_channel();
	static const UL N = 16*1024;
	static const UL mask = N-1;
	bool have(UC bitcount);// override;
	UL get(UC bitcount);// override;
	void put(UL bits, UC bitcount);// override;
	void done();// override;

	//std::vector<UC> nibbles;
	std::array<UC, N> nibbles;
	UL inp=0, utp=0, sz=0;

	UL size() const;
	UC& operator[](UL);
	UC operator[](UL) const;
};



