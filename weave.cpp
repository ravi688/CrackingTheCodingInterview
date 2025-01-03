#include <iostream>
#include <span> // for std::span<>
#include <vector> // for std::vector<>
#include <initializer_list> // for std::initializer_list<>
#include <type_traits>

template<template<typename> typename T, typename U>
concept VectorLike = requires(T<U>& array)
{
	{ array.size() } -> std::integral;
	{ array.data() } -> std::same_as<typename std::add_pointer<U>::type>;
};

template<typename T, template<typename> typename Iteratable>
static std::ostream& operator<<(std::ostream& stream, const Iteratable<T>& values) noexcept
{
	stream << "{ ";
	for(decltype(values.size()) i = 0; const auto& value : values)
	{
		stream << value;
		if(++i < values.size())
			stream << ", ";
	}
	stream << " }";
	return stream;
}

// Explanation:
//	Think of the two arrays as two ladders by which one can climb straddling his both legs across the two ladders.
//	Stepping on the first ladder means adding its element, and Stepping on the second ladder means adding its element after
//	the first ladder's one.
//	So this problem boils down to generating the ways in which the climber could step on both the ladders.
template<typename T>
static void generateWeaves(const std::span<const T> s1, const std::span<const T> s2,
				std::size_t index1,
				std::size_t index2,
				std::vector<T>& buffer,
				std::vector<std::vector<T>>& weaves)
{
	// If we have climbed both the ladders completely then save this step trace
	if(buffer.size() == (s1.size() + s2.size()))
	{
		weaves.push_back(buffer);
		return;
	}
	for(auto i = index1; i < s1.size(); ++i)
	{
		// Step on the first ladder
		buffer.push_back(s1[i]);
		// Now recursively generate all possible steps following it
		generateWeaves(s1, s2, i + 1, index2, buffer, weaves);
		// Pop back so that we could generate for the next step on the first ladder
		buffer.pop_back();
	}
	for(auto i = index2; i < s2.size(); ++i)
	{
		// STep on the second ladder
		buffer.push_back(s2[i]);
		// Now recrusively generate all possible steps following it
		generateWeaves(s1, s2, index1, i + 1, buffer, weaves);
		// Pop back so that we could generate for the next step on the second ladder.
		buffer.pop_back();
	}
}

template<typename T>
static std::vector<std::vector<T>> generateWeaves(const std::span<const T> s1, const std::span<const T> s2)
{
	// Number of possible ways of weaving two arrays while maintaining relative order of elements in each array:
	// (n+m)Cm, where n is the cadinality of set 1, and m is the cardinality of set 2.
	std::vector<T> buffer;
	std::vector<std::vector<T>> weaves;
	generateWeaves(s1, s2, 0, 0, buffer, weaves);
	// NRVO would kick-in here
	return weaves;
}

template<typename T>
static void run(std::initializer_list<T> array1, std::initializer_list<T> array2) noexcept
{
	std::cout << "Array1: " << array1 << "\n";
	std::cout << "Array2: " << array2 << "\n";
	std::vector<std::vector<T>> weaves = generateWeaves(std::span { array1 }, std::span { array2 });
	std::cout << "Number of weaves: " << weaves.size() << "\n";
	for(const auto& weave : weaves)
		std::cout << weave << "\n";
}

int main()
{
	run<int>({ 2, 3 }, { 9, 8 });
	return 0;
}
