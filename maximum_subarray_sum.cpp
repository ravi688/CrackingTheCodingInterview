#include <iostream>
#include <concepts>

// NOTE: 'auto' in template parameters types are permitted in C++17 and onwards,
// Execute: g++ -std=c++14 <this_file.cpp> -o <output> would lead to syntax errors
template<typename T, auto N>
constexpr T get_maximum_sub_array_sum1(const T (&array)[N]) noexcept
{
	// Time Complexity: O(n^2)
	T maxSum = array[0];
	using SizeType = decltype(N);
	for(SizeType i = 1; i <= N; ++i)
	{
		T sum = 0;
		for(SizeType j = i; j < N; ++j)
			sum += array[j];
		for(SizeType j = i; j < N; ++j)
			sum += (array[j] - array[j - i]);
		if(sum > maxSum)
			maxSum = sum;
	}
	return maxSum;
}

template<typename T, auto N>
constexpr T get_maximum_sub_array_sum2(const T (&array)[N]) noexcept
{
	// Time Complexity: O(n)
	using SizeType = decltype(N);
	T currMax = array[0];
	T globalMax = currMax;
	for(SizeType i = 1; i < N; ++i)
	{
		// Explaination:
		//	Here, it is possible that 'currMax + array[i]' is less than 'array[i]'
		//	in which case, we have a gurantee that 'array[i]' can compensate for the next subarray,
		// 	therefore, we reset the 'currMax' to 'array[i]' and start summing up again from there.
		currMax = std::max(array[i], currMax + array[i]);
		if(currMax > globalMax)
			globalMax = currMax;
	}
	return globalMax;
}

// Time Complexity: O(n * (n + 1) / 2) = O(n^2)
template<typename T, std::integral auto N>
constexpr T get_maximum_sub_array_sum3(const T (&array)[N]) noexcept
{
	T maxSum = array[0];
	using SizeType = decltype(N);
	for(SizeType i = 0; i < N; ++i)
	{
		T sum = 0;
		for(SizeType j = i; j < N; ++j)
		{
			sum += array[j];
			maxSum = std::max(sum, maxSum);
		}
	}
	return maxSum;
}

template<typename T, auto N>
static void print_array(const T (&array)[N]) noexcept
{
	for(decltype(N) i = 0; i < N; i++)
	{
		std::cout << array[i];
		if((i + 1) < N)
			std::cout << ", ";
	}
	std::cout << std::endl;
}

template<typename T, typename U>
std::ostream& operator<<(std::ostream& stream, const std::pair<T, U>& pair) noexcept
{
	stream << "{ " << pair.first << ", " << pair.second << " }";
	return stream;
}

int main()
{
	int array[] = { -1, 2, 4, -3, 5, 2, -5, 2 };
	std::cout << "Input: "; print_array(array);
	int result1 = get_maximum_sub_array_sum1(array);
	int result2 = get_maximum_sub_array_sum2(array);
	int result3 = get_maximum_sub_array_sum3<int>(array);
	std::cout << "Output 1: " << result1 << std::endl;
	std::cout << "Output 2: " << result2 << std::endl;
	std::cout << "Output 3: " << result3 << std::endl;
	return 0;
}
