#include <iostream>

template<typename... Args>
static constexpr auto getNumTypes() noexcept
{
	return sizeof...(Args);
}

template<typename Arg, typename... Args>
struct sizeof_all
{
	static constexpr auto value = sizeof(Arg) + sizeof_all<Args...>::value;
};

// Why can't we just specialize for zero Arguments?
// That's because, the compiler uses the original template for syntax error detection and diagnostic messages
// It is quite possible, but it is the compiler which is not allowing it.
// Therefore, we instead specialize for a single argument
template<typename Arg>
struct sizeof_all<Arg>
{
	static constexpr auto value = sizeof(Arg);
};


int main()
{
	auto numTypes = getNumTypes<int, float, double>();
	std::cout << "sizeof...(int, float, double): " << numTypes << std::endl;
	auto size = sizeof_all<int, float, double>();
	std::cout << "total size of (int, float, double): " << size << std::endl;
	return 0;
}
