#include <iostream>


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

int turn_off_bit(int value, int bitIndex)
{
	value &= ~(static_cast<int>(1) << bitIndex);
	return value;
}

int toggle_bit(int value, int bitIndex)
{
	value ^= static_cast<int>(1) << bitIndex;
	return value;
}

int main(int argc, const char** argv)
{
	int a = ~5;
	std::cout << "Given integer: " << a << ", " << IntBinary { a } << "\n";
	a = turn_off_bit(a, 10);
	std::cout << "After turning off bit at index 10: " << a << ", " << IntBinary { a } << "\n";
	a = toggle_bit(a, 10);
	std::cout << "After toggling bit at index 10: " << a << ", " << IntBinary { a } << "\n";
	a = toggle_bit(a, 10);
	std::cout << "Toggling again: " << a << ", " << IntBinary { a } << "\n";
	return 0;
}
