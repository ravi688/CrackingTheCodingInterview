#include <iostream>

bool check_opposite_sign(int a, int b)
{
	constexpr int numBits = sizeof(a) * 8;
	constexpr int signBitMask = static_cast<int>(1) << (numBits - 1);
	return ((a ^ b) & signBitMask) == signBitMask;
}

bool check_opposite_sign_2(int a, int b)
{
	// NOTE: The expression output x ^ y is negative if x and y have opposite signs.
	return (a ^ b) < 0;
}


int main(int argc, const char** argv)
{
	int a, b;
	std::cout << "Type an integer: ";
	std::cin >> a;
	std::cout << "Type another: ";
	std::cin >> b;

	std::cout << "-----Solution no 1----\n";
	if(check_opposite_sign(a, b))
		std::cout << "Integers " << a << ", " << b << " have opposite signs\n";
	else
		std::cout << "Integers " << a << ", " << b << " do not have opposite signs\n";

	std::cout << "-----Solution no 2----\n";
	if(check_opposite_sign_2(a, b))
		std::cout << "Integers " << a << ", " << b << " have opposite signs\n";
	else
		std::cout << "Integers " << a << ", " << b << " do not have opposite signs\n";
	return 0;
}
