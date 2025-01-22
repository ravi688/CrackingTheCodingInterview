#include <iostream>
#include <span> // for std::span<>
#include <string_view> // for std::string_view<>
#include <array> // for std::array<>
#include <algorithm> // for std::lower_bound() and std::ranges::sort()
#include <iterator> // for std::ostream_iterator<>
#include <optional> // for std::optional<>
#include <vector> // for std::vector<>
#include <chrono> // for timing

// Requires for ""sv operator
using namespace std::literals;

// Solution no 1
static std::optional<std::size_t> SparseSearch1(const std::span<const std::string_view> strs, const std::string_view searchStr)
{
	// Sort the list of strings lexicographically but retain their position info in the original array
	std::vector<std::pair<std::string_view, std::size_t>> sortStrs;
	for(std::size_t i = 0; const auto& value : strs)
		sortStrs.push_back({ value, i++ });
	std::ranges::sort(sortStrs);

	// Perform Binary Search
	auto it = std::lower_bound(sortStrs.begin(), sortStrs.end(), std::pair<std::string_view, std::size_t> { searchStr, 0 },
				[](const auto& v1, const auto& v2) noexcept { return v1.first < v2.first; });
	if(it != sortStrs.end() && it->first == searchStr)
		return { it->second };
	return { };
}

// Solution no 2 (faster than solution no 1)
static std::optional<std::size_t> SparseSearch2(const std::span<const std::string_view> strs, std::size_t begin, std::size_t end, const std::string_view searchStr)
{
	while(begin < end)
	{
		auto mid = (end + begin) / 2;
		auto str = strs[mid];
		// If we hit an empty string then we aren't sure whether to go left or right
		if(str.size() == 0)
		{
			// So first search the right half
			auto result = SparseSearch2(strs, mid + 1, end, searchStr);
			if(result) return result;
			// Then search the left half
			result = SparseSearch2(strs, begin, mid, searchStr);
			return result;
		}
		if(str == searchStr)
			return { mid };
		else if(str < searchStr)
			begin = mid + 1;
		else // if(str > searchStr)
			end = mid;
	}
	return { };
}

static std::optional<std::size_t> SparseSearch2(const std::span<const std::string_view> strs, const std::string_view searchStr)
{
	return SparseSearch2(strs, 0, strs.size(), searchStr);
}

struct Solution1
{
	std::optional<std::size_t> operator()(const std::span<const std::string_view> strs, const std::string_view searchStr)
	{
		return SparseSearch1(strs, searchStr);
	}
};

struct Solution2
{
	std::optional<std::size_t> operator()(const std::span<const std::string_view> strs, const std::string_view searchStr)
	{
		return SparseSearch2(strs, searchStr);
	}
};

template<typename Sol>
static void runSparseSearch(const std::span<const std::string_view> strs, const std::string_view searchStr)
{
	auto start = std::chrono::steady_clock::now();
	std::optional<std::size_t> result = Sol { } (strs, searchStr);
	auto end = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - start).count();
	if(result)
		std::cout << "Found at index: " << result.value() << "\n";
	else
		std::cout << "Not Found\n";
	std::cout << "Time taken: " << elapsed << " ms\n";
}

template<template<typename> typename R, typename T>
concept RangeLike = requires(R<T>& v)
{
	{ *v.begin() } -> std::convertible_to<T>;
	{ *v.end() } -> std::convertible_to<T>;
};

template<typename T, template<typename> typename R> requires(RangeLike<R, T>)
static std::ostream& operator<<(std::ostream& stream, const R<const T>& v)
{
	stream << "{ ";
	std::copy(v.begin(), v.end(), std::ostream_iterator<const T>(stream, ", "));
	stream << " }";
	return stream;
}

static void run(const std::span<const std::string_view> strs, const std::string_view searchStr)
{
	std::cout << "Input: " << strs << "\n";
	std::cout << "Search String: " << searchStr << "\n";
	std::cout << "**Solution no 1**\n";
	runSparseSearch<Solution1>(strs, searchStr);
	std::cout << "**Solution no 2**\n";
	runSparseSearch<Solution2>(strs, searchStr);
}

int main()
{
	std::array strs = { "at"sv, ""sv, ""sv, ""sv, "ball"sv, ""sv, ""sv, "car"sv, ""sv, ""sv, "dad"sv, ""sv, ""sv };
	run(strs, "ball");
	run(strs, "car");
	run(strs, "dad");
	run(strs, "at");
	run(strs, "hello");
	run(strs, "world");
	return 0;
}
