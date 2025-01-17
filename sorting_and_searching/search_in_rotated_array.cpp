#include <iostream>
#include <algorithm> // for std::copy
#include <span> // for std::span<>
#include <vector> // for std::vector<>
#include <iterator> // for std::next() and std::ostream_iterator<>
#include <optional> // for std::optionaL<>
#include <chrono> // for timing

template<typename T, typename U = T>
static std::optional<std::size_t> binary_search(const std::span<T>& array, std::size_t start, std::size_t end, const U& value)
{
	while(start < end)
	{
		std::size_t middle = (start + end) / 2;
		if(array[middle] == static_cast<T>(value))
			return { middle };
		else if(static_cast<T>(value) < array[middle])
			end = middle;
		else
			start = middle + 1;
	}
	return { };
}

template<typename T, typename U = T>
static std::optional<std::size_t> find_in_rotated_array(const std::span<T>& array, std::size_t start, std::size_t end, const U& value)
{
	// If we are run out of the elements to search
	if(start >= end) return { };
	// We can't divide a cell (a single element array) into half
	// It can only be divided into an empty array and the original one element array itself
	// So we need to detect this case here and handle it.
	if((end - start) == 1)
	{
		if(array[start] == static_cast<T>(value))
			return { start };
		else return { };
	}
	std::size_t middle = (end + start) / 2;
	std::optional<std::size_t> index;
	// If the left half is sorted
	if(array[start] <= array[middle])
	{
		// Exclude middle, perform binary search in the left half since it is sorted
		index = binary_search(array, start, middle, value);
		if(!index)
		// Include middle, perform find_in_rotated_array in the right half since it is not fully sorted
			index = find_in_rotated_array(array, middle, end, value);
	}
	else // or if the right half is sorted
	{
		// Include middle, perform binary search in the right half since it is sorted
		index = binary_search(array, middle, end, value);
		if(!index)
			// Exclude middle, perform find_in_rotated_array in the left half since it is not fully sorted
			index = find_in_rotated_array(array, start, middle, value);
	}
	return index;
}

template<typename T, typename U = T>
static std::span<T>::iterator find_in_rotated_array(const std::span<T>& array, const U& value)
{
	auto result = find_in_rotated_array(array, 0, array.size(), value);
	if(!result)
		return array.end();
	return std::next(array.begin(), result.value());
}

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

struct Solution1
{
	template<typename T, typename U = T>
	auto operator()(const std::span<T>& array, const U& value)
	{
		return find_in_rotated_array(array, value);
	}
};

struct Solution2
{
	template<typename T, typename U = T>
	auto operator()(const std::span<T>& array, const U& value)
	{
		return array.end();
	}
};

template<typename Sol, typename T, typename U = T>
static void runFindInRotatedArray(const std::vector<T>& array, const U& value)
{
	const std::span<const T> s { array };
	auto start = std::chrono::steady_clock::now();
	auto it = Sol { } (s, value);
	auto end = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - start).count();
	if(it != s.end())
		std::cout << "Found at: " << std::distance(s.begin(), it) << "\n";
	else
		std::cout << "Not found\n";
	std::cout << "Time taken: " << elapsed << " ms\n";
}

template<typename T, typename U = T>
static void run(const std::vector<T>& array, const U& value)
{
	static std::size_t runCount = 0;
	std::cout << "----------RUN: " << runCount << " ----------\n";
	++runCount;
	std::cout << "value to find: " << value << "\n";
	std::cout << "**Solution no 1**\n";
	runFindInRotatedArray<Solution1>(array, value);
	std::cout << "**Solution no 2**\n";
	runFindInRotatedArray<Solution2>(array, value);
}

template<typename T, typename U = T>
static void test_binary_search(const std::span<T> v, const U& value, bool expected)
{
	auto result = binary_search<T, U>(std::span { v }, 0, v.size(), value);
	if(result.has_value() != expected)
		std::cout << "binary search test failed for value: " << value << "\n";
	else if(result.has_value())
		std::cout << "binary search found index " << result.value() << " for value: " << value << "\n";
}

int main()
{
	std::vector<std::size_t> input { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	std::cout << "----------Binary Search Test--------------\n";
	test_binary_search(std::span { input }, 1, true);
	test_binary_search(std::span { input }, 2, true);
	test_binary_search(std::span { input }, 3, true);
	test_binary_search(std::span { input }, 4, true);
	test_binary_search(std::span { input }, 5, true);
	test_binary_search(std::span { input }, 6, true);
	test_binary_search(std::span { input }, 7, true);
	test_binary_search(std::span { input }, 8, true);
	test_binary_search(std::span { input }, 9, true);
	test_binary_search(std::span { input }, 10, false);
	std::cout << "---------Array Rotate Test----------------\n";
	std::cout << "Before: " << input << "\n";
	rotate_array(std::span { input }, 4);
	std::cout << "After: " << input << "\n";
	std::cout << "---------Find in Rotated Array------------\n";
	run(input, 1);
	run(input, 2);
	run(input, 3);
	run(input, 4);
	run(input, 5);
	run(input, 6);
	run(input, 7);
	run(input, 8);
	run(input, 9);
	run(input, 10);
	run(input, 11);
	return 0;
}
