
import std;

template<typename It, typename Snt>
concept Iter = requires(It& it, Snt snt)
{
	{ ++it } -> std::convertible_to<It&>;
	*it;
	{ it != snt } -> std::convertible_to<bool>;
};

auto odds() -> std::generator<int>
{
	int i = 1;
	while (true)
	{
		co_yield i;
		i += 2;
	}
}

template<typename It, typename Snt>
	requires Iter<It, Snt>
void printem(It it, Snt end, int num)
{
	for (int i = 0; i < num; ++i)
	{
		if (it == end) break;
		auto j = *it;
		std::print("{} ", j);
		++it;
	}
	std::println();
}

void test()
{
	auto g = odds();

	printem(g.begin(), g.end(), 10);
}


