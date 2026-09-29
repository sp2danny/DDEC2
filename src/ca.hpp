
#pragma once

#include <vector>
#include <initializer_list>
#include <utility>

template<typename C, typename I>
C& operator << (C& cont, const I& item) requires requires { cont.push_back(item); }
{
	cont.push_back(item);
	return cont;
}

