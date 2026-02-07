#include <iostream>
#include <utility> // std::pair<>
#include <span> // for std::span<>
#include <optional> // for std::optional<>
#include <algorithm> // for std::copy()
#include <iterator> // for std::ostream_iterator<>
#include <vector> // for std::vector<>

#include <cassert> // for assert()

// Time complexity: O(n * (n + 1) / 2 ) = O(n^2)
// Space complexity: constant
std::optional<std::pair<int, int>> find_pair(std::span<const int> arr, const int sum)
{
	for(std::size_t i = 0; i < arr.size(); ++i)
	{
		for(std::size_t j = i + 1; j < arr.size(); ++j)
		{
			if((arr[j] + arr[i]) == sum)
				return { { arr[j], arr[i] } };
		}
	}
	return  { };
}

// Time complexity: O(n * log(n) + n * log(n)) = O(n * log(n))
// Space complexity: O(n)
std::optional<std::pair<int, int>> find_pair_2(std::span<const int> arr, const int sum)
{
	std::vector sortedArr(arr.cbegin(), arr.cend());
	std::sort(sortedArr.begin(), sortedArr.end(), std::less { });
	for(std::size_t i = 0; i < arr.size(); ++i)
	{
		int searchValue = sum - arr[i];
		bool isFound = std::binary_search(std::next(sortedArr.cbegin()), sortedArr.cend(), searchValue);
		if(isFound)
		{
			auto it = std::lower_bound(std::next(sortedArr.cbegin()), sortedArr.cend(), searchValue);
			assert(it != sortedArr.cend());
			return { { arr[i], *it } };
		}
	}
	return { };
}

void print_problem(std::span<const int> arr, const int sum)
{
	std::cout << "Input Array: ";
	std::copy(arr.cbegin(), arr.cend(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << "\n" << "Given sum: " << sum << std::endl;
}


int main(const int argc, const char** argv)
{
	std::vector inputArray { 8, 7, 2, 5, 3, 1 };
	int givenSum = 10;

	print_problem(inputArray, givenSum);

	std::cout << "--- Solution no 1 ----- " << std::endl;
	auto foundPair = find_pair(inputArray, givenSum);
	if(foundPair)
		std::cout << "Pair has been found: (" << foundPair->first << ", " << foundPair->second << ")" << std::endl;
	else
		std::cout << "Pair has not been found" << std::endl;

	std::cout << "--- Solution no 2 ----- " << std::endl;
	foundPair = find_pair_2(inputArray, givenSum);
	if(foundPair)
		std::cout << "Pair has been found: (" << foundPair->first << ", " << foundPair->second << ")" << std::endl;
	else
		std::cout << "Pair has not been found" << std::endl;

	return 0;
}
