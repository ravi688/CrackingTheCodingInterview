#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>


template<typename T1, typename T2>
static std::ostream& operator<<(std::ostream& stream, const std::pair<T1, T2>& pair)
{
	stream << "{ " << pair.first << ", " << pair.second << " }";
	return stream;
}

template<typename T>
static std::ostream& operator<<(std::ostream& stream, const std::vector<T>& values)
{
	stream << "{ ";
	for(std::size_t i = 0; const auto& value : values)
	{
		stream << value;
		if(++i < values.size())
			stream << ", ";
	}
	// Using this don't allow deduction of template arguments for the std::pair<> overload of operator<<
	//std::copy(values.begin(), values.end(), std::ostream_iterator<T>(stream, ", "));
	stream << " }";
	return stream;
}

static void numWays(std::size_t sum, std::size_t index,
					const std::vector<std::size_t>& coins,
					const std::vector<std::size_t>& freqTable,
					std::vector<std::pair<std::size_t, std::size_t>>& buffer,
					std::vector<std::vector<std::pair<std::size_t, std::size_t>>>& coinGroups)
{
	// Base case
	if(sum == 0)
	{
		coinGroups.push_back(buffer);
		return;
	}
	// If this is path won't lead to the exact sum
	if(index >= freqTable.size())
		return;
	auto freq = freqTable[index];
	auto coinValue = coins[index];
	for(std::size_t i = 0; i <= freq; ++i)
	{
		// If this coin can be accumulated 'i' number of times which would equal to or less than the sum
		// Then propage the remainder sum to the next set of coins to see the rest can be fulfilled with those.
		if((i * coinValue) <= sum)
		{
			// We can remove this check in theory, but in practice we don't want to see some coins taken 0 times,
			// which is true in theory but not practically.
			if(i > 0)
				buffer.push_back({ coinValue, i });
			numWays(sum - (i * coinValue), index + 1, coins, freqTable, buffer, coinGroups);
			if(i > 0)
				buffer.pop_back();
		}
	}
}

static std::vector<std::size_t> buildFreqTable(const std::vector<std::size_t>& coins, std::size_t sum)
{
	std::vector<std::size_t> freqTable;
	freqTable.reserve(coins.size());
	for(const auto& coinValue : coins)
		freqTable.push_back(sum / coinValue);
	return freqTable;
}

static std::vector<std::vector<std::pair<std::size_t, std::size_t>>> numWays(std::size_t sum, const std::vector<std::size_t>& coins)
{
	std::vector<std::vector<std::pair<std::size_t, std::size_t>>> coinGroups;
	std::vector<std::pair<std::size_t, std::size_t>> buffer;
	// Build a table of (sum / coins[i]) values
	std::vector<std::size_t> freqTable = buildFreqTable(coins, sum);
	numWays(sum, 0, coins, freqTable, buffer, coinGroups);
	return coinGroups;
}

struct Solution1
{
	std::vector<std::vector<std::pair<std::size_t, std::size_t>>> operator()(std::size_t sum, const std::vector<std::size_t>& coins)
	{
		return numWays(sum, coins);
	}
};

template<typename Sol>
static void runNumWays(std::size_t sum, const std::vector<std::size_t>& coins)
{
	std::vector<std::vector<std::pair<std::size_t, std::size_t>>> ways = Sol { } (sum, coins);
	std::cout << "num ways: " << ways.size() << "\n";
	std::cout << "ways: \n";
	for(const auto& way : ways)
		std::cout << way << "\n";
}

static void run(std::size_t sum, const std::vector<std::size_t>& coins)
{
	std::cout << "Input: sum = " << sum << ", coins = " << coins << "\n";
	std::cout << "**Solution no 1**\n";
	runNumWays<Solution1>(sum, coins);
}

int main()
{
	// List initialization
	std::vector<std::size_t> coins { 25, 10, 5, 1 };
	std::ranges::sort(coins, std::less { });
	run(0, coins);
	run(1, coins);
	run(3, coins);
	run(5, coins);
	run(7, coins);
	run(10, coins);
	run(20, coins);
	return 0;
}
