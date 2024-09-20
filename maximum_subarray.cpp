#include <iostream>

// NOTE: 'auto' in template parameters types are permitted in C++17 and onwards,
// Execute: g++ -std=c++14 <this_file.cpp> -o <output> would lead to syntax errors
template<typename T, auto N>
constexpr std::pair<decltype(N), decltype(N)> get_maximum_sub_array(const T (&array)[N]) noexcept
{
	return {  };
}

template<typename T, auto N>
static void print_array(const T (&array)[N]) noexcept
{
	for(decltype(N) i = 0; i < N; i++)
	{
		std::cout << array[i];
		if((i + 1) < N)
			std::cout << ", ";
	}
	std::cout << std::endl;
}

template<typename T, typename U>
std::ostream& operator<<(std::ostream& stream, const std::pair<T, U>& pair) noexcept
{
	stream << "{ " << pair.first << ", " << pair.second << " }";
	return stream;
}

int main()
{
	int array[] = { -1, 2, 4, -3, 5, 2, -5, 2 };
	std::cout << "Input: "; print_array(array);
	std::pair<int, int> result = get_maximum_sub_array(array);
	std::cout << "Output: " << result << std::endl;
	return 0;
}
