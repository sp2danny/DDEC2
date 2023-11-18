
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <unistd.h>

int usage()
{
	std::cerr << "usage: ce [c|e] file[s]\n";
	return -1;
}

std::string pwd;

void getpwd()
{
	auto s = getpass("password:");
	pwd = s;
}

void code(const std::string& str)
{
	std::ifstream ifs{str};
}

void encode(const std::string& str)
{
	std::ifstream ifs{str};
	
}

int main(int argc, char** argv)
{
	std::vector<std::string> arg;
	for (int i=1; i<argc; ++i)
		arg.push_back(argv[i]);
	if (arg.empty())
		return usage();
	--argc;
	if (arg[0] == "c") {
		getpwd();
		for (int i=1; i<argc; ++i)
			code(arg[i]);
	} else if (arg[0] == "e") {
		getpwd();
		for (int i=1; i<argc; ++i)
			encode(arg[i]);
	} else
		return usage();
}

