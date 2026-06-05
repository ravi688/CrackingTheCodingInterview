// Implement an algorithm to determine if a string has all unique characters. What if you cannot use additional data structures ? 

#include <cstdlib>
#include <cassert> // for assert
#include <string> // for std::string
#include <limits> // for std::numeric_limits
#include <algorithm> // for std::ranges::sort
#include <bitset> // for std::bitset<>
#include <unordered_set> // for std::unordered_set<>

// Solution 1: visit each unique ordered pair of characters inside the string and check if one element of the pair is identical to the another.
// Time complexity: O(n * (n + 1) / 2)
// Space complexity: O(1)
bool has_all_unique_characters(const std::string& str)
{
	for(std::size_t i = 0; i < str.size(); ++i)
	{
		for(std::size_t j = i + 1; j < str.size(); ++j)
		{
			if(str[i] == str[j])
				return false;
		}
	}
	return true;
}

// Solution 2: Maintain a fixed hash table to keep track which characters have been appeared before. And check if it is found again.
// Time complexity: O(n)
// Space complexity: O(n)
bool has_all_unique_characters_2(const std::string& str)
{
	bool isPresent[std::numeric_limits<std::string::value_type>::max()] = { };
	for(const char ch: str)
	{
		if(isPresent[static_cast<unsigned int>(ch)])
			return false;
		else
			isPresent[static_cast<unsigned int>(ch)] = true;
	}
	return true;
}

// Solution 3: Sort the character array in increasing order and check if adjacent characters are same.
// Time complexity: O(n * log(n))
// Space complexity: O(n)
bool has_all_unique_characters_3(const std::string& str)
{
	std::string t { str };
	std::ranges::sort(t, std::ranges::less { });
	for(std::size_t i = 1; i < t.size(); ++i)
	{
		if(t[i - 1] == t[i])
			return false;
	}
	return true;
}

// Solution 4: Use std::bitset
// Time complexity: O(n)
// Space complexity: O(n)
bool has_all_unique_characters_4(const std::string& str)
{
	std::bitset<std::numeric_limits<std::string::value_type>::max()> isPresent;
	for(const char ch: str)
	{
		if(isPresent.test(static_cast<unsigned int>(ch)))
			return false;
		else
			isPresent.set(static_cast<unsigned int>(ch));
	}
	return true;
}

// Solution 5: Use std::unordered_set
// Time complexity: O(n * log(n))
// Space complexity: O(n)
bool has_all_unique_characters_5(const std::string& str)
{
	std::unordered_set<char> m;
	for(const char ch : str)
	{
		if(m.contains(ch))
			return false;
		else
			m.insert(ch);
	}
	return true;
}

int main(int argc, const char** argv)
{
	const std::string input1 = "ABCDEEEEFFFGG";
	const std::string input2 = "ABCDEFGHIJKLM";

	assert(has_all_unique_characters(input1) == false);
	assert(has_all_unique_characters(input2) == true);

	assert(has_all_unique_characters_2(input1) == false);
	assert(has_all_unique_characters_2(input2) == true);

	assert(has_all_unique_characters_3(input1) == false);
	assert(has_all_unique_characters_3(input2) == true);

	assert(has_all_unique_characters_4(input1) == false);
	assert(has_all_unique_characters_4(input2) == true);

	assert(has_all_unique_characters_5(input1) == false);
	assert(has_all_unique_characters_5(input2) == true);

	return EXIT_SUCCESS;
}
