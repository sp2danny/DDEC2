
#include "stdafx.h"

#include <cassert>
#include <algorithm>

#include "nibblemap.hpp"
#include "core.hpp"

// ----------------------------------------------------------------------------

bool fbct2::has(const BVec& bv) const
{
	const Item* p = &head;
	assert(p);
	for (auto c : bv)
	{
		assert (c < 16);
		UL idx = p->leafs[c];
		if (idx==(UL)-1) return false;
		p = &all[idx];
	}
	return true;
}

auto fbct2::find(const BVec& bv) const -> findres
{
	const Item* p = &head;
	assert(p);
	UL idx;
	for (auto c : bv)
	{
		assert (c < 16);
		idx = p->leafs[c];
		if (idx==(UL)-1) return {false,0};
		p = &all[idx];
	}
	return {true, p->idx};
}

bool fbct2::addh(UC c, UL hint)
{
	Item* p = &head;
	p = &all[hint];
	if (p->leafs[c] != (UL)-1) return false;
	UL idx = (UL)all.size();
	all.emplace_back(idx, p->idx, c);
	p->leafs[c] = idx;
	return true;
}


bool fbct2::add(const BVec& bv)
{
	UL i, n = (UL)bv.size();
	Item* p = &head;
	assert(p);
	for (i = 0; i < (n - 1); ++i)
	{
		UC c = bv[i];
		assert (c < 16);
		UL idx = p->leafs[c];
		if (idx==(UL)-1) return false; // up to last should exist
		p = &all[idx];
	}
	UC c = bv.back();
	assert (c < 16);
	if (p->leafs[c]!=(UL)-1) return false; // should not already be there
	UL idx = (UL)all.size();
	all.emplace_back(idx, p->idx, c);
	p->leafs[c] = idx;
	return true;
}

void fbct2::init(UL max)
{
	all.clear();
	if (max>250'000)
		all.reserve(250'000);
	else
		all.reserve(max);
	for (UC c = 0; c < 16; ++c)
	{
		all.emplace_back(c, (UL)-1, c);
		head.leafs[c] = c;
	}
}

UL longest_sequence = 0;

bool fbct2::lookup(UL idx, BVec& bv)
{
	if (idx >= all.size()) return false;
	UL p = idx;
	bv.clear();
	while (p!=(UL)-1)
	{
		assert(p<all.size());
		bv.push_back( all[p].me );
		p = all[p].par;
	}
	UL sz = (UL)bv.size();
	if (sz > longest_sequence)
		longest_sequence = sz;
	std::reverse(bv.begin(), bv.end());
	return true;
}

// ----------------------------------------------------------------------------




