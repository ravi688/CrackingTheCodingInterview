#include <string>
#include <iostream>
#include <stdexcept>

unsigned int CalculateNthFibonacci(unsigned int n)
{
	// Fibonacci series
	// 0 1 1 2 3 5 8 13 21 ...
	
	if(n <= 1)
		return 0;
	if(n == 2)
		return 1;

	return CalculateNthFibonacci(n - 1) + CalculateNthFibonacci(n - 2);
}


int main(int argc, const char** argv)
{
	if(argc < 2)
		throw std::runtime_error("Not enough arguments are provided");

	unsigned int n = std::stoul(std::string(argv[1]));

	std::cout << "Given n = " << n << "\n";

	std::cout << "nth fibonacci number = " << CalculateNthFibonacci(n) << "\n";

	return 0;
}
