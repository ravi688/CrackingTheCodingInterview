#include <iostream> // for std::cout
#include <concepts> // for std::integral
#include <bitset> // for std::bitset

static constexpr auto INPUT_SIZE = 4;

template<std::integral auto N>
static void getNumPaths(std::bitset<N> (&grid)[N], std::integral auto& outNumPaths, std::integral auto& numVisited, std::integral auto i, std::integral auto j) noexcept
{
	numVisited += 1;
	grid[i][j] = true;
	// If this is the bottom most and right most cell
	if(((i + 1) == N) && ((j + 1) == N))
	{
		// And this path has visited all the cells
		if(numVisited == (N * N))
			// Count this path as a possible path
			outNumPaths += 1;
		numVisited -= 1;
		grid[i][j] = false;
		return;
	}
	// Get possible paths from the right cell
	if(((i + 1) < N) && !grid[i + 1][j])
		getNumPaths<N>(grid, outNumPaths, numVisited, i + 1, j);
	// Get possible paths from the below cell
	if(((j + 1) < N) && !grid[i][j + 1])
		getNumPaths<N>(grid, outNumPaths, numVisited, i, j + 1);
	// Get possible paths from the left cell
	if((i > 0) && !grid[i - 1][j])
		getNumPaths<N>(grid, outNumPaths, numVisited, i - 1, j);
	// Get possible paths from the above cell
	if((j > 0) && !grid[i][j - 1])
		getNumPaths<N>(grid, outNumPaths, numVisited, i, j - 1);
	numVisited -= 1;
	grid[i][j] = false;
}

template<std::integral auto N>
static std::integral auto getNumPaths() noexcept
{
	std::size_t numPaths = 0;
	std::size_t numVisited = 0;
	std::bitset<N> grid[N] = { };
	getNumPaths<N>(grid, numPaths, numVisited, 0, 0);
	return numPaths;
}

int main()
{
	std::cout << "Input Size: " << INPUT_SIZE << std::endl;
	auto numPaths = getNumPaths<INPUT_SIZE>();
	std::cout << "Number of Paths: " << numPaths << std::endl;
	return 0;
}
