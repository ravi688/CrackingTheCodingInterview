#include <iostream> // for std::cout, and std::cin
#include <format> // for std::format()
#include <string_view> // for std::string_view
#include <limits> // for std::numeric_limits<>
#include <concepts> // for std::integral
#include <cassert> // for assert()
#include <cstdlib> // for std::srand(), and std::rand()
#include <ctime> // for std::time()

template<typename T>
static T GetInput(std::string_view promptMessage)
{
	T dest;
	while(true)
	{
		std::cout << promptMessage;
		std::cin >> dest;
		if(std::cin.fail())
		{
			std::cerr << "Invalid Input\n";
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		} else break;
	}
	return dest;
}

// Tags
struct Solution1 { };
struct Solution2 { };

template<typename Selection>
struct AddWithoutPlus { };

template<>
struct AddWithoutPlus<Solution1>
{
	template<std::integral T>
	decltype(auto) operator()(const T& a, const T& b)
	{
		constexpr std::size_t numBits = sizeof(T) << 3;
		T carryBit = 0;
		T result = 0;
		for(std::size_t i = 0; i < numBits; ++i)
		{
			auto aBit = a & (static_cast<T>(1) << i);
			auto bBit = b & (static_cast<T>(1) << i);
			aBit >>= i;
			bBit >>= i;
			auto cBit = aBit ^ bBit;
			auto oldCarryBit = carryBit;
			carryBit = (aBit & bBit) | (cBit & carryBit);
			cBit ^= oldCarryBit;
			result |= (cBit << i);
		}
		return result;
	}
};

template<>
struct AddWithoutPlus<Solution2>
{
	template<std::integral T>
	decltype(auto) operator()(T a, T b)
	{
		if(b == 0) return a;
		auto sum = a ^ b;
		auto carry = (a & b) << 1;
		return AddWithoutPlus<Solution2>{}(sum, carry);
	}
};

template<std::integral T>
static void Verify(const T result, const T a, const T b)
{
	auto refResult = a + b;
	assert(result == refResult);
}

template<typename Selection>
static void RunTests()
{
	std::srand(std::time(0));
	for(std::size_t i = 0; i < 1000; ++i)
	{
		long long int a = std::rand();
		long long int b = std::rand();
		auto result = AddWithoutPlus<Selection>{}(a, b);
		Verify(result, a, b);
	}
	std::cout << "All tests passed\n";
}

int main()
{
	RunTests<Solution1>();
	RunTests<Solution2>();
	long long int a = GetInput<long long int>("Please enter first integer: ");
	long long int b = GetInput<long long int>("Please enter second integer: ");
	auto result = AddWithoutPlus<Solution1>{}(a, b);
	std::cout << "Solution 1: " << std::format("{} + {} = {}", a, b, result) << "\n";
	Verify(result, a, b);
	result = AddWithoutPlus<Solution2>{}(a, b);
	std::cout << "Solution 2: " << std::format("{} + {} = {}", a, b, result) << "\n";
	Verify(result, a, b);
}
