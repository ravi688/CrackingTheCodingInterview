// Conclusion:
//	Data aligned to the cache line boundry avoids false sharing situation
//	Also, higher generation CPUs doesn't necessarly mean that they give more performance.
//	For example, I tested this code on intel Core i3 8350U, it took around 2 milliseconds for test1(),
//			and on Core i5 12400, it took around 2 milliseconds for test1() - same (infact sometimes it took longer).


#include <iostream>
#include <thread>
#include <new>
#include <cstdint>
#include <atomic>

static constexpr std::size_t ITERATION_COUNT = 10000;
static constexpr std::size_t TEST_RUN_COUNT = 10;

struct Data1
{
	std::atomic<std::uint32_t> value1;
	std::atomic<std::uint32_t> value2;
} __attribute__((aligned(std::hardware_destructive_interference_size)));

static Data1 gData1;

struct Data2
{
	__attribute__((aligned(std::hardware_destructive_interference_size))) std::atomic<std::uint32_t> value1;
	__attribute__((aligned(std::hardware_destructive_interference_size))) std::atomic<std::uint32_t> value2;
} __attribute__((aligned(std::hardware_destructive_interference_size)));

static Data2 gData2;

static void test1() noexcept
{
	auto start = std::chrono::steady_clock::now();
	std::thread thread1([](Data1& data1) noexcept
	{
		for(std::size_t i = 0; i < ITERATION_COUNT; ++i)
			data1.value1.fetch_add(1);
	}, std::ref(gData1));
	std::thread thread2([](Data1& data1) noexcept
	{
		for(std::size_t i = 0; i < ITERATION_COUNT; ++i)
			data1.value2.fetch_add(1);
	}, std::ref(gData1));
	thread1.join();
	thread2.join();
	auto end = std::chrono::steady_clock::now();
	float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - start).count();
	std::cout << "test1: " << elapsedTime << std::endl;
}

static void test2() noexcept
{
	auto start = std::chrono::steady_clock::now();
	std::thread thread1([](Data2& data2) noexcept
	{
		for(std::size_t i = 0; i < ITERATION_COUNT; ++i)
			data2.value1.fetch_add(1);
	}, std::ref(gData2));
	std::thread thread2([](Data2& data2) noexcept
	{
		for(std::size_t i = 0; i < ITERATION_COUNT; ++i)
			data2.value2.fetch_add(1);
	}, std::ref(gData2));
	thread1.join();
	thread2.join();
	auto end = std::chrono::steady_clock::now();
	float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - start).count();
	std::cout << "test2: " << elapsedTime << std::endl;
}

int main()
{
	std::cout << "hardware concurrency: " << std::thread::hardware_concurrency() << std::endl;
	std::cout << "destructive interference size: " << std::hardware_destructive_interference_size << std::endl;
	std::cout << "sizeof(std::atomic<std::uint32_t>): " << sizeof(std::atomic<std::uint32_t>) << std::endl;

	gData1.value1.store(0);
	gData1.value2.store(0);

	gData2.value1.store(0);
	gData2.value2.store(0);

	for(std::size_t i = 0; i < TEST_RUN_COUNT; ++i)
	{
		std::cout << "Run: " << i << std::endl;
		test1();
		test2();
	}
	return 0;
}
