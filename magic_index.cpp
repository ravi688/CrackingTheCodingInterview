#include <iostream>
#include <optional>
#include <span>
#include <initializer_list>
#include <concepts>

template<std::integral T>
static std::optional<std::size_t> findMagicIndex1(const std::span<T> array)
{
	for(std::size_t i = 0; i < array.size(); ++i)
		if(static_cast<T>(i) == array[i])
			return { i };
	return { };
}

template<std::integral T>
static std::optional<std::size_t> findMagicIndex2(const std::span<T> array, std::size_t start, std::size_t end)
{
	if(start >= end) return { };
	std::size_t midIndex = (end + start) / 2;
	if(static_cast<T>(midIndex) == array[midIndex])
		return { midIndex };
	auto result = findMagicIndex2(array, start, midIndex);
	if(result)
		return result;
	return findMagicIndex2(array, midIndex + 1, end);
}

template<std::integral T>
static std::optional<std::size_t> findMagicIndex2(const std::span<T> array)
{
	return findMagicIndex2(array, 0, array.size());
}

template<std::integral T>
static std::optional<std::size_t> findMagicIndex3([[maybe_unused]] const std::span<T> array)
{
	return { };
}

template<std::integral T>
struct Solution1
{
	std::optional<std::size_t> operator()(const std::span<T> array)
	{
		return findMagicIndex1(array);
	}
};

template<std::integral T>
struct Solution2
{
	std::optional<std::size_t> operator()(const std::span<T> array)
	{
		return findMagicIndex2(array);
	}
};

template<std::integral T>
struct Solution3
{
	std::optional<std::size_t> operator()(const std::span<T> array)
	{
		return findMagicIndex3(array);
	}
};

template<typename T>
static std::ostream& operator<<(std::ostream& stream, const std::span<const T> array)
{
	stream << "{ ";
	for(decltype(array.size()) i = 0; const auto& value : array)
	{
		stream << value;
		if(++i < array.size())
			stream << ", ";
	}
	stream << " }";
	return stream;
}

template<template<typename> typename Sol, typename T>
static void runFindMagicIndex(const std::span<T> array)
{
	std::cout << "Input: " << array << "\n";
	std::optional<std::size_t> index = Sol<T> { } (array);
	if(index)
		std::cout << "Output: " << *index << "\n";
	else
		std::cout << "Output: not found\n";
}

template<std::integral T>
static void run(std::initializer_list<T> array)
{
	static std::size_t runCount = 0;
	std::cout << "-----------RUN: " << runCount << " ----------\n";
	++runCount;
	std::cout << "**Solution 1**\n";
	runFindMagicIndex<Solution1>(std::span { array });
	std::cout << "**Solution 2**\n";
	runFindMagicIndex<Solution2>(std::span { array });
	std::cout << "**Solution 3**\n";
	runFindMagicIndex<Solution3>(std::span { array });
}

int main()
{
	run<int>({ -4, -2, 1, 0, 4, 5, 12, 12, 54, 65, 100 });
	run<int>({ -4, -3, 100, 200 });
	return 0;
}
