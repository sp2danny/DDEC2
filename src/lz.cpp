
#include "lz.hpp"

namespace LZ {

struct LookupEntry;

typedef LookupEntry* LEP;

struct LookupEntry
{
	int code;
	UC token;
	LEP next[256];
};

struct Table
{
	int next;
	void init();
	LEP table[256];
};

void Table::init()
{
	for (int i=0; i<256; ++i)
	{
		
	}
}

[[nodiscard]]
VB compress(const VB& )
{
	return {};
}


[[nodiscard]]
VB decompress(const VB& )
{
	return {};
}



}


