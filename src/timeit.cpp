
import std;


#include "Crypt.hpp"

int main()
{

	std::string pwd = "1234567890123456789";
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

	auto dur = std::chrono::duration_cast<std::chrono::milliseconds>((t2 - t1)/100);


    std::cout << dur << std::endl;
}
