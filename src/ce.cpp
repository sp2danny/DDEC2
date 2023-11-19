
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <string_view>
#include <filesystem>

#include <unistd.h>

#include "Crypt.hpp"

int usage()
{
	std::cerr << "usage: ce [e|d] file[s]\n";
	return -1;
}

std::string pwd;

void getpwd()
{
	auto s = getpass("password:");
	pwd = s;
}

void encrypt(const std::string& str)
{
	std::ifstream ifs{str, std::fstream::binary};
	std::ofstream ofs{str+".encrypt", std::fstream::binary};
	encrypt_target et{pwd, ofs};
	std::cout << "passes " << et.passcount() << "\n";
	
	auto sz = std::filesystem::file_size(str);
	auto i = sz-sz;
	
	while (true)
	{
		++i;
		UL b;
		ifs.read((char*)&b, 1);
		if (!ifs) break;
		et.put(b, 8);
		if ((i%1024)==0) {
			std::cout << str << " : " << ((i*100)/sz) << " %\r";
		}
	}
	et.done();
	std::cout << str << "        \n";

}

void decrypt(const std::string& str)
{
	std::ifstream ifs{str, std::fstream::binary};
	std::ofstream ofs{str+".decrypt", std::fstream::binary};
	decrypt_source ds{pwd, ifs};
	
	auto sz = std::filesystem::file_size(str);
	auto i = sz-sz;

	while (true)
	{
		++i;
		ds.make(8);
		if (!ds.have(8)) break;
		UL b = ds.get(8);
		ofs.write((char*)&b, 1);
		if ((i%1024)==0) {
			std::cout << str << " : " << ((i*100)/sz) << " %\r";
		}
	}
	std::cout << str << "        \n";
}

int main(int argc, char** argv)
{
	std::vector<std::string> arg;
	for (int i=1; i<argc; ++i)
		arg.push_back(argv[i]);
	if (arg.empty())
		return usage();
	--argc;
	if (arg[0] == "e") {
		getpwd();
		for (int i=1; i<argc; ++i)
			encrypt(arg[i]);
	} else if (arg[0] == "d") {
		getpwd();
		for (int i=1; i<argc; ++i)
			decrypt(arg[i]);
	} else
		return usage();
}

