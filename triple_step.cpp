#include <iostream>
#include <vector>
#include <chrono>

// Solution no 1
static std::size_t calcWays1(std::size_t n)
{
	if(n == 0)
		return 1;
	std::size_t ways = 0;
	if(n > 0)
	{
		ways += calcWays1(n - 1);
		if(n > 1)
		{
			ways += calcWays1(n - 2);
			if(n > 2)
				ways += calcWays1(n - 3);
		}
	}
	return ways;
}

// Solution no 2
static std::size_t calcWays2(std::size_t n, std::vector<std::size_t>& memo)
{
	if(n == 0)
		return 1;
	if(memo[n])
		return memo[n];
	std::size_t ways = 0;
	if(n > 0)
	{
		ways += calcWays2(n - 1, memo);
		if(n > 1)
		{
			ways += calcWays2(n - 2, memo);
			if(n > 2)
				ways += calcWays2(n - 3, memo);
		}
	}
	memo[n] = ways;
	return ways;
}

static std::size_t calcWays2(std::size_t n)
{
	std::vector<std::size_t> memo(n + 1);
	return calcWays2(n, memo);
}

struct Solution1
{
	std::size_t operator()(std::size_t n) { return calcWays1(n); }
};

struct Solution2
{
	std::size_t operator()(std::size_t n) { return calcWays2(n); }
};

template<typename Sol>
static void runCalcWays(std::size_t n)
{
	std::cout << "Input: " << n << "\n";
	auto start = std::chrono::steady_clock::now();
	auto value = Sol { } (n);
	auto end = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - start).count();
	std::cout << "Output: " << value << "\n";
	std::cout << "Time taken: " << elapsed << " ms\n";
}

static void run(std::size_t n)
{
	static std::size_t runCount = 0;
	++runCount;
	std::cout << "--------RUN: " << runCount << " ---------\n";
	std::cout << "**Solution 1**\n";
	runCalcWays<Solution1>(n);
	std::cout << "**Solution 2**\n";
	runCalcWays<Solution2>(n);
}

int main()
{
	run(0);
	run(1);
	run(2);
	run(3);
	run(4);
	run(5);
	run(6);
	run(7);
	run(10);
	run(20);
	return 0;
}
