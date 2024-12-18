// Meet in the middle is a technique where the search space is divided into two parts of about equal size.
// A separate search is performed for both of the parts, and finally the results of the searches are combined.

// We are given a list of n numbers and a number x, and we want to find out if it is possible to choose some numbers
// from the list so that their sum is x. For example, given a list [2, 4, 5, 9] and x = 15, we can choose the numbers
// [2, 4, 9] to get 2 + 4 + 9 = 15. However, if x = 10 for the same list, it is not possible to form the sum.

#include <iostream> // for std::cout
#include <vector> // for std::vector<>
#include <numeric> // for std::accumulate()
#include <algorithm> // for std::ranges::sort()

template<typename... Args>
struct Callback
{
	typedef bool (*type)(const std::vector<int>&, Args... args);
};

// Time Complexity: T(n) = 2 * T(n - 1) + c
template<typename... UserDataTypes>
bool traverse_subsets_until(std::vector<int>& subset, std::vector<int>::size_type index, std::vector<int>::size_type startIndex, std::vector<int>::size_type endIndex, const std::vector<int>& values, typename Callback<UserDataTypes...>::type visitor, UserDataTypes... uds) noexcept
{
	if(index == endIndex)
		return visitor(subset, uds...);
	subset.push_back(values[startIndex + index]);
	if(!traverse_subsets_until<UserDataTypes...>(subset, index + 1, startIndex, endIndex, values, visitor, uds...))
	{
		subset.pop_back();
		if(traverse_subsets_until<UserDataTypes...>(subset, index + 1, startIndex, endIndex, values, visitor, uds...))
			return true;
	} else return true;
	return false;
}

template<typename... UserDataTypes>
bool traverse_subsets_until(const std::vector<int>& values, std::vector<int>::size_type start, std::vector<int>::size_type count, typename Callback<UserDataTypes...>::type visitor, UserDataTypes... ud) noexcept
{
	std::vector<int> subset;
	subset.reserve(values.size());
	return traverse_subsets_until<UserDataTypes...>(subset, 0, start, count, values, visitor, ud...);
}


template<typename T>
static std::ostream& operator<<(std::ostream& stream, const std::vector<T>& values) noexcept
{
	stream << "{ ";
	for(decltype(values.size()) i = 0; auto& value : values)
	{
		stream << value;
		if((i + 1) != values.size())
			stream << ", ";
		i += 1;
	}
	stream << " }";
	return stream;
}


// Solution no 1
// Time Complexity: T(n) = 2 * T(n - 1) + n + c
// Approximate Time Complexity: T(n) = 2^n + n*(n - 1) / 2
static bool solution1(const std::vector<int>& values, int targetSum) noexcept
{
	bool isInterrupted = traverse_subsets_until<int>(values, 0, values.size(), [](const std::vector<int>& subset, int targetSum) noexcept -> bool
	{
		bool result = std::accumulate(subset.begin(), subset.end(), 0) == targetSum;
		if(result)
			std::cout << "Subset: " << subset << std::endl;
		return result;
	}, targetSum);
	return isInterrupted;
}

static bool solution2(const std::vector<int>& values, int targetSum) noexcept
{
	auto n = values.size();
	auto leftSize = n >> 1;
	auto rightSize = n - leftSize;
	std::vector<int> leftSubsetSums;
	leftSubsetSums.reserve(leftSize);
	std::vector<int> rightSubsetSums;
	rightSubsetSums.reserve(rightSize);
	traverse_subsets_until<std::vector<int>&>(values, 0, leftSize, [](const std::vector<int>& subset, std::vector<int>& subsetSums) noexcept -> bool
	{
		subsetSums.push_back(std::accumulate(subset.begin(), subset.end(), 0));
		return false;
	}, leftSubsetSums);
	traverse_subsets_until<std::vector<int>&>(values, leftSize, rightSize, [](const std::vector<int>& subset, std::vector<int>& subsetSums) noexcept -> bool
	{
		subsetSums.push_back(std::accumulate(subset.begin(), subset.end(), 0));
		return false;
	}, rightSubsetSums);
	// Sort the left subset sum array into decreasing order
	std::ranges::sort(leftSubsetSums, std::greater { });
	// Sort the right subset sum array into increasing order
	std::ranges::sort(rightSubsetSums, std::less { });
	std::cout << "left: " << leftSubsetSums << std::endl;
	std::cout << "right: " << rightSubsetSums << std::endl;
	// Check if the two lists contain one number in each such that their sum is equal to the 'target sum'
	std::vector<int>::size_type i = 0;
	std::vector<int>::size_type j = 0;
	while((i < leftSubsetSums.size()) && (j < rightSubsetSums.size()))
	{
		auto sum = leftSubsetSums[i] + rightSubsetSums[j];
		if(sum < targetSum)
			j++;
		else if(sum > targetSum)
			i++;
		else return true;
	}
	return false;
}

int main()
{
	std::vector<int> values = { 3, 4, 5, 5, 10, 2, 3, 45, 0, 56 };
	int targetSum = 57;
	std::cout << "Given set: " << values << std::endl;
	std::cout << "Given target sum: " << targetSum << std::endl;
	bool isPossible = solution1(values, targetSum);
	std::cout<< "Solution 1: " << std::boolalpha << isPossible << std::endl;
	isPossible = solution2(values, targetSum);
	std::cout << "Solution 2: " << std::boolalpha << isPossible << std::endl;
	return 0;
}
