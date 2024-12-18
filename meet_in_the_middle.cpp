// Meet in the middle is a technique where the search space is divided into two parts of about equal size.
// A separate search is performed for both of the parts, and finally the results of the searches are combined.

// We are given a list of n numbers and a number x, and we want to find out if it is possible to choose some numbers
// from the list so that their sum is x. For example, given a list [2, 4, 5, 9] and x = 15, we can choose the numbers
// [2, 4, 9] to get 2 + 4 + 9 = 15. However, if x = 10 for the same list, it is not possible to form the sum.

#include <iostream> // for std::cout
#include <vector> // for std::vector<>
#include <numeric> // for std::accumulate()

template<typename UserDataType>
bool traverse_subsets_until(std::vector<int>& subset, std::vector<int>::size_type index, const std::vector<int>& values, bool (*visitor)(const std::vector<int>& subset, UserDataType ud), UserDataType ud) noexcept
{
	if(index == values.size())
		return visitor(subset, ud);
	subset.push_back(values[index]);
	if(!traverse_subsets_until<UserDataType>(subset, index + 1, values, visitor, ud))
	{
		subset.pop_back();
		if(traverse_subsets_until<UserDataType>(subset, index + 1, values, visitor, ud))
			return true;
	} else return true;
	return false;
}

template<typename UserDataType>
bool traverse_subsets_until(const std::vector<int>& values, bool (*visitor)(const std::vector<int>& subset, UserDataType ud), UserDataType ud) noexcept
{
	std::vector<int> subset;
	subset.reserve(values.size());
	return traverse_subsets_until(subset, 0, values, visitor, ud);
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
static bool solution1(const std::vector<int>& values, int targetSum) noexcept
{
	bool isInterrupted = traverse_subsets_until<int>(values, [](const std::vector<int>& subset, int targetSum) noexcept -> bool
	{
		bool result = std::accumulate(subset.begin(), subset.end(), 0) == targetSum;
		if(result)
			std::cout << "Subset: " << subset << std::endl;
		return result;
	}, targetSum);
	return isInterrupted;
}

static bool solution2([[maybe_unused]] const std::vector<int>& values, [[maybe_unused]] int targetSum) noexcept
{
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
