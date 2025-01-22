#include <iostream>
#include <concepts> // for std::integral
#include <vector> // for std::vector<>
#include <iterator> // for std::ostream_iterator<>
#include <initializer_list> // for std::intializer_list<>
#include <optional> // for std::optional<>
#include <algorithm> // for std::ranges::sort() and std::copy()

template<std::integral T>
class no_size_vector
{
	private:
		std::vector<T> m_data;
	public:
		no_size_vector(std::initializer_list<T> initList) : m_data(initList) { }
		template<typename... Args>
		no_size_vector(Args&&... args) : m_data(std::forward<Args&&>(args)...) { }

		T operator[](const std::size_t index) const
		{
			if(index >= m_data.size())
				return -1;
			return m_data[index];
		}

		decltype(auto) begin() { return m_data.begin(); }
		decltype(auto) end() { return m_data.end(); }
		decltype(auto) cbegin() const { return m_data.cbegin(); }
		decltype(auto) cend() const { return m_data.cend(); }
		decltype(auto) begin() const { return m_data.begin(); }
		decltype(auto) end() const { return m_data.end(); }

		std::vector<T>& underlying_vector() { return m_data; }
		const std::vector<T>& underlying_vector() const { return m_data; }
};

template<template<typename> typename V, typename T>
concept RangeLike = requires(V<T>& v)
{
	{ *v.begin() } -> std::convertible_to<T>;
	{ *v.end() } -> std::convertible_to<T>;
};

template<typename T, template<typename> typename V> requires(RangeLike<V, T>)
static std::ostream& operator<<(std::ostream& stream, const V<T>& v)
{
	stream << "{ ";
	std::copy(v.begin(), v.end(), std::ostream_iterator<T>(stream, ", "));
	stream << " }";
	return stream;
}

template<typename T>
static std::optional<std::size_t> searchNoSize1(const no_size_vector<T>& v, const T& searchValue)
{
	// Narrow down the binary search space
	std::size_t start = 0;
	std::size_t end = 0;
	while(v[end] != -1)
	{
		if(v[end] == searchValue)
			return { start };
		else if(v[end] < searchValue)
		{
			start = end;
			end = std::max(end * 2, static_cast<std::size_t>(1));
		}
		else break;
	}
	// Perform binary search (in a slightly modified way)
	while(start < end)
	{
		auto mid = (end + start) / 2;
		auto value = v[mid];
		if(value == searchValue)
			return { mid };
		else if(value == -1 || value > searchValue)
			// Search left half including the element at 'mid' as subtracting 1 from mid might lead to unsigned underflow
			end = mid;
		else // if value < searchValue
			// Search right half excluding the element at 'mid' as it is now known to be less than the search value
			start = mid + 1;
	}
	return { };
}

struct Solution1
{
	template<typename T>
	std::optional<std::size_t> operator()(const no_size_vector<T>& v, const T& searchValue)
	{
		return searchNoSize1(v, searchValue);
	}
};

template<typename Sol, typename T>
static void runSearch(const no_size_vector<T>& v, const T& searchValue)
{
	std::optional<std::size_t> result = Sol { } (v, searchValue);
	if(result)
		std::cout << "Found at index: " << result.value() << "\n";
	else
		std::cout << "Not Found\n";
}

template<typename T>
static void run(const no_size_vector<T>& v, const T& searchValue)
{
	std::cout << "Input: " << v << "\n";
	std::cout << "Search Value: " << searchValue << "\n";
	std::cout << "**Solution no 1**\n";
	runSearch<Solution1>(v, searchValue);
}

int main()
{
	no_size_vector<int> input { 1, 2, 3, 4, 5, 6, 7, 23, 43, 54, 32, 34, 54, 65, 76, 100 };
	std::ranges::sort(input.underlying_vector(), std::less { });
	run(input, 10);
	run(input, 1);
	run(input, 20);
	run(input, 13);
	run(input, 2);
	run(input, 6);
	run(input, 8);
	run(input, 7);
	run(input, 23);
	run(input, 43);
	run(input, 54);
	run(input, 32);
	run(input, 34);
	run(input, 65);
	run(input, 76);
	run(input, 100);
	return 0;
}
