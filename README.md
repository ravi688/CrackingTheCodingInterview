# CrackingTheCodingInterview
Coding Practice for my Job interviews and personal learnings

### OS concepts
 - mmap()
 - page allocation (multiple subsequent calls to malloc())
 - mutexes
 - processes (fork())
### Hardware concepts
 - False Sharing
 - Cache hits/misses
 - TLB
 - Memory Orderings
 - Bus Faults (unaligned accesses)
### Algorithms, and Data Structures
 - Compression and Decompression using Huffman Coding
 - Nqueens problem
 - Coins problem
 - Number of possible paths from top left to bottom right in a grid
 - AVL tree
 - Maximum SubArray sum
 - Binary Tree Traversals { In-Order, Pre-Order, Post-Order }
 - Graph Traversals { DFS and BFS }
 - Linked List Intersection find, Cycle detection, Singly Reversal, and Doubly Reversal
 - Dynamic Bit List (unlike std::bitset<> which is static in size)
 - Power Set Problem (Generate subsets of a given set)
 - Binary Search Tree Validation (using Inorder Traversal, OR recursively validating each subtree, etc.)
### C++ Template Argument Deduction
#### Constness of data member variables of a const qualified class object
```C++
#include <type_traits>

struct Data
{
	int value;
};

template<typename ValueType>
static void function(ValueType& value, auto visitor)
{
	visitor(value);
}

template<typename T>
struct remove_pointer
{
	typedef T type;
};

template<typename T> struct remove_pointer<T*>
{
	typedef T type;
};

template<typename T>
struct is_refers_to_const_value
{
	static constexpr bool value = std::is_const<typename remove_pointer<typename std::remove_reference<T>::type>::type>::value;
};

template<typename T> requires(std::is_pointer<T>::value)
static void functionWrapper(T& t)
{
	// NOTE: if T is const Data*
	// then decltype(t->value) would return 'int'
	// That means decltype() doesn't care about the cv-qualification of t when t->value is passed into it!
	// Therefore we need to add const qualification based on if t refers to a constant value
	using ValueType = typename std::conditional<is_refers_to_const_value<decltype(t)>::value,
				typename std::add_const<decltype(t->value)>::type,
				decltype(t->value)>::type;
	function(t->value, [](ValueType& value) { });
}

static void myFunction()
{
	Data* data = new Data { };
	functionWrapper(data);
	const Data* const_data = data;
	functionWrapper(const_data);
}
```
#### What does std::is_const<>::value evalutes to for reference types?
```C++
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
```

### C++ Software Engineering
#### Use Warnings
gcc -Wall -Wextra -Wpedantic main.cpp -o ./main <br>
-Wpendatic compiler flag disables all the language extensions
#### Use Sanitisers
gcc -fsanitize=address,undefined -c main.cpp -o ./main.o <br>
Sanitizers must also be enbaled when linking, gcc -fsanitize=address,undefined main.o -o ./main
#### Use Static Analysis
$ clang-tidy ... <br>
$ cppcheck ...
#### Always try to qualify variables as const
OR
```C++
const auto data = [](){
 std::vector<int> result;
 // fill result with things.
 return return;
}();
```
#### Use static constexpr for values known at compile time
- This eliminates dynamic allocations
- Compiler can better optimize the code which uses the static constexpr variables
```C++
static constexpr std::array<int, 5> angles{-90, -45, 0, 45, 90};
// instead of
// static const std::vector<int> angles{-90, -45, 0, 45, 90};
```
#### Always use auto to avoid conversions and memory allocations
```C++
std::string_view getValue() { return "Hello World"; }
std::string str = getValue(); // std::string would be instantiated here -> dynamic memory allocation
```
NOTE: Also
```C++
// Returns pointer to constant data
const MyClass* getValuePtr() { return new MyClass { }; }
// Type of value1 is 'const MyClass*'
auto value1 = getValuePtr();
// Type of value2 is still 'const MyClass*'
auto* value2 = getValuePtr();
// Type of value3 is still 'const MyClass*'
const auto* value3 = getValuePtr();
// Type of value4 is 'const MyClass* const'
const auto value4 = getValuePtr();
```
#### Always use range based for loop over Old loops
- Avoids type conversions (int to std::size_t or vice-versa)
- Looks compact
- Use clang-tidy's modernize-loop-convert check
- NOTE: I've done experiments to demonstrates how bit-width and signness conversions in integers affect performance
```C++
for(const auto& element : container) { }
// int to std::size_t conversion (or its opposte maybe)
for(int i = 0; i < container.size(); ++i) { }
```
#### Use auto in ranged for loops
NOTE: std::map<std::string, int>::value_type equals std::pair<const std::string, int>, if you use std::pair<std::string, int>, even just reference, it would create temporary object!
```C++
// Conversion from double to int
for(const int value : container_of_double) { }
// Accidental Slicing
for(const base value : container_of_derived) { }
// No problem
for(const auto& value: container) { }

class Stack { public: Data pop() { ... return value; } };
// This would avoid extra copy which may happen otherwise from temporary to 'value'
for(auto&& value : StackPopIterator(myStack))
```
#### Prefer declaractive sytle (<algorithm>) algorithms
```C++
const auto has_value = std::any_of(begin(container), end(container), greater_than(12));
// C++20
const auto has_value = std::any_of(container, greater_than(12));
```
#### Use Copy-Paste Detector (CPD)
https://github.com/pmd/pmd <br>
In arch linux, you can install it directly. Package name is 'pmd', the tool name is 'pmd-cpd'
#### Do not use default in switch case statements
Do the following instead:
```C++
enum class Values
{
  Value1;
  Value2;
  Value3;
};

std::string_view get_name(Values value)
{
 switch(value)
 {
  case Value1: return "Value1";
  case Value2: return "Value2";
 } // unhandled enum value warning now
 return "unknown";
}
```
#### De-template-ize your generic code
- Move things out of the template code if possible
- This reduces binary size and compile times
#### Internal and External APIs [ Optional ]
- One may allow unchecked input to be passed into internal API functions
- But check every input in external API functions and produce diagnostic messages
#### Use [[nodiscard]] liberally
- It is a C++17 attribute that tells the compiler to warn if a return value is ignored.
- It can be used on functions or the types itself
- C++20 adds the ability to provide a description
```C++
#include <stdexcept>
#include <concepts>
[[nodiscard]] auto divide(std::integral auto numerator, std::integral auto denominator) {
 // is integer division
 if (denominator == 0) {
   throw std::runtime_error("divide by 0!");
 }
  return numerator / denominator;
}
[[nodiscard]] auto divide(auto numerator, auto denominator) {
  // is floating point division
  return numerator / denominator;
}
```
#### Consider deleting problematic conversions
```C++
double high_precision_thing(double);
double high_precision_thing(float) = delete;
```
#### Use Build Generators (for portability)
- Cmake
- Meson
- Bazel
#### Use Package Managers (for dep management)
- pkg-config
- conan
- vcpkg
#### Use Multiple Compilers
- Each compiler does different analyses and implements the standard slightly different way.
