#include <iostream>
#include <algorithm> // for std::copy
#include <span> // for std::span<>
#include <vector> // for std::vector<>
#include <iterator> // for std::next() and std::ostream_iterator<>

// Rotate the array by n steps towards left
template<typename T>
static void rotate_array(std::span<T>&& array, const std::size_t n)
{
	std::vector<T> aux;
	aux.reserve(n);
	aux.insert(aux.begin(), array.cbegin(), std::next(array.cbegin(), n));
	std::copy(std::next(array.begin(), n), array.end(), array.begin());
	std::copy(aux.begin(), aux.end(), std::next(array.begin(), array.size() - n));
}

template<typename T>
static std::ostream& operator<<(std::ostream& stream, const std::vector<T>& v)
{
	stream << "{ ";
	std::copy(v.begin(), v.end(), std::ostream_iterator<T>(stream, ", "));
	stream << " }";
	return stream;
}

int main()
{
	std::vector<std::size_t> input { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	std::cout << "Before: " << input << "\n";
	rotate_array(std::span { input }, 4);
	std::cout << "After: " << input << "\n";
}
