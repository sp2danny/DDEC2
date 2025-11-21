
#include "rat.hpp"

#include <cassert>

Rat::Rat()
{
	neg = false;
	t.push_back(0);
}

Rat::Rat(int i)
	: Rat()
{
	if (!i) return;
	if (i < 0) {
		neg = true;
		i = -i;
	}
	n.push_back(1);

	while (true) {
		auto tt = i & 0xffff;
		t.push_back((std::uint16_t)tt);
		i = i >> 16;
		if (!i) break;
	}

}

bool Rat::is_zero() const
{
	return (t.size() == 1) && (t[0] == 0);
}

bool Rat::is_neg() const
{
	return neg;
}


Rat Rat::operator-() const
{
	Rat res = *this;
	if (!res.is_zero())
		res.neg = !res.neg;
	return res;
}

void Rat::add(const Rat& other)
{
	if (other.is_neg())
		return sub(-other);
}

void Rat::sub(const Rat& other)
{
	if (other.is_neg())
		return add(-other);

}




