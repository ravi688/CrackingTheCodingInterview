#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <string_view>
#include <chrono>
#include <functional>

static bool isValidParens(const std::string_view str)
{
	std::size_t checker = 0;
	for(const auto& ch : str)
	{
		if(ch == '(')
			checker += 1;
		else if(ch == ')')
		{
			if(checker > 0)
				checker -= 1;
			else return false;
		}
	}
	return true;
}

static void generatePermutations3(std::unordered_map<char, std::size_t>& freqTable,
				std::size_t n,
				std::string& buffer,
				std::vector<std::string>& permutations)
{
	if(buffer.size() == n)
	{
		if(isValidParens(buffer))
			permutations.push_back(buffer);
		return;
	}
	for(auto& pair : freqTable)
	{
		if(pair.second > 0)
		{
			pair.second -= 1;
			buffer.push_back(pair.first);
			generatePermutations3(freqTable, n, buffer, permutations);
			buffer.pop_back();
			pair.second += 1;
		}
	}
}

static std::unordered_map<char, std::size_t> buildFreqTable(const std::string_view str)
{
	std::unordered_map<char, std::size_t> map;
	for(const auto& ch : str)
		map[ch] += 1;
	return map;
}

static std::vector<std::string> generatePermutations3(const std::string_view str)
{
	std::unordered_map<char, std::size_t> freqTable = buildFreqTable(str);
	std::string buffer;
	std::vector<std::string> permutations;
	generatePermutations3(freqTable, str.size(), buffer, permutations);
	return permutations;
}

static std::vector<std::string> validParens1(const std::size_t n)
{
	std::string str;
	for(std::size_t i = 0; i < n; ++i)
		str.append("()");
	return generatePermutations3(str);
}

static void validParens2(std::size_t openCount, std::size_t closeCount, std::string& str, std::vector<std::string>& parens)
{
	// If all places hold either '(' or ')' then we have reached to one possible valid permutation
	if((openCount + closeCount) == str.size())
	{
		parens.push_back(str);
		return;
	}
	// The number of open parentheses can never greater than the half of the number of chars in the string,
	// Because, there needs to be as many close parentheses to cancel the open parentheses, totalling the number of chars to the size of the string.
	// So, here we only add an open parentheses if the current number of open parantheses is less than the half of the string's size.
	if(openCount < (str.size() / 2))
	{
		str[openCount + closeCount] = '(';
		// Recursively generate next valid '(' or ')'
		validParens2(openCount + 1, closeCount, str, parens);
	}
	// Only add close parantheses if the current number of close parantheses is less than the open parantheses
	if(openCount > closeCount)
	{
		str[openCount + closeCount] = ')';
		// Recursively generate next valid '(' or ')'
		validParens2(openCount, closeCount + 1, str, parens);
	}
}

static std::vector<std::string> validParens2(const std::size_t n)
{
	std::string str(n * 2, ' ');
	std::vector<std::string> parens;
	validParens2(0, 0, str, parens);
	return parens;
}

struct Solution1
{
	std::vector<std::string> operator()(const std::size_t n)
	{
		return validParens1(n);
	}
};

struct Solution2
{
	std::vector<std::string> operator()(const std::size_t n)
	{
		return validParens2(n);
	}
};

template<typename Sol>
static void runValidParens(const std::size_t n)
{
	std::cout << "Input: " << n << "\n";
	auto start = std::chrono::steady_clock::now();
	auto validParens = Sol { } (n);
	auto end = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - start).count();
	std::cout << "Number of valid parens: " << validParens.size() << "\n";
	std::cout << "Time taken: " << elapsed << " ms\n";
	#ifdef DUMP
	for(std::size_t i = 0; const auto& parens : validParens)
		std::cout << "[" << i++ << "] " << parens << "\n";
	#endif
}

static void run(const std::size_t n)
{
	static std::size_t runCount = 0;
	++runCount;
	std::cout << "-----------RUN: " << runCount << " ------------\n";
	std::cout << "**Solution no 1**\n";
	runValidParens<Solution1>(n);
	std::cout << "**Solution no 2**\n";
	runValidParens<Solution2>(n);
}

int main()
{
	run(3);
	run(4);
	run(5);
	run(6);
	run(10);
	return 0;
}
