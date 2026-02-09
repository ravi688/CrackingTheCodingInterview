#include <iostream>
#include <bitset> // for std::bitset<>

int calculate_xor(int a, int b)
{
	return (!(((~a) & (~b)) | (a & b))) | (((~a) & b) | (a & (~b)));
}

int calculate_xor_2(int a, int b)
{
	return (a | b) - (a & b);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] const char** argv)
{
	int a, b;
	std::cout << "Type an integer: ";
	std::cin >> a;
	std::cout << "Type another: ";
	std::cin >> b;

	std::cout << "Bit pattern (a) = " << std::bitset<sizeof(int) * 8>(a) << "\n";
	std::cout << "Bit pattern (b) = " << std::bitset<sizeof(int) * 8>(b) << "\n";

	std::cout << "----------Solution no 1------------\n";
	int xor_result = calculate_xor(a, b);

	std::cout << "XOR bit pattern = " << std::bitset<sizeof(int) * 8>(xor_result) << "\n";	

	std::cout << "----------Solution no 2------------\n";
	xor_result = calculate_xor_2(a, b);

	std::cout << "XOR bit pattern = " << std::bitset<sizeof(int) * 8>(xor_result) << "\n";
	return 0;
}
