#include <iostream>
#include <pthread.h> // for posix threading interface
#include <chrono> // for timing

// ec: error code
#define CHECK_ERR(ec) do { if(ec) { perror("(error occurred)"); exit(EXIT_FAILURE); } } while(false)
static pthread_mutex_t gStdOutMutex = PTHREAD_MUTEX_INITIALIZER;

static void* thread1Function(void* args)
{
	pthread_mutex_lock(&gStdOutMutex);
	std::cout << "From thread1 function" << std::endl;
	pthread_mutex_unlock(&gStdOutMutex);
	return 0;
}

static void* thread2Function(void* args)
{
	pthread_mutex_lock(&gStdOutMutex);
	std::cout << "From thread2 function" << std::endl;
	pthread_mutex_unlock(&gStdOutMutex);
	return 0;
}

int main()
{
	pthread_t thread1, thread2;

	auto start = std::chrono::steady_clock::now();
	CHECK_ERR(pthread_create(&thread1, /* default attributes */ NULL, thread1Function, NULL));
	auto end = std::chrono::steady_clock::now();
	auto elapsed1 = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - start).count();
	// NOTE: we can't print the elapsed time for thread1 creation on stdout because it might allocate some heap memory
	// resulting messing with memory pages and errors in the time measurement.
	start = std::chrono::steady_clock::now();
	CHECK_ERR(pthread_create(&thread2, /* default attributes */ NULL, thread2Function, NULL));
	end = std::chrono::steady_clock::now();
	auto elapsed2 = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - start).count();

	pthread_mutex_lock(&gStdOutMutex);
	std::cout << "Thread 1 creation time: " << std::fixed << std::setprecision(3) << elapsed1 << " ms" << std::endl;
	std::cout << "Thread 2 creation time: " << std::fixed << std::setprecision(3) << elapsed2 << " ms" << std::endl;
	pthread_mutex_unlock(&gStdOutMutex);

	CHECK_ERR(pthread_join(thread1, NULL));
	CHECK_ERR(pthread_join(thread2, NULL));

	return 0;
}
