
#include <iostream>

#include "util.hpp"

using namespace std::literals;

std::string pwd;

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

[[nodiscard]] std::string replace_ext(std::string_view fn, std::string_view ext)
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

[[nodiscard]] std::string pritty(long long i, const char* token)
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

[[nodiscard]] static bool strmat(const char* pattern, const char* string)
{
	bool ate_p = *pattern == 0;
	bool ate_s = *string == 0;

	if (ate_s && ate_p) return true;
	if (ate_p) return false;
	char pc = *pattern;
	if (ate_s) {
		if (pc == '*')
			return strmat(pattern + 1, string);
		else
			return false;
	}
	switch (pc)
	{
	case '*':
		return strmat(pattern + 1, string) || strmat(pattern, string + 1);
	case '?':
		return strmat(pattern + 1, string + 1);
	default:
		if (pc != *string) return false;
		return strmat(pattern + 1, string + 1);
	}
}

[[nodiscard]] bool strmat(const std::string& pattern, const std::string& string)
{
	return strmat(pattern.c_str(), string.c_str());
}

