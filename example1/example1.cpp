
/* Example:

	Print all positive integer solutions to the equation a^3 + b^3 = c^3 + d^3 where a, b, c, and
	d are integers between 1 and 1000.
*/

#include <iostream>
#include <vector>
#include <unordered_map>

template<typename T>
static constexpr T cube(T v) { return v * v * v; }

static void print_solutions(size_t min, size_t max)
{
	/*
		Substitution: 
		A + B = C + D, where A = a^3, B = b^3, C = c^3, and D = d^3
		Now the new range would be A, B, C, D belong to { 1, 8, 27, 64, 125, ..., 10^9 }
		log2(10^9) = 9log2(10) = 29.8 ~= 30 bits
	 */

	size_t count = max - min + 1;
	std::vector<std::pair<uint32_t, uint32_t>> nums;
	nums.reserve(count);

	// O(n)
	for(size_t i = 0; i < count; i++)
		nums[i] = std::make_pair<uint32_t, uint32_t>(min + i, cube(min + i));

	size_t half_count = count >> 1; // divide by 2
	std::unordered_map<uint32_t, std::pair<uint32_t, uint32_t>> pairs;
	pairs.reserve(half_count * half_count);

	// O(n2)
	for(size_t i = 0; i < count; i++)
		for(int32_t j = 0; j < count; j++)
			pairs.emplace(std::make_pair(nums[i].second + nums[j].second, std::make_pair(nums[i].first, nums[j].first)));

	using map_iterator = std::unordered_map<uint32_t, std::pair<uint32_t, uint32_t>>::const_iterator;

	// O(n2)
	for(map_iterator it = pairs.cbegin(); it != pairs.cend(); it++)
	{
		for(map_iterator it2 = ++pairs.cbegin(); it2 != pairs.cend(); it2++)
		{
			if(it->first == it2->first)
			{
				std::cout << "(a = " << it->second.first 
						  << ", b = " << it->second.second
						  << ", c = " << it2->second.first
						  << ", d = " << it2->second.second
						  << ")\n"
						  << "(a = " << it->second.second
						  << ", b = "<< it->second.first
						  << ", c = " << it2->second.second
						  << ", d = " << it2->second.first
						  << ")\n"
						  << "(a = " << it->second.second
						  << ", b = "<< it->second.first
						  << ", c = " << it2->second.first
						  << ", d = " << it2->second.second
						  << ")\n"
						  << "(a = " << it->second.first
						  << ", b = "<< it->second.second
						  << ", c = " << it2->second.second
						  << ", d = " << it2->second.first
						  << ")\n";
			}
		}
	}
}


int main(int argc, const char* argv[])
{
	print_solutions(0, 10);
	return 0;
}
