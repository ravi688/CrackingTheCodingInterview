#include <iostream>
#include <chrono> // for timing
#include <vector> // for std::vector<>

// Solution 1 (pure recursion with no memoization)
static std::size_t calcFibonacci1(const std::size_t n)
{
	if(n == 0)
		return 0;
	if(n == 1)
		return 1;
	return calcFibonacci1(n - 1) + calcFibonacci1(n - 2);
}

// Solution 2 (with memoization), this is much faster than Solution 1
// (but not for very small numbers like 0 or 1, because of memory allocation in std::vector)
static std::size_t calcFibonacci2(const std::size_t n, std::vector<std::size_t>& memo)
{
	if(n == 0)
		return 0;
	if(n == 1)
		return 1;
	if(!memo[n])
		memo[n] = calcFibonacci2(n - 1, memo) + calcFibonacci2(n - 2, memo);
	return memo[n];
}

static std::size_t calcFibonacci2(const std::size_t n)
{
	std::vector<std::size_t> memo(n + 1);
	return calcFibonacci2(n, memo);
}

// Solution 3 (pure iterative approach), this is much faster than Solution 1 and Solution 2 both
static std::size_t calcFibonacci3(const std::size_t n)
{
	if(n == 0) return 0;
	std::size_t a = 0;
	std::size_t b = 1;
	for(std::size_t i = 1; i < n; ++i)
	{
		std::size_t t = a + b;
		a = b;
		b = t;
	}
	return a + b;
}

struct Solution1
{
	std::size_t operator()(const std::size_t n)
	{
		return calcFibonacci1(n);
	}
};

struct Solution2
{
	std::size_t operator()(const std::size_t n)
	{
		return calcFibonacci2(n);
	}
};

struct Solution3
{
	std::size_t operator()(const std::size_t n)
	{
		return calcFibonacci3(n);
	}
};

template<typename Sol>
static void runCalcFibonacci(const std::size_t n)
{
	std::cout << "Input: " << n << "\n";
	auto start = std::chrono::steady_clock::now();
	std::size_t value = Sol { } (n);
	auto end = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - start).count();
	std::cout << "Output: " << value << "\n";
	std::cout << "Time taken: " << elapsed << " ms" << "\n";
}

static void run(const std::size_t n)
{
	static std::size_t runCount = 0;
	++runCount;
	std::cout << "------RUN: " << runCount << " ------------ \n";
	std::cout << "**Solution 1**:\n";
	runCalcFibonacci<Solution1>(n);
	std::cout << "**Solution 2**:\n";
	runCalcFibonacci<Solution2>(n);
	std::cout << "**Solution 3**:\n";
	runCalcFibonacci<Solution3>(n);
}

int main()
{
	run(5);
	run(3);
	run(7);
	run(10);
	run(20);
	run(30);
	run(40);
	run(44);
	return 0;
}
