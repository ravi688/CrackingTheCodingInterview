#include <string>
#include <vector>
#include <vector>
#include <iostream>
#include <stdexcept>


void _generatePermutations(const std::string_view str, std::vector<bool>& mask, std::string& p, std::vector<std::string>& permutes)
{
	static std::size_t cnt = 0;
	if(cnt == mask.size())
	{
		permutes.push_back(p);
		return;
	}
	for(std::size_t i = 0; i < str.size(); ++i)
	{
		if(!mask[i]) continue;
		p.push_back(str[i]);
		mask[i] = false;
		++cnt;
		_generatePermutations(str, mask, p, permutes);
		p.pop_back();
		mask[i] = true;
		--cnt;
	}
}

// Time complexity: O(n^n)
// Space complexity: O(n)
std::vector<std::string> GeneratePermutations(const std::string& str)
{
	std::vector<bool> mask(str.size(), true);
	std::string p;
	std::vector<std::string> permutations;
	_generatePermutations(str, mask, p, permutations);
	return permutations;
}

int main(int argc, const char** argv)
{
	if(argc < 2)
	{
		throw std::runtime_error("Not enough arguments are provided");
	}
	std::string str = argv[1];
	std::cout << "Given string: " << str << "\n";
	std::cout << "Following are the permutations: \n";
	std::vector<std::string> permutations = GeneratePermutations(str);
	for(std::size_t i = 0; const auto& str : permutations)
	{
		std::cout << "[" << i << "] " << str << "\n";
		++i;
	}
	std::cout << std::endl;
	return 0;
}
