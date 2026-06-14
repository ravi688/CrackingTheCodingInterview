#include <vector>
#include <iostream>
#include <cstdlib>
#include <iterator>
#include <limits>

#define ERROR(desc) do { std::cerr << desc << "\n"; exit(EXIT_FAILURE); } while(false)

struct Input
{
	std::vector<int> heights;
	int k;
};

Input ParseInput(int argc, const char** argv)
{
	if(argc < 3)
		ERROR("Not enough arguments");
	try {
		int k = std::stoi(argv[1]);
		std::vector<int> heights;
		for(int i = 2; i < argc; ++i)
			heights.push_back(std::stoi(argv[i]));
		return { std::move(heights), k }; 
	} catch(...)
	{
		ERROR("Failed to parse arguments");
	}
}

template<typename T>
concept VisitorType = requires(const T& visit) {
	visit(std::declval<std::vector<int>>());
};

template<VisitorType Visitor>
void traverseSubset(std::vector<int>& v, std::size_t i, int k, const Visitor& visit)
{
	if(i >= v.size())
	{
		visit(v);
		return;
	}

	auto save = v[i];
	v[i] += k;
	traverseSubset(v, i + 1, k, visit);
	if(v[i] > 2 * k)
	{
		v[i] -= 2 * k;
		traverseSubset(v, i + 1, k, visit);
	}
	v[i] = save;
}

std::pair<int, int> getMinMax(const std::vector<int>& v)
{
	int min = v[0];
	int max = v[0];
	for(std::size_t i = 1; i < v.size(); ++i)
	{
		if(min > v[i])
			min = v[i];
		else if(max < v[i])
			max = v[i];
	}
	return { min, max };
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& v)
{
	std::copy(v.begin(), v.end(), std::ostream_iterator<T>(stream, " "));
	return stream;
}


// Solution no 1:
// Time complexity: O(n * 2^n)
// Space complexity: O(n)
int CalculateMinimumDifference(const std::vector<int>& heights, const int k)
{
	std::vector<int> v { heights };
	int minDiff = std::numeric_limits<int>::max();
	traverseSubset(v, 0, k, [&minDiff](const std::vector<int>& v) {
			std::cout << "subset: " << v << "\n";
			auto [min, max] = getMinMax(v);
			auto diff = max - min;
			if(diff < minDiff)
				minDiff = diff;
	});

	return minDiff;
}

int main(int argc, const char** argv)
{
	Input input = ParseInput(argc, argv);
	std::cout << "Heights: " << input.heights << "\n";
	std::cout << "K: " << input.k << "\n";
	int output = CalculateMinimumDifference(input.heights, input.k);
	std::cout << "Output: " << output << std::endl;
	return 0;
}
