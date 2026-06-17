#include <string>
#include <iostream>
#include <stdexcept>


// Time complexity: O(n / 2), where n = len(s)
// Space complexity: O(1)
bool isValidPalindrome(const std::string& s)
{
	auto mid = s.size() / 2;
	for(std::size_t i = 0; i < mid; ++i)
	{
		if(s[i] != s[s.size() - 1 - i])
			return false;
	}
	return true;
}

int main(int argc, const char** argv)
{
	if(argc < 2)
		throw std::runtime_error("Not enough arguments are given");

	std::string s { argv[1] };

	if(isValidPalindrome(s))
		std::cout << "\"" << s << "\"" << " is valid palindrome\n";
	else
		std::cout << "\"" << s << "\"" << " is not a valid palindrome\n";

	return EXIT_SUCCESS;
}
