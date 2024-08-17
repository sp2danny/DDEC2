
#include "mersenne-twister.hpp"

// #define MT_UNROLL_MORE

#define M32(x) (0x80000000 & x) // 32nd MSB
#define L31(x) (0x7FFFFFFF & x) // 31 LSBs

#define UNROLL(expr) \
	y = M32(state.MT[i]) | L31(state.MT[i+1]); \
	state.MT[i] = state.MT[expr] ^ (y >> 1) ^ (((std::int32_t(y) << 31) >> 31) & MAGIC); \
	++i;

void MT::generate_numbers()
{
	std::size_t i = 0;
	std::uint32_t y;

	while ( i < DIFF ) {
		UNROLL(i+PERIOD);

#ifdef MT_UNROLL_MORE
		UNROLL(i+PERIOD);
#endif
	}

	while ( i < SIZE -1 ) {
		UNROLL(i-DIFF);

#ifdef MT_UNROLL_MORE
		UNROLL(i-DIFF);
		UNROLL(i-DIFF);
		UNROLL(i-DIFF);
		UNROLL(i-DIFF);
		UNROLL(i-DIFF);
		UNROLL(i-DIFF);
		UNROLL(i-DIFF);
		UNROLL(i-DIFF);
		UNROLL(i-DIFF);
		UNROLL(i-DIFF);
#endif
	}

	{
		// i = 623, last step rolls over
		y = M32(state.MT[SIZE-1]) | L31(state.MT[0]);
		state.MT[SIZE-1] = state.MT[PERIOD-1] ^ (y >> 1) ^ (((int32_t(y) << 31) >> 31) & MAGIC);
	}

	// Temper all numbers in a batch
	for (size_t i = 0; i < SIZE; ++i) {
		y = state.MT[i];
		y ^= y >> 11;
		y ^= y << 7  & 0x9d2c5680;
		y ^= y << 15 & 0xefc60000;
		y ^= y >> 18;
		state.MT_TEMPERED[i] = y;
	}

	state.index = 0;
}

void MT::seed(std::uint32_t value)
{
	state.MT[0] = value;
	state.index = SIZE;

	for ( std::uint_fast32_t i=1; i<SIZE; ++i )
		state.MT[i] = 0x6c078965*(state.MT[i-1] ^ state.MT[i-1]>>30) + i;
}

std::uint32_t MT::rand_u32()
{
	if ( state.index == SIZE ) {
		generate_numbers();
		state.index = 0;
	}

	return state.MT_TEMPERED[state.index++];
}
