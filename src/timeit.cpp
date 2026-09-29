
import std;

#include "Crypt.hpp"

int main()
{
	extern void test();
	test();

	std::string pwd = "123456789abcdefghij";
	Crypt cr{ pwd, true };
	for (auto& c : pwd) c = 0;

	std::array<unsigned char, 65536> block;

	auto t1 = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < 100; ++i)
	{
		std::memset(block.data(), 65536, 1);
		cr.decrypt(block.data(), 65536);
	}

	auto t2 = std::chrono::high_resolution_clock::now();

	auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

	std::println("{}", dur / 100.0);

}
