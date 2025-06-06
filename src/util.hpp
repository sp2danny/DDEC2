
#pragma once

#include <string>
#include <string_view>

extern std::string pwd;

extern void getpwd(const char*, bool);

[[nodiscard]] extern std::string replace_ext(std::string_view fn, std::string_view ext);

[[nodiscard]] extern std::string pritty(long long i, const char* token = "'");

[[nodiscard]] extern bool strmat(const std::string& pattern, const std::string& string);

