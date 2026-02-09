#include <iostream>

bool is_odd(int value)
{
	return (value & 1) == 1;
}

int main(int argc, const char** argv)
{
	std::cout << "type integer: ";
	int value;
	std::cin >> value;

	if(is_odd(value))
		std::cout << "value " << value << " is an odd integer\n";
	else
		std::cout << "value " << value << " is an even integer\n";
	return 0;
}
