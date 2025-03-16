#include <thread> // for std::thread
#include <iostream> // for std::cout
#include <syncstream> // for std::osyncstream

namespace
{
	void do_something()
	{
		std::osyncstream(std::cout) << "Doing something...\n";
	}
	void do_something_else()
	{
		std::osyncstream(std::cout) << "Doing something else...\n";
	}
}

class background_task
{
	public:
		void operator()() const
		{
			do_something();
			do_something_else();
		}
};

int main()
{
	background_task f;
	std::thread my_thread(f);
	my_thread.join();
}
