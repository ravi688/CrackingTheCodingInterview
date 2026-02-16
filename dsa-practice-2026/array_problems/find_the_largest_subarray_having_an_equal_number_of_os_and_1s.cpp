#include <iostream>
#include <vector>
#include <iterator>
#include <unordered_map>

std::vector<int> find_largest_subarray(const std::vector<int>& arr)
{
	std::unordered_map<int, std::size_t> map;
	std::size_t index = 0, len = 0;
	int target_sum = 0, sum = 0;
	for(std::size_t i = 0; i < arr.size(); ++i)
	{
		sum += (arr[i] ? 1 : -1);
		if(map.find(sum) == map.end())
			map[sum] = i;

		if(map.find(sum - target_sum) != map.end())
		{
			if(len < (i - map[sum - target_sum]))
			{
				len = i - map[sum - target_sum];
				index = map[sum - target_sum] + 1;
			}
		}
	}
	return std::vector<int>(std::next(arr.begin(), index), std::next(arr.begin(), index + len));
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& arr)
{
	std::copy(arr.begin(), arr.end(), std::ostream_iterator<T>(stream, " "));
	return stream;
}

int main()
{
	std::vector<int> bits { 0, 0, 1, 0, 1, 0, 0 };
	auto largestSubarray = find_largest_subarray(bits);
	std::cout << "given input array (bit array): " << bits << "\n";
	std::cout << "largest subarray: " << largestSubarray << "\n";
	return 0;
}
