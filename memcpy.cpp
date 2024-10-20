#include <iostream>
#include <cstdlib> // for malloc
#include <limits> // for std::numerc_limits<>
#include <chrono> // for time measurement
#include <cstring> // memcpy

typedef unsigned char Byte;
typedef long int Word;
static constexpr std::size_t BUFFER_SIZE = 8192; // 8 MB


int main()
{
	Byte* buffer1 = (Byte*)malloc(BUFFER_SIZE);
	for(std::size_t i = 0; i < BUFFER_SIZE; ++i)
		buffer1[i] = i % std::numeric_limits<Byte>::max();
	Byte* buffer2 = (Byte*)malloc(BUFFER_SIZE);
	for(std::size_t i = 0; i < BUFFER_SIZE; ++i)
		buffer2[i] = 0;
	std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
	memcpy(buffer1, buffer2, BUFFER_SIZE);
	auto end = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end - start).count();
	std::cout << "memcpy: " << elapsed << " ms" << std::endl;
	start = std::chrono::steady_clock::now();
	for(std::size_t i = 0; i < BUFFER_SIZE; ++i)
		buffer1[i] = buffer2[i];
	end = std::chrono::steady_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end - start).count();
	std::cout << "byte forloop: " << elapsed << " ms" << std::endl;
	constexpr std::size_t wordSize = sizeof(Word);
	constexpr std::size_t numWords = BUFFER_SIZE / wordSize;
	Word* words1 = reinterpret_cast<Word*>(buffer1);
	Word* words2 = reinterpret_cast<Word*>(buffer2);
	start = std::chrono::steady_clock::now();
	for(std::size_t i = 0; i < numWords; ++i)
		words1[i] = words2[i];
	end = std::chrono::steady_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end - start).count();
	std::cout << "word forloop: " << elapsed << " ms" << std::endl;
	return 0;
}
