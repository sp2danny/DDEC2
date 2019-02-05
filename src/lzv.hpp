
#pragma once

#include <cassert>
#include <chrono>

#include "core.hpp"

#include "nibblemap.hpp"
#include "bitstream.hpp"
#include "perftimer.h"

struct lzv_core
{
	void init(UL max);

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
	lzv_encoder(UL max)
	{
		init(max);
	}

	void encode(nibble_channel&, bittarget& dst);

};

struct lzv_decoder : lzv_core, bitsource
{
	lzv_decoder(UL, bitsource&);

	bitsource& source;

	virtual bool have(UC bitcount) override;
	virtual UL get(UC bitcount) override;

private:
	void make(UL nibblecount);

	cyclic_nibble_channel nibbles;
	//std::deque<UC> nibbles;
};

template<typename SRC>
struct lzv_decoder_template : lzv_core
{
	//lzv_decoder_template()
	lzv_decoder_template(UL, SRC&);

	SRC& source;

	bool have(UC bitcount);
	UL get(UC bitcount);

	signed getS(UC bitcount)
	{
		auto u = get(bitcount);
		auto shift = 32 - bitcount;
		signed sl = u << shift; sl >>= shift;
		return sl;
	}

private:
	void make(UL nibblecount);

	cyclic_nibble_channel nibbles;
	BVec bv;

};



// ----------------------------------------------------------------------------


template<typename SRC>
lzv_decoder_template<SRC>::lzv_decoder_template(UL max, SRC& src)
	: source(src)
{
	init(max);
	have_prev = false;
	bv.reserve(256);
}

template<typename SRC>
bool lzv_decoder_template<SRC>::have(UC bitcount)
{
	assert((bitcount % 4) == 0);
	UL nc = bitcount / 4;
	make(nc);
	return nibbles.size() >= nc;
}

template<typename SRC>
UL lzv_decoder_template<SRC>::get(UC bitcount)
{
	(void)bitcount;
	assert(bitcount == 4);
	//make(1);
	UC ret = (UC)nibbles.get(4);
	return ret;
}

extern UC pre1[5];
extern const UL NN;


template<typename SRC>
void lzv_decoder_template<SRC>::make(UL nc)
{
	while (true)
	{
		if (nibbles.size() >= nc) return;

		if (!source.have(current_bd)) return;

		UL code = source.get(current_bd);
		bool ok;

		TP t1;
		if (pt) t1 = hrc::now();

		if (code < next_token)
		{
			ok = m4.lookup(code, bv);
			assert(ok);
			for (auto c : bv)
				nibbles.put(c, 4);
		}
		else {
			UL nn = 1 + code - next_token;
			while (nn--)
				for (auto c : pre1)
					nibbles.put(c, 4);
			// no table growth after preset entry, just continue
			have_prev = false;
			if (pt)
				//pt->decompress += Dur(hrc::now() - t1)
				;
			continue;
		}

		// prepare expand table
		// the code size might have expanded here
		if (next_token >= max_token)
		{
			have_prev = false;
			if (pt)
				//pt->decompress += Dur(hrc::now() - t1)
				;
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

		if (pt)
			//pt->decompress += Dur(hrc::now() - t1)
			;
	}

}


