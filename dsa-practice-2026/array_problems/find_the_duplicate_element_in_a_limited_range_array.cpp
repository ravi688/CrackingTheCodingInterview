#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

int find_duplicate(const std::vector<int>& v)
{
	int n1 = v.size() - 1;
	int nsum = ((n1 + 1) * n1) / 2;
	int s = 0;
	for(std::size_t i = 0; i < v.size(); ++i)
		s += v[i];
	return s - nsum;
}

int find_duplicate2(const std::vector<int>& v)
{
	int r = 0;
	for(std::size_t i = 0; i < v.size(); ++i)
		r ^= v[i];

	for(std::size_t i = 0; i <= (v.size() - 1); ++i)
		r ^= i;

	return r;
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& v)
{
	std::copy(v.begin(), v.end(), std::ostream_iterator<T>(stream, " "));
	return stream;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] const char** argv)
{
	std::cout << "-------------Solution no 1----------\n";
	std::vector array { 1, 2, 3, 4, 4 };
	std::cout << "input array: " << array << "\n";
	auto duplicate = find_duplicate(array);

	std::cout << "duplicate element is: " << duplicate << std::endl;

	std::vector array2 { 1, 2, 3, 4, 2 };
	std::cout << "input array: " << array2 << "\n";

	duplicate = find_duplicate(array2);

	std::cout << "duplicate element is: " << duplicate << std::endl;

	std::cout << "-----------Solution no 2------------\n";
	std::cout << "input array: " << array << "\n";
	duplicate = find_duplicate2(array);

	std::cout << "duplicate element is: " << duplicate << std::endl;

	std::cout << "input array: " << array2 << "\n";
	duplicate = find_duplicate(array2);

	std::cout << "duplicate element is: " << duplicate << std::endl;

	return 0;
}
