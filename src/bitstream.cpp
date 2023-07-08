
#include <cassert>
#include <iostream>

#include "bitstream.hpp"

signed bitsource::getS(int bitcount)
{
	UL u = get(bitcount);
	int shift = 32 - bitcount;
	signed sl = u << shift; sl >>= shift;
	return sl;
}

void bitvector::put(UL bits, int bitcount)
{
	bsf <<= bitcount;
	bsf |= bits;
	cnt += bitcount;
	while (cnt >= 8)
	{
		UC c = (bsf >> (cnt-8)) & 0xff;
		vec.push_back(c);
		cnt -= 8;
	}
}

void bitvector::done()
{
	if (cnt)
		put(0, 8-cnt);
	assert(!cnt);
}

void bitvector::write(std::ostream& out)
{
	out.write((char*)vec.data(), vec.size());
}

void streamsource::make(int bitcount)
{
	while (cnt < bitcount)
	{
		char c;
		if (!in.get(c)) return;
		UC uc = (UC)c;
		bsf = (bsf<<8) | uc;
		cnt += 8;
	}
}

bool streamsource::have(int bitcount)
{
	make(bitcount);
	if (cnt < bitcount)
	{
		std::cerr << "did not have " << (int)bitcount << " bits" << std::endl;
	}
	return cnt >= bitcount;
}

UL streamsource::get(int bitcount)
{
	make(bitcount);
	assert(cnt >= bitcount);
	UL ret = bsf >> (cnt-bitcount);
	cnt -= bitcount;
	bsf &= ((1<<cnt)-1);
	return ret;
}

bool nibble_channel::have(int bitcount)
{
	assert((bitcount%4)==0);
	if (std::ssize(nibbles) < (bitcount/4))
	{
		std::cerr << "did not have " << (int)bitcount << " bits" << std::endl;
	}
	return std::ssize(nibbles) >= (bitcount/4);
}

UL nibble_channel::get(int bitcount)
{
	(void)bitcount;
	assert(bitcount == 4);
	assert(!nibbles.empty());
	UC c = nibbles.front();
	nibbles.pop_front();
	return c;
}

void nibble_channel::put(UL bits, int bitcount)
{
	(void)bitcount;
	assert(bitcount==4);
	nibbles.push_back((UC)bits);
}

void nibble_channel::done()
{
}

// ----------------------------------------------------------------------------

bool debv::have(int bitcount)
{
	int tbsf = std::ssize(vec) * 8 + cnt_in + cnt_ut;
	if (tbsf < bitcount)
	{
		std::cerr << "did not have " << (int)bitcount << " bits" << std::endl;
	}
	return tbsf >= bitcount;
}

UL debv::get(int bitcount)
{
	while (cnt_ut < bitcount)
	{
		if (!vec.empty())
		{
			UC uc = vec.back();
			vec.pop_back();
			cnt_ut += 8;
			bsf_ut = (bsf_ut<<8) | uc;
		} else {
			assert(cnt_in);
			cnt_ut += cnt_in;
			bsf_ut = (bsf_ut<<cnt_in) | bsf_in;
			cnt_in = 0;
			bsf_in = 0;
		}
	}
	UL ret = bsf_ut >> (cnt_ut-bitcount);
	cnt_ut -= bitcount;
	bsf_ut &= ((1<<cnt_ut)-1);
	return ret;
}

void debv::put(UL bits, int bitcount)
{
	bsf_in <<= bitcount;
	bsf_in |= bits;
	cnt_in += bitcount;
	while (cnt_in >= 8)
	{
		UC c = (bsf_in >> (cnt_in-8)) & 0xff;
		vec.push_back(c);
		cnt_in -= 8;
	}
}

void debv::done()
{
	if (cnt_in)
		put(0, 8-cnt_in);
	assert(!cnt_in);
}


// ----------------------------------------------------------------------------

cyclic_nibble_channel::cyclic_nibble_channel()
{
	//N = 1 << 14;
	//mask = N-1;
	//nibbles.resize(N);
}

bool cyclic_nibble_channel::have(int bitcount)
{
	assert((bitcount % 4) == 0);
	return sz > (bitcount / 4);
}

int maximum_nibble_channel_load = 0;

UL cyclic_nibble_channel::get(int bitcount)
{
	(void)bitcount;
	assert(bitcount == 4);
	assert(sz > 0);
	UC c = nibbles[utp++];
	utp &= mask;
	--sz;
	return c;
}

void cyclic_nibble_channel::put(UL bits, int bitcount)
{
	(void)bitcount;
	assert(bitcount == 4);
	assert(sz < N);
	nibbles[inp++] = bits & 0xff;
	inp &= mask;
	++sz;
	if (sz>maximum_nibble_channel_load)
		maximum_nibble_channel_load = sz;
}

void cyclic_nibble_channel::done()
{
}

int cyclic_nibble_channel::size() const
{
	return sz;
}

UC& cyclic_nibble_channel::operator[](int idx)
{
	assert (idx < size());
	idx = (idx + utp) % N;
	return nibbles[idx];
}

UC cyclic_nibble_channel::operator[](int idx) const
{
	assert (idx < size());
	idx = (idx + utp) % N;
	return nibbles[idx];
}

