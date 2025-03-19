
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <ranges>
#include <initializer_list>
#include <utility>
#include <cstddef>

using ssize_t = long;

using namespace std::literals;

namespace fs = std::filesystem;

bool strmat(const char* str, const char* pat)
{
	char str_c = *str;
	char pat_c = *pat;

	bool ate_str = (str_c == 0);
	bool ate_pat = (pat_c == 0);

	if (!ate_pat) {
		if (pat_c=='*') {
			if (ate_str)
				return strmat(str, pat+1);
			else
				return strmat(str+1, pat) || strmat(str, pat+1);
		}
	}

	if (ate_str && ate_pat) return true;
	if (ate_str || ate_pat) return false;

	if (pat_c != '?')
		if (str_c != pat_c) return false;

	return strmat(str+1, pat+1);
}

using UC = unsigned char;
using fpt = auto (*)(UC, UC) -> UC;
using UCV = std::vector<UC>;

fpt get_f(const std::string& oper)
{
	fpt fp = nullptr;

	if (oper == "xor")  fp = +[](UC c1, UC c2) -> UC { return c1 ^ c2; };
	if (oper == "or")   fp = +[](UC c1, UC c2) -> UC { return c1 | c2; };
	if (oper == "and")  fp = +[](UC c1, UC c2) -> UC { return c1 & c2; };
	if (oper == "nand") fp = +[](UC c1, UC c2) -> UC { return  ~(c1 & c2); };
	if (oper == "rotr") fp = +[](UC c1, UC c2) -> UC { return (c1+c2)%256; };
	if (oper == "rotl") fp = +[](UC c1, UC c2) -> UC { return (c1+256-c2)%256; };
	
	return fp;
}

void do_stuff(std::istream& in, std::ostream& out, bool loop, const UCV& data, const std::string& oper)
{
	fpt fp = get_f(oper);
	
	std::vector<unsigned char> buff;
	int sz = (int)std::ssize(data);
	buff.resize(sz);
	
	auto pass_apply = [&]() -> bool {
		in.read((char*)buff.data(), sz);
		auto len = in.gcount();
		for (int i=0; i<len; ++i)
		{
			buff[i] = fp(buff[i], data[i]);
		}
		out.write((char*)buff.data(), len);
		return (len<sz);
	};

	auto pass_copy = [&]() -> bool {
		in.read((char*)buff.data(), sz);
		auto len = in.gcount();
		out.write((char*)buff.data(), len);
		return (len<sz);
	};

	bool first=true, done;
	
	while (true)
	{
		if (first || loop)
			done = pass_apply();
		else
			done = pass_copy();
			
		if (done) break;
		
		first = false;
	}
	
}

void do_stuff(const std::filesystem::directory_entry& de, const std::vector<unsigned char>& data, const std::string& oper)
{
	if (oper == "app") {
		std::fstream fs(de.path(), std::ios_base::out | std::ios_base::binary | std::ios_base::app );
		fs.write((char*)data.data(), data.size());
		return;
	}

	fpt fp = get_f(oper);

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

std::ostream& breakline(std::ostream& stream)
{
	stream << "\n";
	stream << "----------------------------------------------";
	stream << "\n";
	return stream;
}

int main(int argc, char** argv)
{
	if ((argc==2) && (argv[1]=="--help"s))
	{
		std::cout << "operands : oper, mask, dir, source, size" << std::endl;
		std::cout << "options : stream, loop" << std::endl;
		return 0;
	}

	std::string oper, mask, dir, source, size;
	bool streamed = false;
	bool looping = false;

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
		} else if (argv[i] == "-stream"s) {
			streamed = true;
		} else if (argv[i] == "-loop"s) {
			looping = true;
		}
	}
	
	if ((oper=="rnd") && !dir.empty() && !size.empty())
	{
		extern void dorand(const std::string&, ssize_t );
		dorand(dir, std::stol(size));
		return 0;
	}

	{
		auto is_empty = [](const std::string& str) { return str.empty(); };
		std::vector<std::reference_wrapper<const std::string>> all = {oper, source, size, mask, dir};
		if (streamed) { all.pop_back(); all.pop_back(); }
		if (std::ranges::any_of(all, is_empty)) {
			std::cerr << "error" << std::endl;
			return 1;
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
	
	if (streamed)
	{
		//void do_stuff(std::istream& in, std::ostream& out, bool loop, const UCV& data, const std::string& oper)
		do_stuff(std::cin, std::cout, looping, data, oper);
		return 0;
	}

	auto di = std::filesystem::directory_iterator{dir};
	int cnt = 0;
	for (auto const& de : di)
	{
		if (!de.is_regular_file()) continue;
		auto fn = de.path().filename().string();
		if (!strmat(fn.c_str(), mask.c_str())) continue;
		do_stuff(de, data, oper);
		++cnt;
	}

	std::cout << cnt << " files affected" << breakline;
}

// 3897173


#include <sstream>
#include <iomanip>

void dorand(const std::string& dir, ssize_t sz)
{

	fs::path p = dir;
	for (ssize_t i=1; i<=sz; ++i)
	{
		std::stringstream ss;
		ss << std::setw(10) << std::setfill('0') << i;
		auto fn = ss.str() + ".tmp";
		std::ofstream ofs{p / fn};
		for (int j=0; j<1'000'000; ++j)
			ofs << (char)rand();
	}
}
