
import std;


void goes_to()
{
	long double x = 0.5;

	while (true)
	{
		auto r = std::pow(x, x);
		std::print("{:11f}          {:11f}          \r", x, r);
		x *= 0.99999l;
	}

}
