#include <iostream> // for std::cout
#include <vector> // for std::vector
#include <set> // for std::set
#include <unordered_set> // for std::unordered_set
#include <concepts> // for std::convertible_to<> concept
#include <utility> // for std::declval
#include <chrono>

template<typename T>
concept ReadOnlyList = requires(const T& list)
{
	{ list.size() } -> std::convertible_to<std::size_t>;
	list.cbegin();
	list.cend();
	typename T::value_type;
};

template<template<typename> typename S, typename T>
concept InsertableSet = requires(S<T>& set)
{
	typename S<T>::value_type;
	set.insert(std::declval<T>());
	{ set.contains(std::declval<T>()) } -> std::convertible_to<bool>;
} && std::same_as<typename S<T>::value_type, T>;

// NOTE: 'InsertableSet<typename T::value_type> S' work here (for some reason which I'm not aware of)
// Therefore, I'm using requires instead.
template<ReadOnlyList T, template<typename> class S> requires InsertableSet<S, typename T::value_type>
auto calculate_num_common_elements(const T& list1, const T& list2) noexcept
{
	std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
	S<typename T::value_type> checkReg;
	for(auto value : list1)
		checkReg.insert(value);
	decltype(list1.size()) count = 0;
	for(auto value : list2)
		if(checkReg.contains(value))
			++count;
	std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
	double elapsedTime = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end - start).count();
	std::cout << "Elapsed Time: " << elapsedTime << " milliseconds" << std::endl;
	return count;
}

template<ReadOnlyList T>
static std::ostream& operator<<(std::ostream& stream, const T& list) noexcept
{
	for(decltype(list.size()) i = 0; auto value : list)
	{
		stream << value;
		++i;
		if(i != list.size())
			stream << ", ";
	}
	return stream;
}

int main()
{
	std::vector<int> list1 = { 5, 2, 8, 9, 4 };
	for(std::size_t i = 0; i < 10000; ++i)
		list1.push_back(2 * i + 1);
	std::vector<int> list2 = { 3, 2, 9, 5 };
	for(std::size_t i = 0; i < 10000; ++i)
		list2.push_back(2 * i);
	//std::cout << "List1: " << list1 << std::endl;
	//std::cout << "List2: " << list2 << std::endl;
	{
		auto count = calculate_num_common_elements<decltype(list1), std::unordered_set>(list1, list2);
		std::cout << "Result (unordered_set): " << count << std::endl;
	}
	{
		auto count = calculate_num_common_elements<decltype(list1), std::set>(list1, list2);
		std::cout << "Result (set): " << count << std::endl;
	}
	return 0;
};
