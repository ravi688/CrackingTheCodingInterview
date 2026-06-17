#include <string>
#include <stdexcept>
#include <iostream>

#include <limits>
#include <unordered_map>
#include <algorithm>

struct Input
{
	std::string s1;
	std::string s2;
};

Input ParseInput(int argc, const char** argv)
{
	if(argc < 3)
		throw std::runtime_error("Invalid number of arguments");

	std::string s1 { argv[1] };
	std::string s2 { argv[2] };

	return { std::move(s1), std::move(s2) };
}


// Solution no 1: Sorting
// Time complexity: O(nlog(n) + mlog(m)), where n = len(s1), and m = len(s2)
// Space complexity: O(1), excluding the space required for the sorting of the 2 strings
bool isValidAnagram(const std::string& s1, const std::string& s2)
{
	if(s1.size() != s2.size())
		return false;
	std::string s1Sorted { s1 };
	std::string s2Sorted { s2 };

	std::ranges::sort(s1Sorted, std::less { });
	std::ranges::sort(s2Sorted, std::less { });

	return s1Sorted == s2Sorted;
}

// Solution no 2: Using Hashmap
// Time complexity: O(k* (n + m))
// Space complexity: O(n + m)
bool isValidAnagram2(const std::string& s1, const std::string& s2)
{
	if(s1.size() != s2.size())
		return false;
	std::unordered_map<std::string::value_type, std::size_t> map;
	for(const auto ch : s1)
		map[ch] += 1;
	for(const auto ch: s2)
	{
		auto& freq = map[ch];
		if(freq == 0)
			return false;
		else
			freq -= 1;
	}
	return true;
}

// Solution no 3: Using fixed sized hash table
// Time complexity: O(n + m), where n = len(s1), m = len(s2)
// Space complexity: O(1)
bool isValidAnagram3(const std::string& s1, const std::string& s2)
{
	if(s1.size() != s2.size())
		return false;

	std::size_t freqTable[std::numeric_limits<std::string::value_type>::max() + 1] = { };

	for(const auto ch : s1)
		freqTable[static_cast<unsigned int>(ch)] += 1;
	for(const auto ch: s2)
		freqTable[static_cast<unsigned int>(ch)] -= 1;

	for(const auto freq : freqTable)
		if(freq != 0)
			return false;
	return true;
}

int main(int argc, const char** argv)
{
	Input input = ParseInput(argc, argv);
	std::cout << "Given strings: \n"
		<< "s1: " << input.s1 << "\n"
		<< "s2: " << input.s2 << "\n";

	std::cout << "**-------- Solution no 1 ---------------**\n";
	std::cout << "isValidAnagram: " << std::boolalpha << isValidAnagram(input.s1, input.s2) << std::endl;

	std::cout << "**-------- Solution no 2 ---------------**\n";
	std::cout << "isValidAnagram: " << std::boolalpha << isValidAnagram2(input.s1, input.s2) << std::endl;

	std::cout << "**-------- Solution no 3 ---------------**\n";
	std::cout << "isValidAnagram: " << std::boolalpha << isValidAnagram3(input.s1, input.s2) << std::endl;

	return EXIT_SUCCESS;
}
