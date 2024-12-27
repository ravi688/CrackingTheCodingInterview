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
 - Number of possible paths from top left to bottom right in a grid
 - AVL tree
 - Maximum SubArray sum


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
