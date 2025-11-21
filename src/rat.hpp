
#include <vector>
#include <cstdint>
#include <deque>

class Rat
{
public:
	Rat();
	Rat(int);
	Rat(const Rat&) = default;
	Rat(Rat&&) = default;
	Rat& operator=(const Rat&) = default;
	Rat& operator=(Rat&&) = default;
	~Rat() = default;

	Rat operator-() const;

	bool is_zero() const;
	bool is_neg() const;

	void add(const Rat& other);
	void sub(const Rat& other);

private:
	typedef std::deque<std::uint16_t> USD;
	USD t, n;
	bool neg;
};

