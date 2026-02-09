#include <iostream>
#include <limits>

int get_absolute_value(int value)
{
	//
	// We know -n = ~n + 1
	// Since flipping the bits of n requires addition of 1 to get -n, we can subtract 1 from n and then only flip the bits which will not require addition of 1
	// So, -n = ~(n - 1)
	// And, since we know that XOR-in 'a' with (all ones) results in flipping the bits of the 'a'.
	// So, -n = (n - 1) ^ (all ones)
	// Hence = (n - 1) ^ (-1)
	constexpr auto numBits = sizeof(int) * 8;
	// Arithmetic shift towards the right. For negative integer, it results in all ones. For positive integer, it results in zero.
	auto mask = (value >> (numBits - 1));
	return (value + mask) ^ mask;
}

int get_absolute_value2(int value)
{
	// With branching: (n >> 31) ? (~n + 1) : n;
	// Without branching: ((n >> 31) & (~n + 1)) | (~(n >> 31) & n)
	int n = value;
	return ((n >> 31) & (~n + 1)) | (~(n >> 31) & n);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] const char** argv)
{
	std::cout << "Type an integer: ";
	int n;
	while(!(std::cin >> n))
	{
		std::cout << "Invalid input\n";
		std::cout << "Type an integer: ";
		std::cin.clear(); // clear fail state
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	std::cout << "---------Solution no 1-------------\n";
	std::cout << "Abs(" << n << ") = " << get_absolute_value(n) << std::endl;
	std::cout << "---------Solution no 2-------------\n";
	std::cout << "Abs(" << n << ") = " << get_absolute_value2(n) << std::endl;
	return 0;
}
