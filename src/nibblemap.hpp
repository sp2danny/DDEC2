
#pragma once

import std;

#include "core.hpp"

struct map4
{
	typedef std::pair<bool, UL> findres;

	std::map<BVec, UL> v2i;
	std::vector<BVec> i2v;

	bool has(const BVec&) const;
	findres find(const BVec&) const;
	bool add(const BVec&);
	bool lookup(UL, BVec&);
	void init(int max);
	int size() const;
};

struct fbct2
{
	typedef std::pair<bool, UL> findres;

	bool has(const BVec&) const;
	findres find(const BVec&) const;
	bool add(const BVec&);
	bool addh(UC c, UL hint);
	bool lookup(UL, BVec&);
	void init(int max);
	int size() const { return std::ssize(all); }

	struct Item;
	struct Item
	{
		Item() = default;
		Item(UL idx, UL par, UC me) : idx(idx), par(par), me(me) {}
		UL idx = 0;
		UL leafs[16] = {(UL)-1,(UL)-1,(UL)-1,(UL)-1,(UL)-1,(UL)-1,(UL)-1,(UL)-1,(UL)-1,(UL)-1,(UL)-1,(UL)-1,(UL)-1,(UL)-1,(UL)-1,(UL)-1};
		UL par = 0;
		UC me;
	};
	Item head{(UL)-1,(UL)-1,0};
	std::vector<Item> all;
};


