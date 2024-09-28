#include <atomic>
#include <iostream>
#include <thread>
#include <chrono>

class spinlock_mutex
{
private:
	std::atomic_flag m_flag;
public:
	spinlock_mutex() noexcept : m_flag(ATOMIC_FLAG_INIT) { }
	void lock() noexcept
	{
		while(m_flag.test_and_set(std::memory_order_acquire))
		{
			; // spin while test_and_set returns true
		};
	}
	void unlock() noexcept
	{
		m_flag.clear(std::memory_order_relaxed);
	}
};

static spinlock_mutex spinMutex;
static int data = 0;

static void myFunction1() noexcept
{
	spinMutex.lock();
	std::cout << "Thread ID: " << std::this_thread::get_id() << ", data = " << data << std::endl;
	data = 100;
	spinMutex.unlock();
}

static void myFunction2() noexcept
{
	spinMutex.lock();
	std::cout << "Thread ID: " << std::this_thread::get_id() << ", data = " << data << std::endl;
	data = 200;
	spinMutex.unlock();
}

int main()
{	
	std::thread thread1(myFunction1);
	std::thread thread2(myFunction2);
	thread1.join();
	thread2.join();
	return 0;
}
