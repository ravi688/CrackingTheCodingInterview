#include <iostream> // for std::cout
#include <cmath> // for std::ceil, std::log10, and std::pow
#include <iomanip> // for std::setw
#include <limits> // for std::numeric_limits<>
#include <initializer_list> // for std::initializer_list<>
#include <string_view> // std::string_view

static constexpr std::size_t COUNT = 100;
static constexpr std::size_t INVALID = std::numeric_limits<std::size_t>::max();

static void printTableHeader(std::initializer_list<std::pair<std::string_view, std::size_t*>> columns) noexcept
{
	for(auto& col : columns)
	{
		std::size_t width = std::max(col.first.size(), col.second ? *col.second : 0);
		if(col.second) *col.second = width;
		std::cout << " " << std::setw(width) << col.first << " |";
	}
	std::cout << "\n";
}

static std::size_t numDigits(std::size_t value) noexcept
{
	return std::log10(value + 1) + 1;
}

int main()
{
	std::size_t maxNumDigits = numDigits(COUNT);
	std::size_t maxNumDigitsCube = numDigits(std::pow(COUNT, 3));
	std::size_t col1Width = maxNumDigits;
	std::size_t col2Width = maxNumDigitsCube;
	std::size_t col3Width = maxNumDigitsCube;
	std::size_t col4Width = maxNumDigitsCube;
	std::size_t col5Width = maxNumDigitsCube;
	printTableHeader({ { "n", &col1Width },
			{ "cubes", &col2Width },
			{ "first diff", &col3Width },
			{ "second diff", &col4Width },
			{ "third diff", &col5Width } });
	std::size_t prevNCube = INVALID;
	std::size_t prevFirstDiff = INVALID;
	std::size_t prevSecondDiff = INVALID;
	for(std::size_t i = 1; i <= COUNT; ++i)
	{
		std::size_t cube = std::pow(i, 3);
		std::cout << " " << std::setw(col1Width) << i << " |"
			<< " " << std::setw(col2Width) << cube << " |";
		if(prevNCube != INVALID)
		{
			std::size_t firstDiff = cube - prevNCube;
			std::cout << " " << std::setw(col3Width) << firstDiff << " |";
			if(prevFirstDiff != INVALID)
			{
				std::size_t secondDiff = firstDiff - prevFirstDiff;
				std::cout << " " << std::setw(col4Width) << secondDiff << " |";
				if(prevSecondDiff != INVALID)
				{
					std::size_t thirdDiff = secondDiff - prevSecondDiff;
					std::cout << " " << std::setw(col5Width) << thirdDiff << " |";
				}
				else std::cout << " " << std::setw(col5Width) << "" << " |";
				prevSecondDiff = secondDiff;
			}
			else
			{
				std::cout << " " << std::setw(col4Width) << "" << " |"
					  << " " << std::setw(col5Width) << "" << " |";
			}
			prevFirstDiff = firstDiff;
		}
		else
		{
			std::cout << " " << std::setw(col3Width) << "" << " |"
				  << " " << std::setw(col4Width) << "" << " |"
				  << " " << std::setw(col5Width) << "" << " |";
		}
		prevNCube = cube;
		std::cout << "\n";
	}
	std::cout << std::endl;
	return 0;
}
