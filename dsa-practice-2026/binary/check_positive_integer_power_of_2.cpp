#include <iostream>

bool is_power_of_2(int value)
{
	// This is equivalent to
	// value & (value + all ones) == 0
	return (value & (value - 1)) == 0;
}


int main([[maybe_unused]] int argc, [[maybe_unused]] const char** argv)
{
	int a;
TAKE_INPUT:
	std::cout << "Type a positive integer: ";
	std::cin >> a;
	if(a < 0)
	{
		std::cerr << "Invalid input, the number should be positive\n";
		goto TAKE_INPUT;
	}
	if(is_power_of_2(a))
		std::cout << "The number " << a << " is a power of 2\n";
	else
		std::cout << "The number " << a << " is not a power of 2\n";
	return 0;
}
