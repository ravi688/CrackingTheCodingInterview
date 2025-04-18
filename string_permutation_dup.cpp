#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <unordered_set>
#include <unordered_map>
#include <format>
#include <chrono>

static void generatePermutations1(std::string& str, std::string& buffer, std::unordered_set<std::string>& uniques)
{
	if(buffer.size() == str.size())
	{
		uniques.insert(buffer);
		return;
	}
	for(std::size_t i = 0; i < str.size(); ++i)
	{
		if(!str[i]) continue;
		buffer.push_back(str[i]);
		str[i] = 0;
		generatePermutations1(str, buffer, uniques);
		str[i] = buffer.back();
		buffer.pop_back();
	}
}

// Solution no 1
static std::vector<std::string> generatePermutations1(const std::string_view str)
{
	std::vector<std::string> permutations;
	std::string buffer;
	std::string copyStr { str };
	std::unordered_set<std::string> uniques;
	generatePermutations1(copyStr, buffer, uniques);
	for(const auto& value : uniques)
		permutations.push_back(std::move(value));
	return permutations;
}

static constexpr std::size_t getNFactorial(std::size_t n)
{
	std::size_t value = 1;
	while(n > 1)
	{
		value *= n;
		--n;
	}
	return value;
}

// Solution no 2 (faster than Solution no 1)
static std::vector<std::string> generatePermutations2(const std::string_view str)
{
	std::size_t nfact = getNFactorial(str.size());
	std::vector<std::string> permutations;
	std::unordered_set<std::string> uniques;
	std::string copyStr { str };
	while(nfact)
	{
		std::next_permutation(copyStr.begin(), copyStr.end());
		uniques.insert(copyStr);
		--nfact;
	}
	for(const auto& value : uniques)
		permutations.push_back(std::move(value));
	return permutations;
}

static void generatePermutations3(std::unordered_map<char, std::size_t>& freqTable,
				std::size_t n,
				std::string& buffer,
				std::vector<std::string>& permutations)
{
	if(buffer.size() == n)
	{
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

// Solution no 3 (faster than solution no 1 and 2 both)
static std::vector<std::string> generatePermutations3(const std::string_view str)
{
	std::unordered_map<char, std::size_t> freqTable = buildFreqTable(str);
	std::string buffer;
	std::vector<std::string> permutations;
	generatePermutations3(freqTable, str.size(), buffer, permutations);
	return permutations;
}

struct Solution1
{
	std::vector<std::string> operator()(const std::string_view str)
	{
		return generatePermutations1(str);
	}
};

struct Solution2
{
	std::vector<std::string> operator()(const std::string_view str)
	{
		return generatePermutations2(str);
	}
};

struct Solution3
{
	std::vector<std::string> operator()(const std::string_view str)
	{
		return generatePermutations3(str);
	}
};

template<typename Sol>
static void runGeneratePermutations(const std::string_view str)
{
	std::cout << "Input: " << str << "\n";
	auto start = std::chrono::steady_clock::now();
	auto permutations = Sol { } (str);
	auto end = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - start).count();
	std::cout << "Permutation count: " << permutations.size() << "\n";
	std::cout << "Time taken: " << elapsed << " ms\n";
	#ifdef DUMP
	for(std::size_t index = 0; const auto& permutation : permutations)
		std::cout << std::format("[{}] {}", index++, permutation) << "\n";
	#endif
}

static void run(const std::string_view str)
{
	static std::size_t runCount = 0;
	++runCount;
	std::cout << "---------RUN: " << runCount << " ----------\n";
	std::cout << "**Solution no 1**\n";
	runGeneratePermutations<Solution1>(str);
	std::cout << "**Solution no 2**\n";
	runGeneratePermutations<Solution2>(str);
	std::cout << "**Solution no 3**\n";
	runGeneratePermutations<Solution3>(str);
}

int main()
{
	run("hell");
	run("HelooW");
	run("HelooWrrd");
	return 0;
}
