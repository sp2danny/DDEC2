
#pragma once

#include <vector>
#include <cstddef>
#include <cstdint>

namespace LZ
{
	

typedef std::uint8_t UC;
typedef std::vector<UC> VB;

[[nodiscard]]
extern vb compress(const VB&);

[[nodiscard]]
extern vb decompress(const VB&);

}

