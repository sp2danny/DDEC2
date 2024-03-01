
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

using namespace std::literals;

bool strmat(const char* str, const char* pat)
{
	char str_c = *str;
	char pat_c = *pat;

	bool ate_str = (str_c == 0);
	bool ate_pat = (pat_c == 0);
	
	if (ate_str && ate_pat) return true;
	if (ate_str || ate_pat) return false;
	
	switch (pat_c)
	{
		case '?':
			return strmat(str+1, pat+1);
		case '*':
			if (pat[1] == 0) return true;
			return strmat(str+1, pat) || strmat(str, pat+1);
		default:
			if (str_c != pat_c) return false;
			return strmat(str+1, pat+1);
	}
	
}

void do_stuff(const std::filesystem::directory_entry& de, const std::vector<unsigned char>& data, const std::string& oper)
{
	using fpt = auto (*)(unsigned char, unsigned char) -> unsigned char;
	fpt fp;

	if (oper == "xor")  fp = +[](unsigned char c1, unsigned char c2) -> unsigned char { return c1 ^ c2; };
	if (oper == "or")   fp = +[](unsigned char c1, unsigned char c2) -> unsigned char { return c1 | c2; };
	if (oper == "and")  fp = +[](unsigned char c1, unsigned char c2) -> unsigned char { return c1 & c2; };

	std::fstream fs(de.path(), std::ios_base::in | std::ios_base::out | std::ios_base::binary);
	std::vector<unsigned char> buff;
	buff.resize(data.size());
	fs.read((char*)buff.data(), data.size());
	for (int i=0; i<std::ssize(data); ++i)
	{
		buff[i] = fp(buff[i], data[i]);
	}
	fs.seekp(0);
	fs.write((char*)buff.data(), data.size());
}

int main(int argc, char** argv)
{

	std::string oper, mask, dir, source, size;
	
	for (int i=1; i<argc; ++i) {
		/****/ if (argv[i] == "-oper"s) {
			if (++i<argc)
				oper = argv[i];
		} else if (argv[i] == "-mask"s) {
			if (++i<argc)
				mask = argv[i];
		} else if (argv[i] == "-dir"s) {
			if (++i<argc)
				dir = argv[i];
		} else if (argv[i] == "-source"s) {
			if (++i<argc)
				source = argv[i];
		} else if (argv[i] == "-size"s) {
			if (++i<argc)
				size = argv[i];
		}
	}
	
	long long sz = 0, mul = 1;
	for (auto c : size) {
		switch (c) {
			case 'k':
				mul *= 1024;
				break;
			case 'M':
				mul *= 1024*1024;
				break;
			case 'G':
				mul *= 1024*1024*1024;
				break;
			default:
				sz = (sz*10) + (c-'0');
		}
	}
	sz *= mul;
	
	std::vector<unsigned char> data;
	{
		std::ifstream ifs{source, std::ios_base::binary};
		data.resize(sz);
		ifs.read((char*)data.data(), sz);
	}
	
	auto di = std::filesystem::directory_iterator{dir};
	for (auto const& de : di)
	{
		if (!de.is_regular_file()) continue;
		auto fn = de.path().filename().c_str();
		if (!strmat(fn, mask.c_str())) continue;
		do_stuff(de, data, oper);
	}
	
}