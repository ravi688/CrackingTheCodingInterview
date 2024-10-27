// Conclusion
// On some platform, unroll a loop 2 times can be faster than unrolling it 4 times. For example, I've noticed that on Intel Core i5 8320U chip
// While for other platforms, unrolling a loop 2 times, 4 times or 8 times doesn't make much difference - Noticed on Ryzen 5 5600G

#include <iostream>
#include <chrono>

static constexpr std::size_t DATA_SIZE = 10240;
static constexpr std::size_t OP_COUNT = 4;

void test1()
{
	auto start = std::chrono::steady_clock::now();
	std::array<int, DATA_SIZE * OP_COUNT> a, b, c;
	#pragma GCC unroll 4
	for(std::size_t i = 0; i < (DATA_SIZE * OP_COUNT); ++i)
	{
		c[i] = a[i] * b[i];
	}
	auto end = std::chrono::steady_clock::now();
	double elapsed = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end - start).count();
	std::cout << "test1(unroll: 4): " << elapsed << " milliseconds" << std::endl;
}


void test2()
{
	auto start = std::chrono::steady_clock::now();
	std::array<int, DATA_SIZE * OP_COUNT> a, b, c;
	#pragma GCC unroll 2
	for(std::size_t i = 0; i < (DATA_SIZE * OP_COUNT); ++i)
	{
		c[i] = a[i] * b[i];
	}
	auto end = std::chrono::steady_clock::now();
	double elapsed = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end - start).count();
	std::cout << "test1(unroll: 2): " << elapsed << " milliseconds" << std::endl;
}

void test3()
{
	auto start = std::chrono::steady_clock::now();
	std::array<int, DATA_SIZE * OP_COUNT> a, b, c;
	#pragma GCC unroll 8
	for(std::size_t i = 0; i < (DATA_SIZE * OP_COUNT); ++i)
	{
		c[i] = a[i] * b[i];
	}
	auto end = std::chrono::steady_clock::now();
	double elapsed = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end - start).count();
	std::cout << "test1(unroll: 8): " << elapsed << " milliseconds" << std::endl;
}


int main()
{
	test1();
	test2();
	test3();
	return 0;
}
