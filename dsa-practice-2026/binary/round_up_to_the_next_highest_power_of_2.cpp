#include <iostream>
#include <cmath> // for std::log2()

int round_up_next_power_of_2(int n)
{
	if((n & (n - 1)) == 0)
		return n;
	int t;
	do
	{
		t = n;
		n &= (n - 1);
	} while(n);

	return t << 1;
}

int round_up_next_power_of_2_2(int n)
{
	if((n & (n - 1)) == 0)
		return n;

	int lg = std::log2(n);

	return 1 << (lg + 1);
}

int main()
{
	int a;
	std::cout << "Type an integer: ";
	std::cin >> a;

	std::cout << "-------Solution no 1------\n";
	int nextPowerOf2 = round_up_next_power_of_2(a);
	std::cout << "Next power of 2: " << nextPowerOf2 << std::endl;
	
	std::cout << "-------Solution no 2------\n";
	nextPowerOf2 = round_up_next_power_of_2_2(a);
	std::cout << "Next power of 2: " << nextPowerOf2 << std::endl;

	return 0;
}
