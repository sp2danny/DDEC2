
#pragma once

#include <cassert>
#include <chrono>

#include "core.hpp"

#include "nibblemap.hpp"
#include "bitstream.hpp"
#include "perftimer.hpp"

struct lzv_core
{
	void init(int max);

	//map4 m4;
	fbct2 m4;
	UC current_bd;
	UL next_token;
	UL max_token;

	bool have_prev;
	BVec prev;
	UL prev_code;
};

struct lzv_encoder : lzv_core
{
	lzv_encoder(int max)
	{
		init(max);
	}

	void encode(nibble_channel&, bittarget& dst);
};

struct lzv_decoder : lzv_core, bitsource
{
	lzv_decoder(UL, bitsource&);

	bitsource& source;

	virtual bool have(int bitcount) override;
	virtual UL get(int bitcount) override;

private:
	void make(int nibblecount);

	cyclic_nibble_channel nibbles;
};

template<typename SRC>
struct lzv_decoder_template : lzv_core
{
	lzv_decoder_template(int, SRC&);

	SRC& source;

	bool have(int bitcount);
	UL get(int bitcount);

	signed getS(int bitcount)
	{
		auto u = get(bitcount);
		auto shift = 32 - bitcount;
		signed sl = u << shift; sl >>= shift;
		return sl;
	}

private:
	void make(int nibblecount);

	cyclic_nibble_channel nibbles;
	BVec bv;

};

// ----------------------------------------------------------------------------

template<typename SRC>
lzv_decoder_template<SRC>::lzv_decoder_template(int max, SRC& src)
	: source(src)
{
	init(max);
	have_prev = false;
	bv.reserve(256);
}

template<typename SRC>
bool lzv_decoder_template<SRC>::have(int bitcount)
{
	assert((bitcount % 4) == 0);
	int nc = bitcount / 4;
	make(nc);
	return std::ssize(nibbles) >= nc;
}

template<typename SRC>
UL lzv_decoder_template<SRC>::get(int bitcount)
{
	(void)bitcount;
	assert(bitcount == 4);
	//make(1);
	UC ret = (UC)nibbles.get(4);
	return ret;
}

extern UC pre1[5];
extern const int NN;

template<typename SRC>
void lzv_decoder_template<SRC>::make(int nc)
{
	while (true)
	{
		if (std::ssize(nibbles) >= nc) return;

		if (!source.have(current_bd)) return;

		UL code = source.get(current_bd);
		bool ok;

		if (code < next_token)
		{
			ok = m4.lookup(code, bv);
			assert(ok);
			for (auto c : bv)
				nibbles.put(c, 4);
		}
		else {
			int nn = 1 + code - next_token;
			while (nn--)
				for (auto c : pre1)
					nibbles.put(c, 4);
			// no table growth after preset entry, just continue
			have_prev = false;
			continue;
		}

		// prepare expand table
		// the code size might have expanded here
		if (next_token >= max_token)
		{
			have_prev = false;
			continue;
		}

		if (have_prev)
		{
			ok = m4.addh(bv.front(), prev_code);
			if (ok)
			{
				++next_token;
				if (next_token >= (1ul << current_bd))
					++current_bd;
			}
		}

		have_prev = true;
		prev_code = code;

	}

}


