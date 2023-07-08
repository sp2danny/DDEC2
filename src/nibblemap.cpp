
#include <cassert>

#include "nibblemap.hpp"

bool map4::has(const BVec& bv) const
{
	return v2i.count(bv) > 0;
}

auto map4::find(const BVec& bv) const -> findres
{
	auto itr = v2i.find(bv);
	if (itr == v2i.end())
	{
		return {false, (UL)-1};
	}
	else
	{
		return {true, itr->second};
	}
}

bool map4::add(const BVec& bv)
{
	UL nxt = size();
	auto res = v2i.emplace(bv, nxt);
	if (!res.second)
	{
		return false;
	}
	i2v.push_back(bv);
	return true;
}

bool map4::lookup(UL idx, BVec& bv)
{
	if (idx >= (UL)size())
		return false;
	bv = i2v[idx];
	assert(v2i[bv]==idx);
	return true;
}

void map4::init(int max)
{
	i2v.clear();
	v2i.clear();
	i2v.reserve(max);
}

int map4::size() const
{
	assert( i2v.size() == v2i.size() );
	return v2i.size();
}



