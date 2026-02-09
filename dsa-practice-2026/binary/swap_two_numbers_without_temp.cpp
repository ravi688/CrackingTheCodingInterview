#include <iostream>

std::pair<int, int> swap(int a, int b)
{
	a += b;
	b = a - b;
	a -= b;
	return { a, b };
}

std::pair<int, int> swap_2(int a, int b)
{
	a ^= b;
	b ^= a;
	a ^= b;
	return { a, b };
}

int main(int argc, const char** argv)
{
	int a, b;
	std::cout << "type an integer (a) : ";
	std::cin >> a;
	std::cout << "type another (b) : ";
	std::cin >> b;

	std::cout << "--------Solution no 1------------\n";
	std::cout << "Before swap: (a) = " << a << ", (b) = " << b << "\n";
	std::tie(a, b) = swap(a, b);
	std::cout << "After swap: (a) = " << a << ", (b) = " << b << "\n";

	std::cout << "--------Solution no 2------------\n";
	std::cout << "Before swap: (a) = " << a << ", (b) = " << b << "\n";
	std::tie(a, b) = swap_2(a, b);
	std::cout << "After swap: (a) = " << a << ", (b) = " << b << "\n";
	return 0;
}
