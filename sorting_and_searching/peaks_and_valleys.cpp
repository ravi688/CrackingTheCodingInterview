#include <iostream>
#include <span> // for std::span<>
#include <vector> // for std::vector<>
#include <iterator> // for std::ostream_iterator<>
#include <algorithm> // for std::copy()

template<typename T1, typename T2>
static int signedCompare(const T1& v1, const T2& v2)
{
	if(v1 == v2) return 0;
	return (v2 > v1) ? 1 : -1;
}

template<typename T>
static bool checkAlternating(const std::span<T>& array)
{
	if(array.size() < 3) return false;
	int prevSign = signedCompare(array[0], array[1]);
	if(prevSign == 0)
		return false;
	for(std::size_t i = 1; (i + 1) < array.size(); ++i)
	{
		int curSign = signedCompare(array[i], array[i + 1]);
		if((prevSign + curSign) != 0)
			return false;
		prevSign = curSign;
	}
	return true;
}

template<typename T>
static std::ostream& operator<<(std::ostream& stream, const std::vector<T>& array)
{
	std::copy(array.begin(), array.end(), std::ostream_iterator<T>(stream, ", "));
	return stream;
}

template<typename T>
static void testCheckAlternating(const std::vector<T>& array, bool expected)
{
	bool result = checkAlternating(std::span { array });
	if(result != expected)
	{
		std::cerr << std::boolalpha << "Error: Check Alternating Test failed, expected: " << expected << ", for sequence : " << array << "\n";
		std::exit(EXIT_FAILURE);
	}
}

static void runWarmupTests()
{
	testCheckAlternating<int>({ 1, 0, 3, 2, 100, 10, 80, 4, 90 }, true);
	testCheckAlternating<int>({ 1, 0, 3, 2, 100, 10, 80 }, true);
	testCheckAlternating<int>({ 2, 100, 10, 80, 4, 90 }, true);
	testCheckAlternating<int>({ 2, 2, 2, 2, 2 }, false);
	testCheckAlternating<int>({ 2, 0, 2, -1, 2 }, true);
	std::cout << "Info: All warmup check alternating tests passed\n";
}

template<typename T>
static void fixupForAlternating(std::vector<T>& sortedArr)
{
	if(sortedArr.size() < 3) return;
	for(std::size_t i = 1; i < sortedArr.size(); i += 2)
		std::swap(sortedArr[i], sortedArr[i - 1]);
}

template<typename T>
static std::vector<typename std::remove_const<T>::type> convertToPeaksAndValleys(const std::span<T>& array)
{
	std::vector<typename std::remove_const<T>::type> copyArray { array.begin(), array.end() };
	std::ranges::sort(copyArray, std::greater { });
	fixupForAlternating(copyArray);
	return copyArray;
}

struct Solution1
{
	template<typename T>
	std::vector<typename std::remove_const<T>::type> operator()(const std::span<T>& array)
	{
		return convertToPeaksAndValleys(array);
	}
};

template<typename Sol, typename T>
static void runPeaksAndValleys(const std::span<T>& array)
{
	std::vector<typename std::remove_const<T>::type> result = Sol { } (array);
	std::cout << "Output: " << result << "\n";
	testCheckAlternating<typename std::remove_const<T>::type>(result, true);
}

template<typename T>
static void run(const std::vector<T>& array)
{
	static std::size_t runCount = 0;
	std::cout << "------------RUN: " << runCount << " -------------\n";
	++runCount;
	std::cout << "Input: " << array << "\n";
	std::cout << "**Solution no 1**\n";
	runPeaksAndValleys<Solution1>(std::span { array });
}

int main()
{
	runWarmupTests();
	run<int>({ 1, 10, 80, 4, 100, 2, 0, 3, 5 });
	run<int>({ 5, 3, 1, 2, 3 });
	return 0;
}
