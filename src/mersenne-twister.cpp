
#include "mersenne-twister.hpp"

static const MT::u32 SIZE   = MT::SIZE; // 624
static const MT::u32 PERIOD = 397;
static const MT::u32 DIFF   = SIZE - PERIOD; // 227
static const MT::u32 MAGIC  = 0x9908b0df;

#define M32(x) (0x80000000 & x)
#define L31(x) (0x7FFFFFFF & x)

#define STEP(a,b,expr) \
	y = M32(state.RAW[a]) | L31(state.RAW[b]); \
	state.RAW[i] = state.RAW[expr] ^ (y >> 1) ^ (((std::int32_t(y) << 31) >> 31) & MAGIC)

#define TEMPER(i) \
	y = state.RAW[i]; \
	y ^= y >> 11; \
	y ^= y << 7  & 0x9d2c5680; \
	y ^= y << 15 & 0xefc60000; \
	y ^= y >> 18; \
	state.TMP[i] = y

void MT::generate()
{
	u32 i = 0, y;

	while (i < DIFF) {
		STEP(i, i+1, i+PERIOD);
		++i;
	}

	while (i < SIZE -1) {
		STEP(i, i+1, i-DIFF);
		++i;
	}

	STEP(SIZE-1, 0, PERIOD-1);

	for (i = 0; i < SIZE; ++i) {
		TEMPER(i);
	}
}

void MT::seed(u32 value)
{
	state.RAW[0] = value;
	state.at = SIZE;

	for (u32 i=1; i<SIZE; ++i)
		state.RAW[i] = 0x6c078965*(state.RAW[i-1] ^ state.RAW[i-1]>>30) + i;
}

auto MT::rand() -> u32
{
	if (state.at == SIZE) {
		generate();
		state.at = 0;
	}

	return state.TMP[state.at++];
}

