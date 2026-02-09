#include <iostream>
#include <bitset> // for std::bitset<>


enum class Parity : unsigned char
{
	Even,
	Odd
};

Parity compute_parity(int value)
{
 // 0 0 0 0 1 0 0 0 1 0 0 -> even parity
 // 0 0 0 0 1 1 0 0 1 0 0 -> odd parity

	int set_bit_count = 0;
	// Turn off the right most set bit at each iteration
	// This will give us the number of set bits in the given bit pattern
	while(value != 0)
	{
		value &= (value - 1);
		++set_bit_count;
	}
	// Based on the bit count is even or odd, we can tell the parity of the bit pattern
	return (set_bit_count & 1) ? Parity::Odd : Parity::Even;
}

Parity compute_parity2(int value)
{
	bool even_parity = true;
	while(value)
	{
		if(value & 1)
			even_parity = !even_parity;
		value >>= 1;
	}
	return even_parity ? Parity::Even : Parity::Odd;
}

void show_parity(Parity parity, int value)
{
	if(parity == Parity::Even)
		std::cout << value << " has even parity\n";
	else
		std::cout << value << " has odd parity\n";

}

int main([[maybe_unused]] int argc, [[maybe_unused]] const char** argv)
{
	int value;
	std::cout << "Type an integer: ";
	std::cin >> value;
	std::cout << "Bit pattern: " << std::bitset<sizeof(int) * 8>(value) << "\n";
	std::cout << "------Solution no 1----------\n";
	Parity parity = compute_parity(value);
	show_parity(parity, value);
	std::cout << "------Solution no 2----------\n";
	parity = compute_parity2(value);
	show_parity(parity, value);
	return 0;
}
