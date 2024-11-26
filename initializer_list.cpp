
// If you comment out all std::cout, then
// Compiling this file with the following command:
// g++ -std=c++20 -O0 initializer_list.cpp -o ./main
// And executing it under valgrind with memcheck tool:
// valgrind --tool=memcheck ./main
// Reports 0 heap allocations
//
// If you uncomment the std::cout(s), then it would print garbage values
// Which demonstrates that locally instantiated underlying array of const values gets destroyed as soon as the stack frame is popped out of the stack
// i.e. the function returns.

#include <initializer_list>
#include <iostream> // for std::iostream

static void process_values(const std::initializer_list<int>& list) noexcept
{
	for(int value : list) { /* nothing */ }
}

static std::initializer_list<int> redirect_init(std::initializer_list<int> list) noexcept
{
	return list;
}

static std::initializer_list<int> get_values() noexcept
{
	// a temporary array of constant values is created on the stack frame of 'get_values'
	// so accessing 'values' here is probably safe.
	std::initializer_list<int> values = redirect_init({ 4, 5, 6, 7});
	return values;
}

static void print_values(const std::initializer_list<int>& list) noexcept
{
	for(int value : list)
		std::cout << value << " ";
	std::cout << std::endl;
}

int main()
{
	int value = 5;
	std::initializer_list<int> values = { 3, 4, 5, 6, value };
	process_values(values);

	// Accessing 'values1' is not safe, it would hold garbage values because the locally instantiated underlying array has been destroyed
	// when 'get_values' returns
	std::initializer_list<int> values1 = get_values();
	print_values(values1);
}
