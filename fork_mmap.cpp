#include <iostream>
#include <string_view> // for std::string_view
#include <cstring> // for std::memcpy()
#include <cassert> // for std::assert()
#include <thread> // for std::this_thread
#include <chrono> // for std::chrono
#include <cstdlib> // for std::rand

#include <unistd.h> // for fork() system call
#include <sys/mman.h> // for mmap() and munmap() system calls
#include <sys/wait.h> // for waitpid() system call

static constexpr std::string_view gData =
"Hello World!\n"
"This is the data sent from the parent process to the child process\n"
"If you can read and understand this, that means it is not corrupted\n"
"And data transfer has been successfull!\n";

int main()
{
	unsigned char* sharedBuffer = static_cast<unsigned char*>(mmap(NULL, sizeof(bool) + gData.size(), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0));
	if(sharedBuffer == MAP_FAILED)
	{
		std::cerr << "Error: Failed to created shared memory region" << std::endl;
		return -1;
	}
	pid_t childPID = fork();
	// Parent code
	if(childPID != 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 300));
		// populate the data first
		std::memcpy(sharedBuffer + sizeof(bool), gData.data(), gData.size());
		// then notify the child process that data is available
		// generally basic types are guaranteed to be atomic
		reinterpret_cast<bool*>(sharedBuffer)[0] = true;
		int status;
		pid_t pid = waitpid(childPID, &status, 0);
		assert(pid == childPID);
		if(status != 0)
			std::cerr << "Warn: The child process has exited with non-zero return code" << std::endl;
		return 0;
	}
	// Child code
	else
	{
		bool* flag = reinterpret_cast<bool*>(sharedBuffer);
		// wait for the flag to be raised
		while(!flag[0])
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		// now interpret the data and print to stdout
		std::string_view str { reinterpret_cast<char*>(sharedBuffer + sizeof(bool)), gData.size() };
		std::cout << str << std::endl;
		return 0;
	}
	return 0;
}
