#include <iostream>
#include <concepts> // for std::unsigned_integral
#include <functional> // for std::ref

static constexpr std::size_t INPUT_SIZE = 8;

static void search(bool (&restrictedRows)[],
		bool (&restrictedDiag1)[],
		bool (&restrictedDiag2)[],
		std::size_t n, std::size_t& numPlaced, std::size_t& outNumWays) noexcept
{
	if(numPlaced == n)
	{
		outNumWays += 1;
		return;
	}
	for(std::size_t i = 0; i < n; ++i)
	{
		if(restrictedRows[i] || restrictedDiag1[n - 1 + numPlaced - i] || restrictedDiag2[numPlaced + i])
			continue;
		restrictedRows[i] = 1;
		restrictedDiag1[n - 1 + numPlaced - i] = 1;
		restrictedDiag2[numPlaced + i] = 1;
		numPlaced += 1;
		search(restrictedRows, restrictedDiag1, restrictedDiag2, n, std::ref(numPlaced), std::ref(outNumWays));
		numPlaced -= 1;
		restrictedRows[i] = 0;
		restrictedDiag1[n - 1 + numPlaced - i] = 0;
		restrictedDiag2[numPlaced + i] = 0;
	}
}

// Solution no 1 (Backtracking)
static std::size_t getNumWays1(std::unsigned_integral auto n) noexcept
{
	std::size_t numWays = 0;
	std::size_t numPlaced = 0;
 	bool restrictedRows[n] = { };
	// Max number of diagonals slanted towards right = 2 * n - 1, where n is the number of boxes in a side
	bool restrictedDiag1[2 * n - 1] = { };
	// Max number of diagonals slanted towards left = 2 * n - 1, where n is the number of boxes in a side
	bool restrictedDiag2[2 * n - 1] = { };
	search(restrictedRows, restrictedDiag1, restrictedDiag2, n, std::ref(numPlaced), std::ref(numWays));
	return numWays;
};

int main()
{
	std::cout << "Input Size: " << INPUT_SIZE << std::endl;
	auto numWays = getNumWays1(INPUT_SIZE);
	std::cout << "Num Ways: " << numWays << std::endl;
	return 0;
}
