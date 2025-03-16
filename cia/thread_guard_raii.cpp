#include <thread> // for std::thread
#include <syncstream> // for std::osyncstream
#include <iostream> // for std::cout

class thread_guard final
{
	private:
		std::thread& m_thread;
	public:
		thread_guard(std::thread& thread) : m_thread(thread) { }
		~thread_guard()
		{
			if(m_thread.joinable())
				m_thread.join();
		}
};

static void myThreadFunction()
{
	std::osyncstream(std::cout) << "Thread Started" << std::endl;
	std::size_t counter = 0;
	for(std::size_t i = 0; i < 10000000; ++i) ++counter;
	std::osyncstream(std::cout) << "Thread Finished" << std::endl;
}

int main()
{
	std::thread myThread(myThreadFunction);
	thread_guard tg(myThread);
}
