
///*
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <string_view>
#include <filesystem>
#include <iomanip>
#include <limits>
#include <memory>
#include <utility>
//*/

//import std;

#include "pop.hpp"

#include "Crypt.hpp"

int usage()
{
	std::cerr << "usage: de [-t target] [-e ext] d|e file[s]\n";
	return -1;
}

std::string pwd;
bool old = true;

extern void getpwd(const char*, bool);

#ifdef _MSC_VER

#include <windows.h> 

void getpwd(const char* msg, bool endl)
{
	if (msg)
		std::cout << msg;

	HANDLE hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode = 0; 

	GetConsoleMode(hStdInput, &mode);

	SetConsoleMode(hStdInput, mode & ~ENABLE_ECHO_INPUT);

	std::getline(std::cin, pwd);

	if (endl)
		std::cout << std::endl;

	SetConsoleMode(hStdInput, mode);
} 

#else

#include <unistd.h>

void getpwd(const char* msg, bool)
{
	char* gp = getpass(msg);
	pwd = gp;
	while (*gp)
	{
		(*gp) = 0;
		++gp;
	}
}

#endif

long long encrypt
(
	std::istream& is,
	std::ostream& os,
	std::size_t rem,
	bool prog,
	//const std::string& str
	std::string_view str
)
{
	Crypt cr{pwd, old};
	pwd.assign('\0', pwd.size());

	auto sz = rem;
	auto i = sz-sz;
	constexpr UL BL = cr.maxblock();
	std::vector<std::byte> buff;
	buff.resize(BL);

	int sh=0, m=1;
	while (true) {
		if (((sz/BL)>>sh) < 400) break;
		++sh;
		m = (m << 1) | 1;
	}

	if (!rem) {
		while (true) {
			is.read((char*)buff.data(), BL);
			auto rd = is.gcount();
			if (rd) {
				cr.encrypt_block((UC*)buff.data(), rd);
				os.write((char*)buff.data(), rd);
			}
			if (rd<BL)
				return cr.nextcount();
		}
	}

	i = 0;
	while (rem > 0)
	{
		if (rem >= BL) {
			is.read((char*)buff.data(), BL);
			cr.encrypt_block((UC*)buff.data(), BL);
			os.write((char*)buff.data(), BL);
			rem -= BL;
			if ((i&m)==0) [[unlikely]]
				if (prog) std::cout << str << " : " << (((sz-rem)*100)/sz) << " %\r" << std::flush;
			++i;
		} else {
			is.read((char*)buff.data(), rem);
			cr.encrypt_block((UC*)buff.data(), (int)rem);
			os.write((char*)buff.data(), rem);
			break;
		}
	}
	if (prog) std::cout << str << "        \n";
	return cr.nextcount();
}

[[nodiscard]] std::string replace_ext(
	std::string_view fn, // const std::string& fn,
	std::string_view ext // const std::string ext
)
{
	std::size_t p2, p1 = fn.find_last_of('/');

	std::string ret;

	if (p1 != std::string::npos) {
		p2 = fn.find_first_of('.', p1);
	} else {
		p2 = fn.find_first_of("."s);		
	}

	if (p2 == std::string::npos)
	{
		ret += fn;
	} else {
		ret += fn.substr(0, p2);
	}

	ret += "."s;
	ret += ext;

	return ret;

}

long long encrypt(const std::string& str, const std::string& target, const std::string& ext)
{	
	bool report = true;
	std::size_t rem = 0;
	std::string nfn;

	pop<std::istream> ifs;
	pop<std::ostream> ofs;

	if (str == "-"s) {
		ifs.borrow(std::cin);
		report = false;
	} else {
		ifs.create<std::ifstream>(str, std::fstream::binary);
		rem = std::filesystem::file_size(str);
	}

	if (target == "-"s) {
		ofs.borrow(std::cout);
		report = false;
	} else {
		if (str == "-"s)
			nfn = target + "/" + "output.decrypt";
		else
			nfn = target + "/" + str + ".encrypt";
		if (!ext.empty())
			nfn = replace_ext(nfn, ext);
		ofs.create<std::ofstream>(nfn, std::fstream::binary);
	}

	if (ifs && ofs)
	{
		return encrypt(*ifs, *ofs, rem, report, str);
	} else {
		std::cerr << "error\n";
		return 0;
	}
}

long long decrypt(std::istream& is, std::ostream& os, std::size_t rem, bool prog, const std::string& str)
{
	Crypt cr{pwd, old};
	pwd.assign('\0', pwd.size());

	auto sz = rem;
	constexpr UL BL = cr.maxblock();
	std::vector<std::byte> buff;
	buff.resize(BL);

	if (!rem) {
		while (true) {
			is.read((char*)buff.data(), BL);
			auto rd = is.gcount();
			if (rd) {
				cr.decrypt_block((UC*)buff.data(), rd);
				os.write((char*)buff.data(), rd);
			}
			if (rd<BL)
				return cr.nextcount();
		}
	}

	int i=0, sh=0, m=1;
	while (true) {
		if (((sz/BL)>>sh) < 400) break;
		++sh;
		m = (m << 1) | 1;
	}

	while (rem > 0)
	{
		if (rem >= BL) {
			is.read((char*)buff.data(), BL);
			cr.decrypt_block((UC*)buff.data(), BL);
			os.write((char*)buff.data(), BL);
			rem -= BL;
			if ((i&m)==0) [[unlikely]]
				if (prog) std::cout << str << " : " << (((sz-rem)*100)/sz) << " %\r" << std::flush;
			++i;
		} else {
			is.read((char*)buff.data(), rem);
			cr.decrypt_block((UC*)buff.data(), (int)rem);
			os.write((char*)buff.data(), rem);
			break;
		}
	}
	if (prog) std::cout << str << "        " << std::endl;
	return cr.nextcount();
}

long long decrypt(const std::string& str, const std::string& target, const std::string ext)
{
	bool report = true;
	std::size_t rem = 0;
	std::string nfn;

	pop<std::istream> ifs;
	pop<std::ostream> ofs;

	if (str == "-"s) {
		ifs.borrow(std::cin);
		report = false;
	} else {
		ifs.create<std::ifstream>(str, std::fstream::binary);
		rem = std::filesystem::file_size(str);
	}

	if (target == "-"s) {
		ofs.borrow(std::cout);
		report = false;
	} else {
		if (str == "-"s)
			nfn = target + "/" + "output.decrypt";
		else
			if (str.ends_with(".encrypt"))
				nfn = target + "/" + str.substr(0, str.length()-8);
			else
				nfn = target + "/" + str + ".decrypt";
		if (!ext.empty())
			nfn = replace_ext(nfn, ext);
		ofs.create<std::ofstream>(nfn, std::fstream::binary);
	}

	if (ifs && ofs)
	{
		return decrypt(*ifs, *ofs, rem, report, str);
	} else {
		std::cerr << "error\n";
		return 0;
	}
}

std::string pritty(long long i, const char* token = "'")
{
	if (!i) return "0"s;
	std::string ss = "";
	bool neg = false;
	if (i<0) { i=-i; neg = true; }
	int a=0;
	while (true)
	{
		if (!i) break;
		if ((a!=0) && ((a%3)==0)) ss = token + ss;
		ss = "0123456789"[i%10] + ss;
		i /= 10;
		++a;
	}
	if (neg) ss = "-" + ss;
	return ss;
}

int main(int argc, char** argv)
{
	using namespace std::literals;
	bool de, have = false, hp = false;
	std::vector<std::string> files;
	std::string target = ".";
	std::string ext;
	for (int i=1; i<argc; ++i) {
		if (argv[i]=="-t"s) {
			target = argv[++i];
		} else if (argv[i]=="-e"s) {
			ext = argv[++i];
		} else if (argv[i]=="-p"s) {
			pwd = argv[++i];
			hp = true;
		} else if (argv[i]=="-o"s) {
			old = true;
		} else if (argv[i]=="-n"s) {
			old = false;
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

	if (!hp) {
		if (target=="-"s)
			getpwd("", false);
		else
			getpwd("Password:", true);
	}

	if (target != "-"s)
		std::cout << "passes " << Crypt{pwd}.passcount() << "\n";

	long long acc = 0;
	for (auto&& f : files) {
		if (de)
			acc += decrypt(f, target, ext);
		else
			acc += encrypt(f, target, ext);
	}

	for (auto& c : pwd) c = 0;

	if (target != "-"s)
		std::cout << "tokens " << pritty(acc) << std::endl;
}



