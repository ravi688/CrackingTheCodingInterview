// There is an article on the internet: https://thelinuxcode.com/pipe_system_call_c/
// Which has an example containing write() and read() to the same pipe in the same main thread,
// his/her specific case works just fine (for samll data) but if you increase the data size to write into the pipe's buffer, it would block
// It is better illustrated in the following code:

#include <iostream> // for std::cout
#include <vector> // for std::vector<>
#include <stdlib.h> // for EXIT_FAILURE macro
#include <unistd.h> // for pipe()

// Tested on Ubuntu 24.04
// Pipes have limited buffer; so this size will cause write() function to block until a read() is execute over the same buffer
// In this case, read() is done in the same thread as that of write(), since read() comes after the write(),
// this process will be blocked forever.
static constexpr std::size_t SIZE = 4096 * 1024;
// The following size won't cause any blocking; probably because the pipe's buffer size is greater than or equal to 4096 bytes
// static constexpr std::size_t SIZE = 4096;

// NOTE: We can't use constexpr here, because std::vector<> internally uses loop to initialize all the elements,
// So if the number of elements exceed a certain limit the compiler complains about that when constexpr is applied.
static const std::vector<int> gData(SIZE);

int main()
{
	int ends[2];
	int ec = pipe(ends);
	if(ec == -1)
	{
		perror("Failed to create pipe");
		exit(EXIT_FAILURE);
	}
	std::cout << "Writing message to pipe..." << std::endl;
	write(ends[1], gData.data(), gData.size());

	std::vector<int> readBuffer(SIZE);
	read(ends[0], (void*)(readBuffer.data()), gData.size());
	std::cout << "Reading message complete" << std::endl;
	return 0;
}
