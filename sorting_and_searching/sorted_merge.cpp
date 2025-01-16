#include <iostream> // for std::cout
#include <array> // for std::array<>
#include <iterator> // for std::ostream_iterator<>

// We have given two sorted arrays A, and B. A is large enough to also hold B.
// How can we merge B into A and the resulting A must be sorted?

template<typename T, std::size_t ALength, std::size_t BLength>
static void sortedMerge(std::array<T, ALength>& a, std::size_t lastA, std::array<T, BLength>& b, std::size_t lastB)
{
	std::size_t indexA = lastA;
	std::size_t indexB = lastB;
	std::size_t mergedIndex = lastA + lastB;
	while(indexA > 0 && indexB > 0)
	{
		if(a[indexA - 1] > b[indexB - 1])
		{
			a[mergedIndex - 1] = a[indexA - 1];
			--indexA;
		}
		else
		{
			a[mergedIndex - 1] = b[indexB - 1];
			--indexB;
		}
		--mergedIndex;
	}
	while(indexA > 0)
		a[mergedIndex--] = a[indexA--];
	while(indexB > 0)
		b[mergedIndex--] = b[indexB--];
}

template<typename T, std::size_t Length>
static std::ostream& operator<<(std::ostream& stream, const std::array<T, Length>& v)
{
	stream << "{ ";
	std::copy(v.begin(), v.end(), std::ostream_iterator<T>(stream, ", "));
	stream << " }";
	return stream;
}

int main()
{
	std::array<int, 10> a { 3, 4, 5, 67 };
	std::array<int, 6> b { 100, 200, 300, 400, 500, 600 };
	std::cout << "Input: \n";
	std::cout << "A = " << a << "\n";
	std::cout << "B = " << b << "\n";
	sortedMerge(a, 4, b, 6);
	std::cout << "Output: \n";
	std::cout << "A = " << a << "\n";
	return 0;
}
