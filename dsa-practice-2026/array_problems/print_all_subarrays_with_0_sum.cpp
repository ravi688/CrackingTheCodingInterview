#include <iostream>
#include <vector> // for std::vector<>
#include <iterator> // for std::next(), and std::ostream_iterator<>
#include <algorithm> // for std::copy
#include <numeric> // for std::accumulate()
#include <unordered_map> // for std::unordered_multimap

bool check_zero_subarray(const std::span<const int> arr, std::size_t begin, std::size_t end)
{
	auto value = std::accumulate(std::next(arr.begin(), begin), std::next(arr.begin(), end), 0);
	return value == 0;
}

// Time complexity: O(n ^ 3)
// Space complexity: O(n), worst case
std::vector<std::vector<int>> find_zero_subarrays(const std::vector<int>& arr)
{
	std::vector<std::vector<int>> zeroSubarrays;
	for(std::size_t i = 0; i < arr.size(); ++i)
	{
		for(std::size_t j = i + 1; j <= arr.size(); ++j)
		{
			if(check_zero_subarray(arr, i, j))
				zeroSubarrays.push_back(std::vector(std::next(arr.begin(), i), std::next(arr.begin(), j)));
		}
	}
	return zeroSubarrays;
}

// Time complexity: O(n ^ 2)
// Space complexity: O(n), worst case
std::vector<std::vector<int>> find_zero_subarrays_2(const std::vector<int>& arr)
{
	std::vector<std::vector<int>> zeroSubarrays;
	for(std::size_t i = 0; i < arr.size(); ++i)
	{
		int runningSum = 0;
		for(std::size_t j = i; j < arr.size(); ++j)
		{
			runningSum += arr[j];
			if(runningSum == 0)
				zeroSubarrays.push_back(std::vector(std::next(arr.begin(), i), std::next(arr.begin(), j + 1)));
		}
	}
	return zeroSubarrays;
}

// Time complexity: O(n), average/best case
// Space complexity: O(n), worst case
std::vector<std::vector<int>> find_zero_subarrays_3(const std::vector<int>& arr)
{
	std::vector<std::vector<int>> zeroSubarrays;
	std::unordered_multimap<int, std::size_t> map;
	int runningSum = 0;
	map.insert({ 0, 0 });
	for(std::size_t i = 0; i < arr.size(); ++i)
	{
		runningSum += arr[i];
		auto range = map.equal_range(runningSum);
		auto it = range.first;
		while(it != map.end() && it != range.second)
		{
			auto index = it->second;
			zeroSubarrays.push_back(std::vector(std::next(arr.begin(), index), std::next(arr.begin(), i + 1)));
			++it;
		}
		map.insert({ runningSum, i + 1 });
	}
	return zeroSubarrays;
}


void runTest(const std::vector<int>& arr, auto algorithm)
{
	auto zeroSubarrays = algorithm(arr);

	if(zeroSubarrays.size())
	{
		std::cout << "Found zero subarrays: \n";
		for(const auto& subarrays : zeroSubarrays)
		{
			std::copy(subarrays.cbegin(), subarrays.cend(), std::ostream_iterator<int>(std::cout, " "));
			std::cout << "\n";
		}
	}
	else
		std::cout << "No zero subarray was found\n";
	std::cout << std::flush;

}

int main(int argc, const char** argv)
{
	std::cout << "---------------Solution no 1:---------------\n";
	std::cout << "**TEST 0**\n";
	runTest({ 4, 2, -3, -1, 0, 4 }, find_zero_subarrays);
	std::cout << "**TEST 1**\n";
	runTest({ 3, 4, -7, 3, 1, 3, 1, -4, -2, -2 }, find_zero_subarrays);

	std::cout << "---------------Solution no 2:---------------\n";
	std::cout << "**TEST 0**\n";
	runTest({ 4, 2, -3, -1, 0, 4 }, find_zero_subarrays_2);
	std::cout << "**TEST 1**\n";
	runTest({ 3, 4, -7, 3, 1, 3, 1, -4, -2, -2 }, find_zero_subarrays_2);


	std::cout << "---------------Solution no 3:---------------\n";
	std::cout << "**TEST 0**\n";
	runTest({ 4, 2, -3, -1, 0, 4 }, find_zero_subarrays_3);
	std::cout << "**TEST 1**\n";
	runTest({ 3, 4, -7, 3, 1, 3, 1, -4, -2, -2 }, find_zero_subarrays_3);

	return 0;
}
