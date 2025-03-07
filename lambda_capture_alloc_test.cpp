// This code (if run under valgrind memcheck), proves that capture lambdas do not allocate memory per se
// However, if any of the objects being captured (by value) has a (copy) constructor which does allocates memory,
// then only capturing them in lambdas may lead to memory allocations.

// Compile this code by running:
// g++ -std=c++20 -Wall -Wextra -Wpedantic -O0 lambda_capture_alloc_test.cpp -o ./main
// Run the executable under valgrind memcheck:
// valgrind --tool=memcheck ./main

struct MyType
{
	float value = 5.0f;
};

int main()
{
	MyType myObj;
	auto myLambda = [&myObj]() -> float { return myObj.value; };
	[[maybe_unused]] auto value = myLambda();
	(void)(value);
}
