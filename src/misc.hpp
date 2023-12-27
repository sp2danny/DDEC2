
#pragma once

struct Reporter
{
	Reporter() { ++cnt; std::cout << "default constructed\n"; }
	Reporter(const Reporter&) { ++cnt; std::cout << "copy constructed"; }
	Reporter(Reporter&&) { ++cnt; std::cout << "move constructed\n"; }
	Reporter& operator=(const Reporter&) { std::cout << "copy assigned\n"; return *this; }
	Reporter& operator=(Reporter&&) { std::cout << "move assigned\n"; return *this; }
	~Reporter() { --cnt; std::cout << "destructed\n"; }
	static int Count() { return cnt; }
private:
	static inline int cnt = 0;
};

struct Int : Reporter
{
	Int(int i=0) : i(i) {}
	operator int&() { return i; }
	operator int() const { return i; }
private:
	int i;
};
