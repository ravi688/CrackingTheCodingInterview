
// Compiling this file with the following command:
// g++ -std=c++20 -O0 initializer_list.cpp -o ./main
// And executing it under valgrind with memcheck tool:
// valgrind --tool=memcheck ./main
// Reports 0 heap allocations

#include <initializer_list>

static void process_values(const std::initializer_list<int>& list) noexcept
{
	for(int value : list) { /* nothing */ }
}

int main()
{
	int value = 5;
	std::initializer_list<int> values = { 3, 4, 5, 6, value };
	process_values(values);
}
