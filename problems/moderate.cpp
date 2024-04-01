#include <iostream>


// Write a function to swap a number in place (that, without temporary variables)

template<typename T>
void problem1_print_header();

template<>
void problem1_print_header<int>()
{
	std::cout << "Problem 1 <int>: " << std::endl;
}

template<>
void problem1_print_header<float>()
{
	std::cout << "Problem 1 <float>: " << std::endl;
}

template<typename T>
void problem1()
{
	problem1_print_header<T>();
	T a = 100;
	T b = 200;

	std::cout << "\t a = " << a << std::endl;
	std::cout << "\t b = " << b << std::endl;

	b += a;
	a = b - a;
	b = b - a;

	std::cout << "\t after swapping inplace: " << std::endl;
	std::cout << "\t a = " << a << std::endl;
	std::cout << "\t b = " << b << std::endl;
}

int main()
{
	problem1<int>();
	problem1<float>();
	return 0;
}
