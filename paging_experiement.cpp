/*
 Assuming page size of 4 KB, 

	Allocating 8 pages using malloc() doesn't really allocates any pages in the physical memory.

	Trying to access/write the first byte of each page one-by-one and timing the access/write reveals approximately 0.005 milliseconds of delay
	on core i5 83550U processor.

	For some reason, the first page access/write always takes only 0.001 milliseconds, maybe the malloc allocates the first page as a guarantee?

	Next, if we access the second byte or in-fact any byte within the range of the page just accessed, it takes almost 0 milliseconds of delay.

	That proves the pages are allocated lazily even if we call malloc(). The pages are materialized upon access.


	Furthermore, if we access all of the bytes across all pages using memset first, and then performing the above experiments.
	We would find that acceess/writes are taking only 0.0001 or almost 0 milliseconds.
*/


#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <string.h>

static constexpr unsigned int PAGE_SIZE = 4 * 1024;
static constexpr unsigned int NUM_PAGES = 8;

int main()
{
	std::cout << std::fixed << std::setprecision(4);

	auto start = std::chrono::steady_clock::now();
	unsigned char* blocks = (unsigned char*)malloc(PAGE_SIZE * NUM_PAGES);
	auto end = std::chrono::steady_clock::now();
	auto diff = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end - start).count();
	std::cout << (NUM_PAGES * PAGE_SIZE) << " bytes, allocation time: " << diff << std::endl;
	start = std::chrono::steady_clock::now();
//	memset(blocks, 0, PAGE_SIZE * NUM_PAGES);
	end = std::chrono::steady_clock::now();
	diff = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end - start).count();
	std::cout << "memset over " << (NUM_PAGES * PAGE_SIZE) << " bytes, time: " << diff << std::endl;

	for(int i = 0; i < NUM_PAGES; ++i)
	{
		std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
		blocks[i * PAGE_SIZE] = 0;
		auto end = std::chrono::steady_clock::now();
		auto diff = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end - start).count();
		std::cout << "Page index: " << i << ", offset = 0 , Access Time: " << diff << std::endl;
		start = std::chrono::steady_clock::now();
		if((i + 1) < NUM_PAGES)
			blocks[i * PAGE_SIZE + PAGE_SIZE] = 0;
		end = std::chrono::steady_clock::now();
		diff = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end - start).count();
		std::cout << "Page index: " << i << ", offset = 1, Access Time: " << diff << std::endl;
	}

	free(blocks);
	return 0;
}
