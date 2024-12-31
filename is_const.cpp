#include <type_traits>

struct Data
{
	char __bytes[255];
};

static void function()
{
	const Data value { };
	const Data& dataRef = value;
	static_assert(std::is_same<decltype(dataRef), const Data&>::value);
	// std::is_const<> to reference types always returns false as we can't qualify a reference type as 'const'
	// NOTE: std::is_const<> tells whether the type is const qualified or not, it doesn't tell the intrinsic constness!
	static_assert(std::is_const<decltype(dataRef)>::value); // it fails here

	// Also
	// std::is_const<const int*>::value or std::is_const<int const*>::value would evaluate to false
	// And std::is_const<int * const>::value or std::is_const<const int* const>::value would evaluate to true
}
