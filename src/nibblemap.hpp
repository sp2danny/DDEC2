
#pragma once

#include "core.hpp"

#include <utility>
#include <vector>
#include <list>
#include <map>

/*
struct nibblemap
{
	

	virtual bool has(const BVec&) const abstract;
	virtual findres find(const BVec&) const abstract;
	virtual bool add(const BVec&) abstract;
	virtual bool lookup(UL, BVec&) abstract;
	virtual void init(UL max) abstract;
	virtual UL size() const abstract;
};
*/

struct map4
{
	typedef std::pair<bool, UL> findres;

	std::map<BVec, UL> v2i;
	std::vector<BVec> i2v;

	bool has(const BVec&) const;
	findres find(const BVec&) const;
	bool add(const BVec&);
	bool lookup(UL, BVec&);
	void init(UL max);
	UL size() const;
};


struct fbct2
{
	typedef std::pair<bool, UL> findres;

	bool has(const BVec&) const;
	findres find(const BVec&) const;
	bool add(const BVec&);
	bool addh(UC c, UL hint);
	bool lookup(UL, BVec&);
	void init(UL max);
	UL size() const { return (UL)all.size(); }

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

/*
struct simple_decompress
{
	bool add(const BVec& bv) { all.push_back(bv); }
	bool lookup(UL idx, BVec& bv) { if(idx>=size()) return false; bv=all[idx]; return true; }
	void init(UL max) { all.reserve(max); }
	UL size() const { return (UL)all.size(); }
private:
	std::vector< BVec > all;
};
*/

