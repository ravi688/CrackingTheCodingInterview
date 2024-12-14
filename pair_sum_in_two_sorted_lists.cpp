#include <iostream> // for std::cout
#include <algorithm> // for std::sort<>
#include <array> // for std::array<>
#include <string_view> // for std::string_view<>
#include <concepts> // for std::integral concept
#include <functional> // for std::greater<>()
#include <format> // for std::format()
#include <optional> // for std::optional<>

template<typename T>
concept IteratableContainer = requires(T& cont)
{
	{ cont.size() } -> std::integral;
	{ cont.begin() };
	{ cont.end() };
};

template<IteratableContainer Array>
static void print_array(const std::string_view desc, const Array& array) noexcept
{
	std::cout << desc << ": { ";
	for(decltype(array.size()) i = 0; const auto& value : array)
	{
		std::cout << value;
		if((i + 1) < array.size())
			std::cout << ", ";
		++i;
	}
	std::cout << " }" << std::endl;
}

template<typename T>
concept SubscriptableContainer = requires(T& cont)
{
	{ cont.size() } -> std::integral;
	{ cont[std::declval<decltype(cont.size())>()] };
};


// Time Complexity: O(n) -> Linear
template<SubscriptableContainer Array>
static auto find_pair(const Array& array1, const Array& array2, auto sum) noexcept
{
	auto size1 = array1.size();
	auto size2 = array2.size();
	decltype(size1) i1 = 0;
	decltype(size2) i2 = size2 - 1;
	while((i1 < size1) && (i2 > 0))
	{
		auto tsum = array1[i1] + array2[i2];
		if(tsum < sum)
			++i1;
		else if(tsum > sum)
			--i2;
		else break;
	}
	if((i1 >= size1) || (i2 >= size2))
		return std::optional<std::pair<decltype(i1), decltype(i2)>> { };
	return std::optional { std::make_pair(i1, i2) };
}

// Time Complexity: O(nlog(m))
template<IteratableContainer Array>
static auto find_pair2(const Array& array1, const Array& array2, auto sum) noexcept
{
	for(decltype(array1.size()) i = 0; const auto& value : array1)
	{
		if(value > sum)
		{
			++i;
			continue;
		}
		auto searchValue = sum - value;
		// I think no overload or variant exists in STL equivalent for std::binary_search() which also returns the position of the found element
		if(std::binary_search(array2.begin(), array2.end(), searchValue))
		{
			// If we are sure that the element exists then only we determine position to that element
			auto result = std::lower_bound(array2.begin(), array2.end(), searchValue);
			return std::optional { std::make_pair(i, static_cast<decltype(i)>(std::distance(array2.begin(), result))) };
		}
		++i;
	}
	return std::optional<std::pair<decltype(array1.size()), decltype(array1.size())>> { };
}

int main()
{
	int sum = 100;
	std::array array1 = { 1, 2, 3, -4, 5, -6, 10, -10, -32, -200, -105, 23, 4, 5, 6, 43 };
	std::array array2 = { 4, 5, 32, 5, 6, 23, 12, 65, 205, 332, 422, 4455, 20, 4, 10, 5 };
	// Sort the arrays in ascending order (increasing order)
	std::ranges::sort(array1, std::less { });
	std::ranges::sort(array2, std::less { });
	print_array(std::format("array1[{}]", array1.size()), array1);
	print_array(std::format("array2[{}]", array2.size()), array2);
	std::cout << "Given sum: " << sum << std::endl;
	std::cout << "Solution 1: " << std::endl;
	auto result = find_pair(array1, array2, sum);
	if(result)
	{
		auto [firstArrayIndex, secondArrayIndex] = *result;
		std::cout << "First Array Index: " << firstArrayIndex << "\n";
		std::cout << "Second Array Index: " << secondArrayIndex << std::endl;
	}
	else std::cout << "No such pair of elements exist in the two lists" << std::endl;
	std::cout << "Solution 2: " << std::endl;
	result = find_pair2(array1, array2, sum);
	if(result)
	{
		auto [firstArrayIndex, secondArrayIndex] = *result;
		std::cout << "First Array Index: " << firstArrayIndex << "\n";
		std::cout << "Second Array Index: " << secondArrayIndex << std::endl;
	}
	else std::cout << "No such pair of elements exist in the two lists" << std::endl;
	return 0;
}
