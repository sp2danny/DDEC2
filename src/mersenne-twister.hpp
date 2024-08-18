
#pragma once

#include <cstdint>
#include <cstddef>

class MT
{
public:
	std::uint32_t rand_u32();

	void seed(std::uint32_t seed_value);

	std::uint32_t operator()() { return rand_u32(); }

	static const std::size_t SIZE = 624;

private:

	void generate_numbers();


	struct MTState {
	  std::uint32_t MT[SIZE];
	  std::uint32_t MT_TEMPERED[SIZE];
	  std::size_t index = SIZE;
	};

	MTState state;
};

