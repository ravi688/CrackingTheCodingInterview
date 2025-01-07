#include <iostream>

// Solution no 1
static std::size_t multiply1(std::size_t n1, std::size_t n2)
{
	if(n2 == 1)
		return n1;
	return multiply1(n1, n2 - 1) + n1;
}

// Solution no 2
static std::size_t multiply2(std::size_t n1, std::size_t n2)
{
	if(n2 == 1)
		return n1;
	auto halfn2 = n2 >> 1;
	return (multiply2(n1, halfn2) << 1) + ((n2 & 1) ? n1 : 0);
}

struct Solution1
{
	std::size_t operator()(std::size_t n1, std::size_t n2)
	{
		return multiply1(n1, n2);
	}
};

struct Solution2
{
	std::size_t operator()(std::size_t n1, std::size_t n2)
	{
		return multiply2(n1, n2);
	}
};

template<typename Sol>
static void runMultiply(std::size_t n1, std::size_t n2)
{
	std::cout << "Input: n1 = " << n1 << ", n2 = " << n2 << "\n";
	std::size_t result = Sol { } (n1, n2);
	std::cout << "Output: " << result << "\n";
}

static void run(std::size_t n1, std::size_t n2)
{
	static std::size_t runCount = 0;
	std::cout << "--------RUN: " << runCount << " --------\n";
	++runCount;
	std::cout << "**Solution no 1**\n";
	runMultiply<Solution1>(n1, n2);
	std::cout << "**Solution no 2**\n";
	runMultiply<Solution2>(n1, n2);
}

int main()
{
	run(10, 30);
	run(20, 23);
	run(12, 11);
	run(1, 1);
	run(2, 3);
	run(4, 4);
	run(8, 4);
	run(3, 5);
	return 0;
}
