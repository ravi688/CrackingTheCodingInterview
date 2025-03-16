#include <thread> // for std::thread
#include <syncstream> // for std::osyncstream
#include <iostream> // for std::cout
#include <cassert> // for assert()

class scoped_thread final
{
	private:
		std::thread m_thread;
	public:
		scoped_thread(std::thread& thread) : m_thread(std::move(thread)) { }
		scoped_thread(std::thread&& thread) : m_thread(std::move(thread)) { }
		~scoped_thread()
		{
			assert(m_thread.joinable());
			m_thread.join();
		}
};

static void myThreadFunction()
{
	std::osyncstream(std::cout) << "Thread Started, id: " << std::this_thread::get_id() << std::endl;
	std::size_t counter = 0;
	for(std::size_t i = 0; i < 10000000; ++i) ++counter;
	std::osyncstream(std::cout) << "Thread Finished" << std::endl;
}

int main()
{
	std::thread myThread(myThreadFunction);
	{
		scoped_thread st(myThread);
	}
	{
		scoped_thread st { std::thread(myThreadFunction) };
	}
	std::cout << "Master thread" << std::endl;
}
