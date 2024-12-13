#include <iostream>
#include <chrono> // for timing
#include <concepts> // for std::integral concept
#include <string_view> // for std::string_view
#include <ctime> // for std::time()
#include <cstdlib> // for std::srand() and std::rand()

// The name of the array is not important here, so we just use ampersand only
template<std::integral auto N>
static auto constexpr array_length(auto (&)[N]) noexcept -> decltype(N)
{
	return N;
}

template<template<typename> typename S, typename T, auto N>
concept Sorter = requires(S<T>& sorter, T (&array)[N])
{
	sorter(array);
};

// NOTE: There is a difference between class template typenames and just normal typenames
// Class template typenames (with one template parameter) are declared as: template<typename> typename
// While the normal typename is declared as just: typename
template<template<typename> typename TSorter, typename T, std::integral auto N> requires(Sorter<TSorter, T, N>)
static constexpr void sort(T (&array)[N]) noexcept
{
	return TSorter<T> { } (array);
}

template<typename T, std::integral auto N>
static constexpr void scramble(T (&array)[N]) noexcept
{
	using SizeType = decltype(N);
	for(SizeType i = 0; i < N; ++i)
		std::swap(array[i], array[std::rand() % N]);
}

template<typename T, std::integral auto N>
static constexpr void print_array(const std::string_view desc, T (&array)[N]) noexcept
{
	std::cout << desc << ": ";
	using SizeType = decltype(N);
	for(SizeType i = 0; i < N; ++i)
		std::cout << array[i] << " ";
	std::cout << std::endl;
}

// Bubble sort strategy
template<typename S>
struct bubble_sort
{
	template<typename T = S, std::integral auto N>
	void operator() (T (&array)[N]) noexcept
	{
		(void)array;
	}
};

// Merge sort strategy
template<typename S>
struct merge_sort
{
	template<typename T = S, std::integral auto N>
	void operator() (T (&array)[N]) noexcept
	{
		(void)array;
	}
};

// Quick sort strategy
template<typename S>
struct quick_sort
{
	template<typename T = S, std::integral auto N>
	void operator() (T (&array)[N]) noexcept
	{
		(void)array;
	}
};

// Heap sort strategy
template<typename S>
struct heap_sort
{
	template<typename T = S, std::integral auto N>
	void operator() (T (&array)[N]) noexcept
	{
		(void)array;
	}
};

// Insertion sort strategy
template<typename S>
struct insertion_sort
{
	template<typename T = S, std::integral auto N>
	void operator() (T (&array)[N]) noexcept
	{
		(void)array;
	}
};

// Selection sort strategy
template<typename S>
struct selection_sort
{
	template<typename T = S, std::integral auto N>
	void operator() (T (&array)[N]) noexcept
	{
		(void)array;
	}
};

static constexpr auto get_elapsed(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end) noexcept
{
	return std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - start).count();
}

template<template<typename> typename Sorter, typename T, std::integral auto N>
static void run(const std::string_view desc, T (&array)[N]) noexcept
{
	scramble(array);
	print_array("Input", array);
	auto start = std::chrono::steady_clock::now();
	sort<Sorter>(array);
	auto end = std::chrono::steady_clock::now();
	std::cout << desc << ": " << get_elapsed(start, end) << " ms" << std::endl;
	print_array("Output", array);
}

int main()
{
	std::srand(std::time(nullptr));

	int array[] = { 23, 3, 43, 1, 2, 4, 6, -4, 6, -100, -200, -4, -6, 0, 1, 0, 2, 0, 23, -100, -333 };
	auto length = array_length(array);
	std::cout << "Input Size: " << length << std::endl;

	run<bubble_sort>("Bubble Sort", array);
	run<merge_sort>("Merge Sort", array);
	run<insertion_sort>("Insertion Sort", array);
	run<selection_sort>("Selection Sort", array);
	run<heap_sort>("Heap Sort", array);
	run<quick_sort>("Quick Sort", array);
	return 0;
}
