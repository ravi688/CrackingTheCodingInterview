// Problem statement:
// The cost of stock on each day is given in an array price[]. Each day you may
// decide to either buy or sell the stock i at price[i], you can even buy and sell
// the stock on the same day. Find the maximum profit that you can get.
// NOTE: A stock can only be sold if it has been bought previously and multiple
// stocks cannot be held on any given day.
//
// Compiling and Buildling:
// g++ -std=c++20 -Wall -Wpedantic stock_buy_and_sell.cpp -o ./main
// ./main 100 180 260 310 40 535 695


#include <vector>
#include <iostream>
#include <cstdlib>
#include <string>
#include <iterator>

std::vector<int> ParseStockPrices(int argc, const char** argv)
{
	if(argc < 2)
	{
		std::cerr << "Not enough arguments are provided\n";
		exit(EXIT_FAILURE);
	}

	std::vector<int> v;
	for(int i = 1; i < argc; ++i)
	{
		// NOTE: std::stoi throws an exception if conversion is not possible or the value falls out of the range of int data type
		int value = std::stoi(std::string { argv[i] });
		v.push_back(value);
	}

	return v;
}

int CalculateMaxProfit(const std::vector<int>& prices)
{
	// We need to find ascending sub-arrays which are continguous across the given array.
	// And calculate the sum of differences.

	int profit = 0;
	int start = 0;
	std::size_t i = 1;
	while(true)
	{
		bool isEndOfArray = (i == prices.size());
		if(isEndOfArray || (prices[i - 1] > prices[i]))
		{
			profit += (prices[i - 1] - prices[start]);
			start = i;
			if(isEndOfArray)
				break;
		}
		++i;
	}
	return profit;
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& v)
{
	std::copy(v.begin(), v.end(), std::ostream_iterator<T>(stream, " "));
	return stream;
}

int main(int argc, const char** argv)
{
	std::vector<int> stockPrices = ParseStockPrices(argc, argv);
	std::cout << "Given stock prices: " << stockPrices << "\n";
	int maxProfit = CalculateMaxProfit(stockPrices);
	std::cout << "Max profit: " << maxProfit << std::endl;
	return 0;
}
