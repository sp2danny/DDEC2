
#include "lz.hpp"

namespace LZ {

struct LookupEntry;

typedef LookupEntry* LEP;

struct LookupEntry
{
	int code;
	std::LZ token;
	LEP next[256];
}

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
vb compress(const vb& in)
{
	return {};
}


[[nodiscard]]
vb decompress(const vb& in)
{
	return {};
}



}


