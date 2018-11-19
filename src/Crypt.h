
#pragma once

#include <random>
#include <vector>
#include <string>
#include <iosfwd>

#include "core.hpp"
#include "bitstream.hpp"

typedef std::mt19937 Eng;

struct Operator
{
	enum { op_none=0, op_xor, op_swap } op = op_none;
	UL par1=0, par2=0;
	void execute(UC* block, UL size);
};

struct Crypt
{
	Crypt(std::string);

	static constexpr UL maxblock() { return 65536; }

	void encrypt_block  (UC* block, UL size);
	void decrypt_block  (UC* block, UL size);

	void encrypt (UC* block, UL size);
	void decrypt (UC* block, UL size);

private:
	UL next();

	void loadup_big(UL size, UC* block = nullptr);
	void loadup_scramble(UL size, UC* block = nullptr);
	void loadup_xorpass(UL size, UC* block = nullptr);
	void loadup_mixpass(UL size, UC* block = nullptr);
	void execute_loadup(UC* block, UL size);

private:
	std::vector<Eng> keys;
	UL kidx;
	std::vector<Operator> opers;
};

struct encrypt_target : bittarget
{
	encrypt_target(const std::string& key, std::ostream& out);

	virtual void put(UL bits, UC bitcount) override;
	virtual void done() override;

private:
	std::vector<UC> block;
	UL bsf = 0;
	UC cnt = 0;
	Crypt cr;
	std::ostream& out;
};

struct decrypt_source : bitsource
{
	decrypt_source(const std::string& key, std::istream& in);

	/*virtual*/ bool have(UC bitcount) override;
	/*virtual*/ UL get(UC bitcount) override;
	void make(UC bitcount);

	static const UL maxblock = Crypt::maxblock();

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


