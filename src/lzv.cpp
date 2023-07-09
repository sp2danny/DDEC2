
#include <iostream>

#include "lzv.hpp"

#include "perftimer.hpp"

static_assert(std::numeric_limits<UC>::digits ==  8);
static_assert(std::numeric_limits<UL>::digits == 32);

void lzv_core::init(int max)
{
	m4.init(max);
	for (UC c = 0; c < 16; ++c)
	{
		BVec bv = {c};
		m4.add(bv);
	}
	current_bd = 5;
	next_token = 16;
	max_token = max;
	have_prev = false;
	prev.clear();
}

UC pre1[] = {0x0, 0x0, 0xf, 0xf, 0xf};
const int NN = 1;

void lzv_encoder::encode(nibble_channel& src, bittarget& dst)
{
	UL curr=0, nn = (UL)src.nibbles.size();

	auto& nbl = src.nibbles;

	#ifndef NDEBUG
	UL stats_pre_n = 0;
	UL stats_cod_n = 0;
	UL stats_pre_l = 0;
	UL stats_cod_l = 0;
	#endif

	auto max_rle = [&]() -> UL
	{
		auto max_t = 1 << current_bd;
		return (max_t - next_token) / NN;
	};

	auto mat = [&](UL p, UC* t) -> bool
	{
		for (UL i = 0ul; i < 5; ++i)
			if (nbl[p + i] != t[i])
				return false;
		return true;
	};
	auto tok_rep_l = [&](UC* t, UL maxrep) -> UL
	{
		UL p = curr;
		UL rep = 0;
		while (true)
		{
			if ((p + 5) >= nn) break;
			if (!mat(p, t))
				break;
			++rep;
			if (rep >= maxrep) return rep;
			p += 5;
		}
		return rep;
	};

	while (true)
	{
		if (curr>=nn) break;

		UC token = nbl[curr];

		// 1 - preset sequences
		UL max_n = std::min( UL{nn-curr} / 5, max_rle());
		if (max_n)
		{
			UL trl = tok_rep_l(pre1, max_n);
			if (trl)
			{
				//trl = std::min(trl, max_rle());
				dst.put(next_token + NN * (trl - 1) + 0, current_bd);
				curr += trl * 5;

				#ifndef NDEBUG
				stats_pre_n += 1;
				stats_pre_l += 5 * trl;
				#endif

				prev.clear();
				continue;
			}
		}

		// 2 - find longest sequence in tab
		BVec bv = {token};
		while (true)
		{
			++curr;
			if (curr >= nn)
				break;
			token = nbl[curr];
			bv.push_back(token);
			auto r = m4.find(bv);
			if (r.first) continue;
			bv.pop_back();
			break;
		}
		auto r = m4.find(bv);
		assert(r.first);
		dst.put(r.second, current_bd);

		#ifndef NDEBUG
		stats_cod_n += 1;
		stats_cod_l += (UL)bv.size();
		#endif

		if (curr >= nn)
			break;

		if (next_token >= max_token)
			continue;

		if (prev.empty())
		{
			prev = std::move(bv);
			continue;
		}

		BVec ny = std::move(prev);
		ny.push_back(bv[0]);
		bool ok = m4.add(ny);
		if (ok)
		{
			++next_token;
			if (next_token >= (1ul << current_bd))
				++current_bd;
			prev = std::move(bv);
		}
		else {
			prev = std::move(bv);
		}
	}

	#ifndef NDEBUG
	std::cout << " code entries " << stats_cod_n;
	if (stats_cod_n)
		std::cout << " avg size " << (float(stats_cod_l) / float(stats_cod_n));
	std::cout << std::endl;
	std::cout << " pre entries " << stats_pre_n;
	if (stats_pre_n)
		std::cout << " avg size " << (float(stats_pre_l) / float(stats_pre_n));
	std::cout << std::endl;
	#endif

}

lzv_decoder::lzv_decoder(UL max, bitsource& src)
	: source(src)
{
	init(max);
	have_prev = false;
}

bool lzv_decoder::have(int bitcount)
{
	assert((bitcount%4)==0);
	int nc = bitcount/4;
	make(nc);
	return std::ssize(nibbles) >= nc;
}

UL lzv_decoder::get(int bitcount)
{
	(void)bitcount;
	assert(bitcount == 4);
	make(1);
	UC ret = (UC)nibbles.get(4);
	return ret;
}

void lzv_decoder::make(int nc)
{
	while (true)
	{
		if (std::ssize(nibbles) >= nc) return;

		if (!source.have(current_bd)) return;

		UL code = source.get(current_bd);
		bool ok;
		BVec bv;

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
			UL x = code - next_token;
			UL nn = (x / NN) + 1;
			UC* pp = 0;
			switch (x % NN)
			{
				case 0: pp = pre1; break;
				default: assert(false);
			}
			while (nn--)
				for (int i = 0; i < 5; ++i)
					nibbles.put(pp[i], 4);
			// no table growth after preset entry, just continue
			have_prev = false;
			if (pt)
				pt->decompress += Dur(hrc::now()-t1);
			continue;
		}

		// prepare expand table
		// the code size might have expanded here
		if (next_token >= max_token)
		{
			have_prev = false;
			if (pt)
				pt->decompress += Dur(hrc::now() - t1);
			continue;
		}

		if (have_prev)
		{
			//prev.push_back(bv.front());
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
		//prev = std::move(bv);
		if (pt)
			pt->decompress += Dur(hrc::now() - t1);
	}

}



