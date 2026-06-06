
#include <vector>
#include <initializer_list>
#include <span>
#include <iostream>
#include <ostream>
#include <iterator>
#include <algorithm>


// Insertion sort
// Time complexity: O(n * (n + 1) / 2) = O(n^2)
// Space complexity: O(n)
template<typename T>
std::vector<T> sort(std::initializer_list<T> a)
{
	std::vector<T> v(a);
	for(std::size_t i = 1; i < v.size(); ++i)
	{
		std::size_t j = i;
		while((j > 0) && (v[j] < v[j - 1]))
		{
			std::swap(v[j], v[j - 1]);
			--j;
		}
	}
	return v;
}


template<typename T>
concept Iteratable = requires(T& v)
{
	v.begin();
	v.end();
};

template<Iteratable T>
std::ostream& operator<<(std::ostream& stream, T a)
{
	std::copy(a.begin(), a.end(), std::ostream_iterator<typename T::value_type>(stream, " "));
	return stream;
}

int main(int argc, const char** argv)
{
	const auto& arr = { 2, 3, 5, 8, 1, 2, 9, 50, 34, 32, 122, 14, 50 };
	std::cout << "Original array: " << arr << std::endl;
	std::vector<int> sortedArray = sort(arr);
	std::cout << "Sorted array: " << sortedArray << std::endl;

	return 0;
}
