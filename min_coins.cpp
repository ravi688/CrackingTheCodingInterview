// Greedy algorithm

#include <iostream> // std::cout
#include <concepts> // for std::integral
#include <array> // for std::array
#include <vector> // for std::vector
#include <algorithm> // for std::sort

static constexpr std::size_t INPUT_SIZE = 140;

static constexpr std::array<std::size_t, 4> gCoins = { 10, 5, 15, 100 };

auto GetLargestSmallEql(auto beginIt, auto endIt, auto value) noexcept
{
	for(auto it = beginIt; it != endIt; it++)
	{
		if(*it <= value)
			return it;
	}
	return endIt;
}

template<std::integral auto NCoins>
static void _getMinCoins(std::array<std::size_t, NCoins>& coins, std::integral auto sum, std::vector<std::size_t>& minCoins) noexcept
{
	// Find the largest coin which is equal to 'sum' or less than 'sum'
	auto it = GetLargestSmallEql(coins.begin(), coins.end(), sum);
	if(it != coins.end())
	{
		auto coinValue = *it;
		while(sum >= coinValue)
		{
			minCoins.push_back(coinValue);
			sum -= coinValue;
		}
		_getMinCoins(coins, sum, minCoins);
	}
}

template<std::integral auto NCoins>
static std::vector<std::size_t> getMinCoins(std::array<std::size_t, NCoins> coins, std::integral auto sum) noexcept
{
	// Sort the given coins in decreasing order - meaning the highest value coin comes first (at index 0)
	std::sort(coins.begin(), coins.end(), std::greater { });
	std::vector<std::size_t> minCoins;
	_getMinCoins(coins, sum, minCoins);
	return minCoins;
}

template<typename T>
concept ArrayLikeContainer = requires(T& cont)
{
	{ cont.size() } -> std::integral;
	cont.begin();
	cont.end();
};

template<ArrayLikeContainer T>
static std::ostream& operator<<(std::ostream& stream, const T& array) noexcept
{
	stream << "{ ";
	for(decltype(array.size()) i = 0; auto& value : array)
	{
		stream << value;
		if((i + 1) != array.size())
			stream << ", ";
		++i;
	}
	stream << " }";
	return stream;
}

int main()
{
	std::cout << "Given Coins: " << gCoins << "\n";
	std::cout << "Given Sum: " << INPUT_SIZE << "\n";
	std::vector<std::size_t> minCoins = getMinCoins(gCoins, INPUT_SIZE);
	std::cout << "Minimum Number of Coins: " << minCoins << std::endl;
	return 0;
}
