#include <iostream>
#include <thread> // for std::thread
#include <atomic> // for std::atomic
#include <chrono> // for timing
#include <new> // for std::hardware_destructive_interference_size

static constexpr std::size_t SUB_TARGET_COUNT = 10000;
static constexpr std::size_t THREAD_COUNT = 4;
static constexpr std::size_t RUN_COUNT = 10;

static std::atomic<std::uintptr_t> gCounter = 0;
static __attribute__((aligned(std::hardware_destructive_interference_size))) std::atomic_flag gIsDispatch = ATOMIC_FLAG_INIT;

static void incrementFunc() noexcept
{
	while(!gIsDispatch.test(std::memory_order_acquire));
	for(std::size_t i = 0; i < SUB_TARGET_COUNT; ++i)
		gCounter.fetch_add(1, std::memory_order_acq_rel);
}

int main()
{
	std::cout << "SUB_TARGET_COUNT: " << SUB_TARGET_COUNT << std::endl;
	std::cout << "THREAD_COUNT: " << THREAD_COUNT << std::endl;
	std::cout << "RUN_COUNT: " << RUN_COUNT << std::endl;

	std::cout << "Increment using " << THREAD_COUNT << " THREADS in parallel" << std::endl;
	std::vector<std::thread> threads;
	threads.reserve(THREAD_COUNT);
	for(std::size_t i = 0; i < RUN_COUNT; ++i)
	{
		threads.clear();
		gIsDispatch.clear();
		gCounter.store(0, std::memory_order_release);
		for(std::size_t j = 0; j < THREAD_COUNT; ++j)
			threads.emplace_back(incrementFunc);
		gIsDispatch.test_and_set(std::memory_order_acq_rel);
		auto start = std::chrono::steady_clock::now();
		for(std::size_t j = 0; j < THREAD_COUNT; ++j)
			threads[j].join();
		auto end = std::chrono::steady_clock::now();
		float elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - start).count();
		std::cout << "Final value: " << gCounter.load(std::memory_order_acquire) << " Elapsed: " << elapsed << " ms" << std::endl;
	}

	std::cout << "Increment using 1 thread" << std::endl;
	std::uintptr_t counter;
	for(std::size_t i = 0; i < RUN_COUNT; ++i)
	{
		counter = 0;
		auto start = std::chrono::steady_clock::now();
		for(std::size_t i = 0; i < (THREAD_COUNT * SUB_TARGET_COUNT); ++i)
			++counter;
		auto end = std::chrono::steady_clock::now();
		float elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - start).count();
		std::cout << "Final value: " << counter << " Elapsed: " << elapsed << " ms" << std::endl;
	}
	return 0;
}
