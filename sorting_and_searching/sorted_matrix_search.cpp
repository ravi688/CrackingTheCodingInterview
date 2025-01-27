// Solution no 1:
//	Run linear search for each row until we find the element
//	Time complexity: O(m * n)
// Solution no 2:
//	Run binary search for each row until we find the element
//	Time complexity: O(m * O(log(n)))
// Solution no 3:
//	Run binary search for only the rows which have their first column element less than or equal to the search value
//	Time complexity: O(lower_bound(search_value) * O(log(n))
//	Worst case Time complexity: O(m * O(log(n))

#include <iostream>
#include <array> // for std::array<>
#include <vector> // for std::vector<>
#include <optional> // for std::optional<>
#include <format> // for std::format()
#include <algorithm> // for std::ranges::lower_bound()
#include <iterator> // for std::random_access_iterator<> concept
#include <utility> // for std::declval<>()
#include <chrono> // for timing

template<typename T, std::size_t M, std::size_t N>
using Grid = std::array<std::array<T, N>, M>;

struct Rect
{
	std::size_t x;
	std::size_t y;
	std::size_t width;
	std::size_t height;
};

static std::ostream& operator<<(std::ostream& stream, Rect rect)
{
	stream << "{ " << rect.x << ", " << rect.y << ", " << rect.width << ", " << rect.height << " }";
	return stream;
}

// DOES NOT WORK AS EXPECTED
template<typename T, std::size_t M, std::size_t N>
static std::optional<std::pair<std::size_t, std::size_t>> searchElement(const Grid<T, M, N>& grid, Rect rect, const T& searchValue)
{
	if(rect.width == 0 || rect.height == 0)
		return { };
	std::size_t midX = (rect.x * 2 + rect.width) / 2;
	std::size_t midY = (rect.y * 2 + rect.height) / 2;
	auto& value = grid[midY][midX];
	if(value == searchValue)
		return { { midY, midX } };
	else if(value > searchValue)
		return searchElement(grid, { rect.x, rect.y, rect.width / 2, rect.height / 2 }, searchValue);
	else // if value < searchValue
	{
		auto result = searchElement(grid, { midX, midY, rect.width / 2, rect.height / 2 }, searchValue);
		if(result) return result;
		result = searchElement(grid, { rect.x, midY, rect.width / 2, rect.height / 2 }, searchValue);
		if(result) return result;
		result = searchElement(grid, { midX, rect.y, rect.width / 2, rect.height / 2 }, searchValue);
		return result;
	}
}

template<typename T, std::size_t M, std::size_t N>
static std::optional<std::pair<std::size_t, std::size_t>> searchElement(const Grid<T, M, N>& grid, const T& searchValue)
{
	return searchElement(grid, { 0, 0, N, M }, searchValue);
}

template<typename T>
concept RandomAccessContainer = requires(const T& v)
{
	{ v.begin() } -> std::random_access_iterator;
	{ v.end() } -> std::random_access_iterator;
	v.operator[](std::declval<std::size_t>());
};

template<RandomAccessContainer ContainerType, typename ValueType = ContainerType::value_type>
static std::optional<std::size_t> binary_search(const ContainerType& v, const ValueType& searchValue)
{
	auto it = std::ranges::lower_bound(v, searchValue);
	if(it != v.end() && *it == searchValue)
		return { std::distance(v.begin(), it) };
	return { };
}

template<typename T, std::size_t M, std::size_t N>
static std::optional<std::pair<std::size_t, std::size_t>> searchElement2(const Grid<T, M, N>& grid, const T& searchValue)
{
	for(std::size_t i = 0; const auto& row : grid)
	{
		if(auto result = binary_search(row, searchValue); result)
			return { { i, result.value() } };
		++i;
	}
	return { };
}

template<typename T, std::size_t M, std::size_t N>
static std::optional<std::pair<std::size_t, std::size_t>> searchElement3(const Grid<T, M, N>& grid, const T& searchValue)
{
	// Build a vector of the first column elements in all the rows.
	std::vector<T> firstColVector;
	firstColVector.reserve(M);
	for(const auto& row : grid)
		firstColVector.push_back(row[0]);
	// Find the first element (i.e. row) which is greater than or equal to the search value
	auto it = std::ranges::lower_bound(firstColVector, searchValue);
	// Calculate the iterator to the corresponding row
	if(it != firstColVector.end() && *it == searchValue)
		++it;
	auto rowIt = std::next(grid.begin(), std::distance(firstColVector.begin(), it));
	// Now perform binary search over those specific rows only
	for(auto it = grid.begin(); it != rowIt; ++it)
	{
		if(auto result = binary_search(*it, searchValue); result)
			return { { std::distance(grid.begin(), it), result.value() } };
	}
	return { };
}

struct Solution1
{
	template<typename T, std::size_t M, std::size_t N>
	decltype(auto) operator()(const Grid<T, M, N>& grid, const T& searchValue)
	{
		return searchElement(grid, searchValue);
	}
};

struct Solution2
{
	template<typename T, std::size_t M, std::size_t N>
	decltype(auto) operator()(const Grid<T, M, N>& grid, const T& searchValue)
	{
		return searchElement2(grid, searchValue);
	}
};

struct Solution3
{
	template<typename T, std::size_t M, std::size_t N>
	decltype(auto) operator()(const Grid<T, M, N>& grid, const T& searchValue)
	{
		return searchElement3(grid, searchValue);
	}
};

template<typename Sol, typename T, std::size_t M, std::size_t N>
static void runSearchElement(const Grid<T, M, N>& grid, const T& searchValue)
{
	auto start = std::chrono::steady_clock::now();
	auto result = Sol { }(grid, searchValue);
	auto end = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - start).count();
	if(result)
		std::cout << std::format("Found at [y = {}, x = {}]\n", result->first, result->second);
	else
		std::cout << "Not Found\n";
	std::cout << "Time taken: " << elapsed << " ms\n";
}

template<typename T, std::size_t M, std::size_t N>
static void run(const Grid<T, M, N>& grid, const T& searchValue)
{
	static std::size_t runCount = 0;
	std::cout << "----------RUN: " << runCount << " --------------\n";
	++runCount;
	std::cout << "Search value: " << searchValue << "\n";
	std::cout << "**Solution no 1**\n";
	runSearchElement<Solution1>(grid, searchValue);
	std::cout << "**Solution no 2**\n";
	runSearchElement<Solution2>(grid, searchValue);
	std::cout << "**Solution no 3**\n";
	runSearchElement<Solution3>(grid, searchValue);
}

int main()
{
	std::array<std::array<int, 11>, 10> grid
	{{
		{ 1,   2,  3,  4,  5,  6,  7,  8,  9,  10,  11 },
		{ 11, 12, 13, 14, 15, 16, 17, 18, 19, 110, 111 },
		{ 21, 22, 23, 24, 25, 26, 27, 28, 29, 210, 211 },
		{ 31, 32, 33, 34, 35, 36, 37, 38, 39, 310, 311 },
		{ 41, 42, 43, 44, 45, 46, 47, 48, 49, 410, 411 },
		{ 51, 52, 53, 54, 55, 56, 57, 58, 59, 510, 511 },
		{ 61, 62, 63, 64, 65, 66, 67, 68, 69, 610, 611 },
		{ 71, 72, 73, 74, 75, 76, 77, 78, 79, 710, 711 },
		{ 81, 82, 83, 84, 85, 86, 87, 88, 89, 810, 811 },
		{ 91, 92, 93, 94, 95, 96, 97, 98, 99, 910, 911 }
	}};
	run(grid, 45);
	run(grid, 100);
	run(grid, 91);
	run(grid, 81);
	run(grid, 61);
	run(grid, 52);
	run(grid, 23);
	run(grid, 25);
	run(grid, 47);
	run(grid, 234);
	run(grid, 910);
	run(grid, 911);
	run(grid, 111);
	return 0;
}
