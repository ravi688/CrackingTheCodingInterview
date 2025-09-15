#include <iostream>

#include <chrono>
#include <cstring>

static void test_memcpy()
{
	uint32_t value = 432424;
	float f;
	auto start = std::chrono::steady_clock::now();
	std::memcpy(&f, &value, sizeof(uint32_t));
	auto end = std::chrono::steady_clock::now();
	auto diff = end - start;
	auto elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::nano>>(diff).count();
	std::cout << "test_memcpy elapsed: " << elapsed << std::endl;
	(void)f;
}

static void test_reinterpret_cast()
{
	uint32_t value = 432424;
	float f;
	auto start = std::chrono::steady_clock::now();
	f = *reinterpret_cast<float*>(&value);
	auto end = std::chrono::steady_clock::now();
	auto diff = end - start;
	auto elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::nano>>(diff).count();
	std::cout << "test_reinrepret_cast elapsed: " << elapsed << std::endl;
	(void)f;
}


int main()
{
	uint32_t i = 50;
	while(i > 0)
	{
		test_memcpy();
		test_reinterpret_cast();
		--i;
	}
	return 0;
}
