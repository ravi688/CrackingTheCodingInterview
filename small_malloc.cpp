// The first spike happens at [0], second happens at [4], and the third happens at [12] and the pattern follows for 4KB after [12]
// This demonstrates that OS allocates a page only if it is needs extra, otherwise uses the same page for small enough subsequent allocations.
// The first 2 spikes can be explain as that the OS might have allocated pages for the text section and data section etc of the program at load time.
// So, it used the existing pages?

// Output on Ubuntu Intel Core i5 8350U, Page size 4196 Bytes
// [ 0]TotalBytes:    512, elapsed: 0.004517
// [ 1]TotalBytes:   1024, elapsed: 0.000740
// [ 2]TotalBytes:   1536, elapsed: 0.000280
// [ 3]TotalBytes:   2048, elapsed: 0.000200
// [ 4]TotalBytes:   2560, elapsed: 0.009040
// [ 5]TotalBytes:   3072, elapsed: 0.000325
// [ 6]TotalBytes:   3584, elapsed: 0.000269
// [ 7]TotalBytes:   4096, elapsed: 0.000193
// [ 8]TotalBytes:   4608, elapsed: 0.000189
// [ 9]TotalBytes:   5120, elapsed: 0.000189
// [10]TotalBytes:   5632, elapsed: 0.000192
// [11]TotalBytes:   6144, elapsed: 0.000194
// [12]TotalBytes:   6656, elapsed: 0.008188
// [13]TotalBytes:   7168, elapsed: 0.000216
// [14]TotalBytes:   7680, elapsed: 0.000187
// [15]TotalBytes:   8192, elapsed: 0.000192
// [16]TotalBytes:   8704, elapsed: 0.000187
// [17]TotalBytes:   9216, elapsed: 0.000191
// [18]TotalBytes:   9728, elapsed: 0.000195
// [19]TotalBytes:  10240, elapsed: 0.000202
// [20]TotalBytes:  10752, elapsed: 0.006947
// [21]TotalBytes:  11264, elapsed: 0.000254
// [22]TotalBytes:  11776, elapsed: 0.000194
// [23]TotalBytes:  12288, elapsed: 0.000192
// [24]TotalBytes:  12800, elapsed: 0.000188
// [25]TotalBytes:  13312, elapsed: 0.000190
// [26]TotalBytes:  13824, elapsed: 0.000191
// [27]TotalBytes:  14336, elapsed: 0.006977
// [28]TotalBytes:  14848, elapsed: 0.000213
// [29]TotalBytes:  15360, elapsed: 0.000207
// [30]TotalBytes:  15872, elapsed: 0.000191
// [31]TotalBytes:  16384, elapsed: 0.000188


#include <iostream>
#include <cstdlib> // for std::malloc
#include <chrono> // for timing
#include <cstring> // for std::memset

static constexpr std::size_t ITERATION_COUNT = 32;
static constexpr std::size_t CHUNK_SIZE = 512; // 1 Byte

int main()
{
	std::size_t totalBytes = 0;
	for(std::size_t i = 0; i < ITERATION_COUNT; ++i)
	{
		std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
		unsigned char* bytes = static_cast<unsigned char*>(malloc(CHUNK_SIZE));
		std::memset(bytes, 0, CHUNK_SIZE);
		auto end = std::chrono::steady_clock::now();
		float elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - start).count();
		totalBytes += CHUNK_SIZE;
		std::cout << "[" << std::setw(2) << i << "]" << "TotalBytes: " << std::setw(6) << totalBytes << ", elapsed: " << std::fixed << elapsed << std::endl;
	}
}
