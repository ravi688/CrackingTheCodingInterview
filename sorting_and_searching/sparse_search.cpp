#include <iostream>
#include <span> // for std::span<>
#include <string_view> // for std::string_view<>
#include <array> // for std::array<>
#include <algorithm> // for std::lower_bound() and std::ranges::sort()
#include <iterator> // for std::ostream_iterator<>
#include <optional> // for std::optional<>
#include <vector> // for std::vector<>

// Requires for ""sv operator
using namespace std::literals;

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

struct Solution1
{
	std::optional<std::size_t> operator()(const std::span<const std::string_view> strs, const std::string_view searchStr)
	{
		return SparseSearch1(strs, searchStr);
	}
};

template<typename Sol>
static void runSparseSearch(const std::span<const std::string_view> strs, const std::string_view searchStr)
{
	std::optional<std::size_t> result = Sol { } (strs, searchStr);
	if(result)
		std::cout << "Found at index: " << result.value() << "\n";
	else
		std::cout << "Not Found\n";
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
	runSparseSearch<Solution1>(strs, searchStr);
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
