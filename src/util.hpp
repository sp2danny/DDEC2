
#pragma once

#include <string>
#include <string_view>

extern std::string pwd;

extern void getpwd(const char*, bool);

[[nodiscard]] extern std::string replace_ext(std::string_view fn, std::string_view ext);

[[nodiscard]] extern std::string pritty(long long i, const char* token = "'");

#include <bit>
#include <utility>
#include <concepts>

/* */
auto to_little(std::integral auto val)
{

	if constexpr (std::endian::native == std::endian::little) {
		return val;
	} else if constexpr (std::endian::native == std::endian::big) {
		return std::byteswap(val);
	} else {
		//#warning "mixed endianness not supported"
		std::unreachable();
	}
	
}
/* */
