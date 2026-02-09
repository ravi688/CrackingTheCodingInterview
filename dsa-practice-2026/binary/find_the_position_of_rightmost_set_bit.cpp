#include <iostream>
#include <cmath>


#define TEST_BIT(value, index) (((value) & (static_cast<int>(1) << (index))) >> (index))

struct IntBinary
{
	int _value;
};

std::ostream& operator<<(std::ostream& stream, const IntBinary& intBin)
{
	constexpr int numBits = sizeof(int) * 8;
	for(int i = 0; i < numBits; ++i)
	{
		auto bit = static_cast<char>(TEST_BIT(*reinterpret_cast<const unsigned int*>(&intBin._value), numBits - i - 1));
		stream << static_cast<unsigned int>(*reinterpret_cast<unsigned char*>(&bit));
	}
	return stream;
}

int find_position_right_most_set_bit(int a)
{
	// For odd numbers, the first right most bit is always at index 0 and set.
	if(a & 1)
		return 0;

	// Turn off all the bits except the right most set bit
	a ^= (a & (a - 1));

	// OR we could have also done
	// a = a & ~n

	return std::ceil(std::log2(a));
}

int main([[maybe_unused]] int argc, [[maybe_unused]] const char** argv)
{
	int a;
	std::cout << "Type an integer: ";
	std::cin >> a;
	std::cout << "Given integer: " << a << ", " << IntBinary { a } << "\n";
	int pos = find_position_right_most_set_bit(a);
	std::cout << "Position of the right most set bit: " << pos << "\n";
	return 0;
}
