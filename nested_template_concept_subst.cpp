#include <type_traits> // for std::add_pointer<>
#include <concepts> // for std::integral<>, and std::same_as<>
#include <vector> // for std::vector<>
#include <iostream> // for std::cout
#include <iterator> // for std::ostream_iterator<>
#include <algorithm>

template<typename T, template<typename> typename V>
concept VectorLike = requires(V<T> v)
{
	{ v.size() } -> std::integral;
	{ v.data() } -> std::same_as<typename std::add_pointer<T>::type>;
};

// NOTE: template<typename T, VectorLike<T> V> doesn't work, the (GCC) compiler throws errors.
template<typename T, template<typename> typename V> requires(VectorLike<T, V>)
static void PrintElements(const V<T>& v) noexcept
{
	std::ranges::copy(v, std::ostream_iterator<T>(std::cout, " "));
}

int main()
{
	std::vector values = { 1, 2, 3, 4, 5, 6 };
	PrintElements(values);
	return 0;
}
