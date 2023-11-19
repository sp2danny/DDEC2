
#pragma once

#include <random>
#include <vector>
#include <string>
#include <string_view>
#include <iosfwd>

#include "core.hpp"
#include "bitstream.hpp"

typedef std::mt19937 Eng;

struct Operator
{
	enum { op_none=0, op_xor, op_swap } op = op_none;
	UL par1=0, par2=0;
	void execute(UC* block, int size);
};

struct Crypt
{
	Crypt(std::string);

	static constexpr int maxblock() { return 65536; }

	void encrypt_block  (UC* block, int size);
	void decrypt_block  (UC* block, int size);

	void encrypt (UC* block, int size);
	void decrypt (UC* block, int size);
	
	Crypt(const Crypt&) = delete;
	Crypt(Crypt&&) = delete;
	
	int passcount() const { return std::ssize(keys); }

private:
	UL next();

	void loadup_big(int size, UC* block = nullptr);
	void loadup_scramble(int size, UC* block = nullptr);
	void loadup_xorpass(int size, UC* block = nullptr);
	void loadup_mixpass(int size, UC* block = nullptr);
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


