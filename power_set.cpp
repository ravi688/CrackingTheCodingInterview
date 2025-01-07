#include <vector>
#include <span>
#include <iostream>

// Similar solution is in subsets_gen.cpp
template<typename T, typename U = typename std::remove_cv<T>::type>
static void generatePowerSets1(const std::span<T> array, std::size_t index, std::vector<U>& buffer, std::vector<std::vector<U>>& subsets)
{
	if(index >= array.size())
	{
		subsets.push_back(buffer);
		return;
	}
	buffer.push_back(array[index]);
	generatePowerSets1(array, index + 1, buffer, subsets);
	buffer.pop_back();
	generatePowerSets1(array, index + 1, buffer, subsets);
}

template<typename T, typename U = typename std::remove_cv<T>::type>
static std::vector<std::vector<U>> generatePowerSets1(const std::span<T> array)
{
	std::vector<U> buffer;
	std::vector<std::vector<U>> subsets;
	generatePowerSets1(array, 0, buffer, subsets);
	return subsets;
}

template<typename T>
struct Solution1
{
	auto operator()(const std::span<T> array)
	{
		return generatePowerSets1<T>(array);
	}
};

template<typename T>
static std::ostream& operator<<(std::ostream& stream, const std::span<T> array)
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
static void runGeneratePowerSets(const std::span<T> array)
{
	std::cout << "Input: " << array << "\n";
	auto powerset = Sol<T> { } (array);
	std::cout << "PowerSet cardinality: " << powerset.size() << "\n";
	for(const auto& set : powerset)
		std::cout << std::span{ set } << "\n";
}

template<typename T>
static void run(std::initializer_list<T> array)
{
	runGeneratePowerSets<Solution1>(std::span { std::data(array), array.size() });
}

int main()
{
	run<int>({ 3, 8, 4, 1, 0, 10, 34, 56 });
	return 0;
}
