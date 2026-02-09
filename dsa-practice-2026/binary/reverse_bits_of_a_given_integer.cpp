#include <iostream>
#include <bitset>

unsigned int SwapBits(unsigned int a, int i0, int i1)
{
	unsigned int mask0 = 1u << i0;
	unsigned int mask1 = 1u << i1;
	unsigned int b0 = a & mask0;
	unsigned int b1 = a & mask1;
	a &= ~(1 << i0);
	a &= ~(1 << i1);
	a |= (b0 >> i0) << i1;
	a |= (b1 >> i1) << i0;
	return a;
}

int ReverseBits(int a)
{
	constexpr int numBits = sizeof(int) * 8;
	unsigned int _a = *reinterpret_cast<unsigned int*>(&a);
	for(int i = 0; i < (numBits >> 1); ++i)
		_a = SwapBits(_a, i, numBits - i - 1);
	return _a;
}


int main()
{
	int a;
	std::cout << "Type an integer: ";
	std::cin >> a;
	std::cout << "Given bit pattern: " << std::bitset<sizeof(int) * 8>(a) << "\n";

	a = ReverseBits(a);
	std::cout << "Reversed bit pattern: " << std::bitset<sizeof(int) * 8>(a) << "\n";
	return 0;
}
