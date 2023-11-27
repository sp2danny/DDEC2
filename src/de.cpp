
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <string_view>
#include <filesystem>
#include <iomanip>

#include <unistd.h>

#include "Crypt.hpp"

int usage()
{
	std::cerr << "usage: de [-t target] d|e file[s]\n";
	return -1;
}

std::string pwd;

void getpwd()
{
	auto s = getpass("password:");
	pwd = s;
}

void encrypt(const std::string& str, const std::string& target)
{
	std::ifstream ifs{str, std::fstream::binary};
	std::ofstream ofs{target+"/"+str+".encrypt", std::fstream::binary};
	
	Crypt cr{pwd};

	std::cout << "passes " << cr.passcount() << "\n";
	
	auto rem = std::filesystem::file_size(str);
	auto sz = rem;
	auto i = sz-sz;
	const UL BL = cr.maxblock();
	std::vector<std::byte> buff;
	buff.resize(BL);
	
	while (rem > 0)
	{
		if (rem >= BL) {
			ifs.read((char*)buff.data(), BL);
			cr.encrypt_block((UC*)buff.data(), BL);
			ofs.write((char*)buff.data(), BL);
			i += BL;
			rem -= BL;
			std::cout << str << " : " << ((i*100)/sz) << " %\r";
		} else {
			ifs.read((char*)buff.data(), rem);
			cr.encrypt_block((UC*)buff.data(), rem);
			ofs.write((char*)buff.data(), rem);
			break;
		}
	}
	std::cout << str << "        \n";
}

void decrypt(const std::string& str, const std::string& target)
{
	std::ifstream ifs{str, std::fstream::binary};

	std::string nfn;
	if (str.ends_with(".encrypt"))
		nfn = target + "/" + str.substr(0, str.length()-8);
	else
		nfn = target + "/" + str + ".decrypt";

	std::ofstream ofs{nfn, std::fstream::binary};

	Crypt cr{pwd};

	std::cout << "passes " << cr.passcount() << "\n";

	auto rem = std::filesystem::file_size(str);
	auto sz = rem;
	const UL BL = cr.maxblock();
	std::vector<std::byte> buff;
	buff.resize(BL);
	
	int i=0, sh=0, m=1;
	while (true) {
		if (((sz/BL)>>sh) < 400) break;
		++sh;
		m = (m << 1) | 1;
	}

	while (rem > 0)
	{
		if (rem >= BL) {
			ifs.read((char*)buff.data(), BL);
			cr.decrypt_block((UC*)buff.data(), BL);
			ofs.write((char*)buff.data(), BL);
			rem -= BL;
			if ((i&m)==0) [[unlikely]]
				std::cout << str << " : " << (((sz-rem)*100)/sz) << " %\r" << std::flush;
			++i;
		} else {
			ifs.read((char*)buff.data(), rem);
			cr.decrypt_block((UC*)buff.data(), rem);
			ofs.write((char*)buff.data(), rem);
			break;
		}
	}
	std::cout << str << "        " << std::endl;
}

int main(int argc, char** argv)
{
	using namespace std::literals;
	bool de, have = false, hp = false;
	std::vector<std::string> files;
	std::string target = ".";
	for (int i=1; i<argc; ++i) {
		if (argv[i]=="-t"s) {
			target = argv[++i];
		} else if (argv[i]=="-p"s) {
			pwd = argv[++i];
			hp = true;
		} else if (!have) {
			if (argv[i]=="d"s) {
				have = true;
				de = true;
			} else if (argv[i]=="e"s) {
				have = true;
				de = false;
			} else {
				return usage();
			}
		} else {
			files.push_back(argv[i]);
		}
	}
	if (files.empty())
		return usage();
	if (!have)
		return usage();

	if (!hp)
		getpwd();

	for (auto&& f : files)
		(de?decrypt:encrypt)(f, target);
}
