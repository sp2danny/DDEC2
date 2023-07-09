
#include <algorithm>
#include <fstream>
#include <chrono>
#include <cassert>
#include <iostream>

#include "Crypt.hpp"

#include "core.hpp"
#include "perftimer.hpp"

static UL make_4b(const char* str)
{
	UL k = 0;
	k |= UL(UC(str[0])) <<  0;
	k |= UL(UC(str[1])) <<  8;
	k |= UL(UC(str[2])) << 16;
	k |= UL(UC(str[3])) << 24;
	return k;
}

void Operator::execute(UC* block, [[maybe_unused]] int size)
{
	switch (op)
	{
	case op_swap:
		assert(par1 < (UL)size);
		assert(par2 < (UL)size);
		std::swap(block[par1], block[par2]);
		break;
	case op_xor:
		assert(par1 < (UL)size);
		assert(par2 < 256);
		block[par1] ^= par2;
		break;
	default:
		assert(false && "Unknown Operand");
		break;
	}
}

Crypt::Crypt(std::string key)
{
	while (key.length()%4)
		key += ' ';

	auto s = key.c_str();

	auto n = key.length();
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
	kidx = (kidx + 1) % keys.size();
	return ret;
}

void Crypt::loadup_big(int size, UC* block)
{
	assert(size <= maxblock());
	UL n = (UL)keys.size();
	for (UL ks = 0; ks < n; ++ks)
	{
		kidx = ks;
		loadup_scramble(size, block);
		loadup_xorpass(size, block);
	}
}

void Crypt::loadup_scramble(int size, UC* block)
{
	assert(size <= maxblock());

	if (block && (size == 65536))
	{
		for (int i = 0; i < 65536; ++i)
		{
			UL k = next() & 0xffff;
			std::swap(block[i], block[k]);
		}
		return;
	}

	for (int i = 0; i < size; ++i)
	{
		UL k;
		if (size==65536)
			k = next() & 0xffff;
		else
			k = next() % size;
		Operator op{Operator::op_swap, (UL)i, k};
		if (block)
			op.execute(block, size);
		else
			opers.push_back(op);
	}
}

void Crypt::loadup_xorpass(int size, UC* block)
{
	assert(size <= maxblock());

	if (block)
	{
		for (int i = 0; i < size; ++i)
		{
			UL k = next() & 0xff;
			block[i] ^= k;
		}
		return;
	}

	for (int i = 0; i < size; ++i)
	{
		UC k = next() % 256;
		Operator op{Operator::op_xor, (UL)i, k};
		if (block)
			op.execute(block, size);
		else
			opers.push_back(op);
	}
}

void Crypt::loadup_mixpass(int size, UC* block)
{
	assert(size <= maxblock());
	for (int i = 0; i < size; ++i)
	{
		UL nxt = next();
		UL idx = (nxt/256) % size;
		UC k = nxt % 256;
		Operator op{Operator::op_xor, idx, k};
		if (block)
			op.execute(block, size);
		else
			opers.push_back(op);
	}
}

void Crypt::execute_loadup(UC* block, int size)
{
	assert(size <= maxblock());
	for (auto op : opers)
		op.execute(block, size);
}

void Crypt::encrypt_block(UC* block, int size)
{
	assert(size <= maxblock());
	opers.clear();
	loadup_big(size);
	std::reverse(opers.begin(), opers.end());
	execute_loadup(block, size);
	opers.assign( opers.size(), Operator{} );
}

void Crypt::decrypt_block(UC* block, int size)
{
	assert(size <= maxblock());
	loadup_big(size, block);
}

void Crypt::encrypt(UC* block, int size)
{
	int max = maxblock();
	while (size)
	{
		if (size > max)
		{
			encrypt_block(block, max);
			size -= max;
			block += max;
		} else {
			encrypt_block(block, size);
			break;
		}
	}
}

void Crypt::decrypt(UC* block, int size)
{
	int max = maxblock();
	while (size)
	{
		if (size > max)
		{
			decrypt_block(block, max);
			size -= max;
			block += max;
		}
		else {
			decrypt_block(block, size);
			break;
		}
	}
}

encrypt_target::encrypt_target(std::string_view key, std::ostream& out)
	: cr(std::string{key})
	, out(out)
{
	block.reserve(cr.maxblock());
}

void encrypt_target::put(UL bits, int bitcount)
{
	assert( (cnt+bitcount) <= ((int)sizeof(UL)*8) );

	bsf <<= bitcount;
	bsf |= bits;
	cnt += bitcount;

	while (cnt >= 8)
	{
		UC c = (bsf >> (cnt-8)) & 0xff;
		block.push_back(c);
		cnt -= 8;
		bsf &= ((1<<cnt)-1);

		UL max = cr.maxblock();
		if (block.size() >= cr.maxblock())
		{
			assert(block.size() == cr.maxblock());
			cr.encrypt_block(block.data(), max);
			out.write( (char*)block.data(), max );
			out.flush();
			block.clear();
		}
	}
}

void encrypt_target::done()
{
	while (cnt)
		put(0, 1);
	int sz = block.size();
	if (!sz) return;
	cr.encrypt_block(block.data(), sz);
	out.write((char*)block.data(), sz);
	out.flush();
}

// ----------------------------------------------------------------------------

decrypt_source::decrypt_source(std::string_view key, std::istream& in)
	: in(in)
	, cr(std::string{key})
{
	blsz = 0;
	pos = 0;
	bsf = 0;
	cnt = 0;
}

bool decrypt_source::have(int bitcount)
{
	make(bitcount);
	if (cnt < bitcount)
	{
		std::cerr << "did not have " << (int)bitcount << " bits" << std::endl;
	}
	return cnt >= bitcount;
}

UL decrypt_source::get(int bitcount)
{
	assert(cnt >= bitcount);
	UL ret = bsf >> (cnt - bitcount);
	cnt -= bitcount;
	bsf &= ((1 << cnt) - 1);
	return ret;
}

void decrypt_source::make(int bitcount)
{
	TP t1, t2;
	while (true)
	{
		if (cnt >= bitcount) return;

		if (pos >= blsz)
		{
			if (pt)
				t1 = hrc::now();
			UL max = cr.maxblock();
			if (pt) {
				t2 = hrc::now();
				pt->decrypt += Dur(t2 - t1);
				t1 = t2;
			}
			UL n = in.read((char*)block.data(), max) ? max : (UL)in.gcount();
			if (pt) {
				t2 = hrc::now();
				pt->streamin += Dur(t2 - t1);
				t1 = t2;
			}
			if (!n)
			{
				std::cerr << "could not read from in\n";
				return;
			}
			blsz = n;
			cr.decrypt_block(block.data(), n);
			pos = 0;
			if (pt)
				pt->decrypt += Dur(hrc::now() - t1);
		}

		if (pt)
			t1 = hrc::now();
		UC c = block[pos++];
		bsf <<= 8;
		bsf |= c;
		cnt += 8;
		if (pt)
			pt->decrypt += Dur(hrc::now() - t1);

	}
}

