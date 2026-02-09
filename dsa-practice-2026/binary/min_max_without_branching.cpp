#include <iostream>

std::pair<int, int> get_min_max(int a, int b)
{
	int t = a - b;
	constexpr auto numBits = sizeof(int) * 8;
	auto mask = t & (1 << (numBits - 1));
	mask >>= (numBits - 1);
	auto max = (mask & b) | ((~mask) & a);
	auto min = ((~mask) & b) | (mask & a);
	return { min, max };
}

int main()
{
	int a, b;
	std::cout << "Type an integer: ";
	std::cin >> a;
	std::cout << "Type another: ";
	std::cin >> b;

	auto [min, max] = get_min_max(a, b);
	std::cout << "Min: " << min << "\n";
	std::cout << "Max: " << max << "\n";

	return 0;
}
