// Running the executable should not print "Copy constructor is called" as we are passing a pair of references.
// Run it as:
// g++ -std=c++20 pair_of_reference_passing_as_parameters.cpp -o ./main

#include <utility> // for std::pair<>
#include <vector> // for std::vector<>
#include <iostream> // for std::cout

struct MyData {
	int data1; float data2;
	
	MyData(int _data1, float _data2) : data1(_data1), data2(_data2) { }
	MyData(const MyData&) noexcept
	{
		std::cout << "Copy constructor is called" << std::endl;
	}
};

static void my_function_call(std::vector<std::pair<MyData&, MyData&>> dataList)
{
	(void)(dataList);
}


int main()
{

	MyData data1 { 4, 5 };
	MyData data2 { 56, 6.7f };
	my_function_call({ { data1, data2 } });

	return 0;
}
