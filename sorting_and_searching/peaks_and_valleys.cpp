#include <iostream>
#include <span> // for std::span<>
#include <vector> // for std::vector<>
#include <iterator> // for std::ostream_iterator<>
#include <algorithm> // for std::copy()
#include <chrono> // for timing

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

// Solution no 1
template<typename T>
static std::vector<typename std::remove_const<T>::type> convertToPeaksAndValleys(const std::span<T>& array)
{
	std::vector<typename std::remove_const<T>::type> copyArray { array.begin(), array.end() };
	std::ranges::sort(copyArray, std::greater { });
	fixupForAlternating(copyArray);
	return copyArray;
}

template<typename T>
static std::size_t getMaxIndex(const std::span<T>& array, std::size_t i1, std::size_t i2, std::size_t i3)
{
	auto v1 = array[i1], v2 = array[i2], v3 = array[i3];
	if(v1 > v2)
	{
		if(v1 > v3)
			// v1 > v2 and v1 > v3
			return i1;
		else
			// v2 < v1 <= v3
			return i3;
	}
	else
	{
		if(v2 > v3)
			// v1 <= v2 and v3 < v2
			return i2;
		else
			// v1 < v2 <= v3
			return i3;
	}
}

// Solution no 2 (faster than solution no 1)
template<typename T>
static std::vector<typename std::remove_const<T>::type> convertToPeaksAndValleys2(const std::span<T>& array)
{
	std::vector<typename std::remove_const<T>::type> copyArray { array.begin(), array.end() };
	for(std::size_t i = 1; (i + 1) < array.size(); i += 2)
	{
		auto maxIndex = getMaxIndex(array, i - 1, i, i + 1);
		std::swap(copyArray[i], copyArray[maxIndex]);
	}
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

struct Solution2
{
	template<typename T>
	std::vector<typename std::remove_const<T>::type> operator()(const std::span<T>& array)
	{
		return convertToPeaksAndValleys2(array);
	}
};

template<typename Sol, typename T>
static void runPeaksAndValleys(const std::span<T>& array)
{
	auto start = std::chrono::steady_clock::now();
	std::vector<typename std::remove_const<T>::type> result = Sol { } (array);
	auto end = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - start).count();
	std::cout << "Output: " << result << "\n";
	std::cout << "Time taken: " << elapsed << " ms\n";
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
	std::cout << "**Solution no 2**\n";
	runPeaksAndValleys<Solution2>(std::span { array });
}

int main()
{
	runWarmupTests();
	run<int>({ 1, 10, 80, 4, 100, 2, 0, 3, 5 });
	run<int>({ 5, 3, 1, 2, 3 });
	return 0;
}
