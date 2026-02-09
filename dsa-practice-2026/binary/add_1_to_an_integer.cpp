#include <iostream>

int add_one(int value)
{
	// a + ~a = (all ones) = 2^N - 1
	// a + ~a + 1 = (all ones) = 2^N
	// a + 1 = -(-2^N + ~a)
	// a + 1 = -~a
	
	return -(~value);
}


int main(int argc, const char** argv)
{
	std::cout << "type an integer: ";
	int value;
	std::cin >> value;

	auto value2 = add_one(value);
	std::cout << value << " + 1 = " << value2 << "\n";

	return 0;
}
