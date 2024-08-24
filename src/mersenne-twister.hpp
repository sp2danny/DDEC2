
#pragma once

#include <cstdint>

class MT
{
public:
	typedef std::uint32_t u32;

	u32 rand();

	void seed(u32);

	u32 operator()() { return rand(); }

	static const u32 SIZE = 624;

private:

	void generate();

	struct State {
		u32 RAW[SIZE] = {0};
		u32 TMP[SIZE] = {0};
		u32 at = SIZE;
	};

	State state;
};

