#include <iostream>
#include <iterator>
#include <vector>
#include <unordered_map>

// Time complexity: O(n * (n + 1) / 2) = O(n^2)
// Space complexity: O(n), worst case
std::vector<int> find_max_len_subarray(const std::vector<int>& arr, int sum)
{
	std::pair<int, int> maxLenSubArr { 0, 0 };

	for(std::size_t i = 0; i < arr.size(); ++i)
	{
		int _sum = 0;
		for(std::size_t j = i; j < arr.size(); ++j)
		{
			_sum += arr[j];
			if(_sum == sum)
			{
				if(static_cast<std::size_t>(maxLenSubArr.second - maxLenSubArr.first) < (j - i + 1))
				{
					maxLenSubArr.first = i;
					maxLenSubArr.second = j + 1;
				}
			}
		}
	}
	std::cout << "start index = " << maxLenSubArr.first << ", end index (exclusive) = " << maxLenSubArr.second << "\n";
	return std::vector<int>(std::next(arr.begin(), maxLenSubArr.first), std::next(arr.begin(), maxLenSubArr.second));
}

// Time complexity: O(n)
// Space complexity: O(n), worst case
std::vector<int> find_max_len_subarray_2(const std::vector<int>& arr, int sum)
{
	std::unordered_map<int, std::size_t> map;
	std::size_t len = 0;
	int index = 0;
	int _sum = 0;
	for(std::size_t i = 0; i < arr.size(); ++i)
	{
		_sum += arr[i];
		// Concept/Idea:
		// <------------------------------> S2
		// <------------------->S1<--------> S3
		// +++++++++++++++++++++&&&&&&&&&&&&
		// Value of S3 = S2 - S1
		// if (S3 == sum)
		// 	we have found a subarry having sum equal to the target sum (sum)
		if(map.find(_sum) == map.end())
			map[_sum] = i;
		if(map.find(_sum - sum) != map.end() && len < (i - map[_sum - sum]))
		{
			len = i - map[_sum - sum];
			index = map[_sum - sum] + 1;
		}
	}
	return std::vector<int>(std::next(arr.begin(), index), std::next(arr.begin(), index + len));
}

// Special case: for unsigned integers
// Time complexity: O(n)
// Space complexity: O(1)
std::vector<unsigned int> find_max_len_subarray_3(const std::vector<unsigned int>& arr, unsigned sum)
{
	std::size_t right = 0;
	std::size_t left = 0;
	unsigned int _sum = 0;
	std::size_t index = 0;
	std::size_t len = 0;
	while(right < arr.size())
	{
		if(_sum < sum)
			_sum += arr[right++];
		else if(_sum > sum && (left < right))
			_sum -= arr[left++];
		else // _sum == sum
		{
			if(len < (right - left))
			{
				len = right - left;
				index = left;
			}
			if(right < arr.size())
				_sum += arr[right++];
		}
	}
	return std::vector<unsigned int>(std::next(arr.begin(), index), std::next(arr.begin(), index + len));
}

// Special case: for unsigned integers
// Time complexity: O(n)
// Space complexity: O(1)
std::vector<unsigned int> find_max_len_subarray_4(const std::vector<unsigned int>& arr, unsigned sum)
{
	std::size_t len = 0, index = 0;
	std::size_t left = 0, right = 0;
	unsigned int _sum = 0;
	for(; right < arr.size(); ++right)
	{
		_sum += arr[right];
		while(_sum > sum && left < right)
			_sum -= arr[left++];
		if(_sum == sum)
		{
			if(len < (right - left + 1))
			{
				len = right - left + 1;
				index = left; 
			}
		}
	}
	return std::vector<unsigned int>(std::next(arr.begin(), index), std::next(arr.begin(), index + len));
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& v)
{
	std::copy(v.begin(), v.end(), std::ostream_iterator<T>(stream, " "));
	return stream;
}

int main()
{
	std::vector<int> arr { 5, 6, -5, 5, 3, 5, -2, 0 };
	int sum = 8;
	std::cout << "input array: " << arr << "\n";
	std::cout << "sum: " << sum << "\n";
	std::cout << "-----Solution no 1-----\n";
	auto subarr = find_max_len_subarray(arr, sum);
	std::cout << "max length subarray: " << subarr << std::endl;
	std::cout << "-----Solution no 2-----\n";
	subarr = find_max_len_subarray_2(arr, sum);
	std::cout << "max length subarray: " << subarr << std::endl;
	std::cout << "-----Solution no 3-----\n";
	std::vector<unsigned int> uarr { 1, 2, 3, 2, 5, 1, 1, 2, 3, 2 };
	std::cout << "input array: " << uarr << "\n";
	std::cout << "sum: " << sum << "\n";
	auto usubarr = find_max_len_subarray_3(uarr, static_cast<unsigned int>(sum));
	std::cout<< "max length subarray: " << usubarr << "\n";
	std::cout << "-----Solution no 4-----\n";
	usubarr = find_max_len_subarray_4(uarr, static_cast<unsigned int>(sum));
	std::cout << "max length subarray: " << usubarr << "\n";
	return 0;

}
