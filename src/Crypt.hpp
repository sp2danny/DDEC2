
#pragma once

#include <random>
#include <vector>
#include <string>
#include <string_view>
#include <iosfwd>

#include "core.hpp"
#include "bitstream.hpp"
#include "mersenne-twister.hpp"

// typedef std::mt19937 Eng;

typedef MT Eng;

struct Operator
{
	enum { op_none=0, op_xor, op_swap } op = op_none;
	UL par1=0, par2=0;
	void execute(UC* block, int size);
};

struct Crypt
{
	Crypt(std::string, bool=true);
	Crypt(const Crypt&) = default;
	Crypt(Crypt&&) = default;
	
	~Crypt() { clear(); }

	static constexpr int maxblock() { return MXBLK; }

	void encrypt_block  (UC* block, int size);
	void decrypt_block  (UC* block, int size);

	void encrypt (UC* block, int size);
	void decrypt (UC* block, int size);

	int passcount() const { return std::ssize(keys); }
	long long nextcount() const { return ncnt; }
	
	void clear();

private:
	static const int MXBLK = 1<<16;
	static const int MASK = MXBLK-1;

private:
	UL next();
	UC next8();
	UL next16(), next32();
	int have = 0;
	UL bits;
	bool old;
	long long ncnt = 0;

	void loadup_big(int size, UC* block = nullptr);
	void loadup_scramble(int size, UC* block = nullptr);
	void loadup_xorpass(int size, UC* block = nullptr);
	//void loadup_mixpass(int size, UC* block = nullptr);
	void execute_loadup(UC* block, int size);

private:
	std::vector<Eng> keys;
	int kidx;
	std::vector<Operator> opers;
};

struct encrypt_target : bittarget
{
	encrypt_target(std::string_view key, std::ostream& out);

	virtual void put(UL bits, int bitcount) override;
	virtual void done() override;

	int passcount() const { return cr.passcount(); }

private:
	std::vector<UC> block;
	UL bsf = 0;
	int cnt = 0;
	Crypt cr;
	std::ostream& out;
};

struct decrypt_source : bitsource
{
	decrypt_source(std::string_view key, std::istream& in);

	virtual bool have(int bitcount) override;
	virtual UL get(int bitcount) override;
	void make(int bitcount);

	static const int maxblock = Crypt::maxblock();

private:
	std::istream& in;
private:
	std::array<UC, maxblock> block;
	UL pos = 0, blsz = 0;
private:
	UL bsf = 0;
	UC cnt = 0;
	Crypt cr;
};


