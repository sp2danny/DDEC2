
import std;

class linseq
{
public:
	linseq(int start, int stop) : start(start), stop(stop) {}

	struct iterator {
		int operator*() { return val; }
		iterator& operator++() { ++val; return *this; }
		auto operator<=>(const iterator&) const = default;
	friend
		class linseq;
	private:
		iterator(int val) : val(val) {}
		int val;
	};

	iterator begin() const { return {start}; }
	iterator end() const { return {stop+1}; }

private:
	int start, stop;
};

void test()
{
	const int start = 0;
	const int stop = 64;

	linseq seq(start, stop);

	int bsf = -1;
	for (int a : seq)
		for (int b : seq)
			for (int c : seq)
				for (int d : seq)
				{
					int sum = a + b + c + d;
					if (sum != 63) continue;
					int tst = a*b + b*c + c*d;
					if (tst > bsf)
					{
						bsf = tst;
						std::println("{},{},{},{} : {}", a, b, c, d, tst);
					}
				}

}


