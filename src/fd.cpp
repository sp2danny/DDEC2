
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

#include "fod.hpp"
#include "util.hpp"

using namespace std::literals;

int usage()
{
	std::cerr << "usage: fd [-t target] [-e ext] file[s]\n";
	return -1;
}

void decrypt(Crypt cr, const std::string& str, const std::string& target, const std::string ext)
{
	std::size_t sz;
	std::string nfn;

	std::ifstream ifs(str, std::fstream::binary);
	sz = std::filesystem::file_size(str);

	if (str.ends_with(".encrypt"))
		nfn = target + "/" + str.substr(0, str.length()-8);
	else
		nfn = target + "/" + str + ".decrypt";
	if (!ext.empty())
		nfn = replace_ext(nfn, ext);
	std::ofstream ofs(nfn, std::fstream::binary);
	
	BVec data;
	data.resize(sz);

	if (ifs && ofs)
	{
		ifs.read((char*)data.data(), sz);
	} else {
		std::cerr << "error\n";
		return;
	}
	
	cr.decrypt(data.data(), std::ssize(data));
	
	ofs.write((char*)data.data(), sz);
}

int main(int argc, char** argv)
{
	using namespace std::literals;

	std::vector<std::string> files;
	std::string target = ".";
	std::string ext;
	bool havepwd = false;

	if ((argc==2) && (argv[1]=="--help"s))
		return usage();

	for (int i=1; i<argc; ++i) {
		if (argv[i]=="-t"s) {
			target = argv[++i];
		} else if (argv[i]=="-p"s) {
			pwd = argv[++i];
			havepwd = true;
		} else if (argv[i]=="-e"s) {
			ext = argv[++i];
		} else {
			files.push_back(argv[i]);
		}
	}
	if (files.empty())
		return usage();

	if (!havepwd)
		getpwd("Password:", true);

	Crypt cr{pwd};
	for (auto& c : pwd) c = 0;

	int i=0, n=std::ssize(files);
	
	for (auto&& f : files) {
		std::cout << ++i << "/" << n << " : " << f << "          \r" << std::flush;
		decrypt(cr, f, target, ext);
	}

}
