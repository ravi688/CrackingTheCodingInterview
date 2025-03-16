#include <thread> // for std::thread
#include <stdexcept> // for std::runtime_error()
#include <concepts> // for std::integer concept
#include <iostream> // for std::cerr
#include <syncstream> // for std::osyncstream

static void do_something_in_current_thread()
{
	throw std::runtime_error("Runtime error has been occurred");
}

template<std::integral IType>
static void use_integer(IType& value)
{
	value += 1;
	value -= 1;
}

struct func
{
	int& m_value;
	func(int& value) : m_value(value) { }

	void operator()()
	{
		for(std::size_t i = 0; i < 100000000; ++i)
			use_integer(m_value);
		std::osyncstream(std::cout) << "Thread ID: " << std::this_thread::get_id() << ", Finished processing" << std::endl;
	}
};

static void f()
{
	int value = 3;
	func myFunc(value);
	std::thread myThread(myFunc);
	try
	{
		do_something_in_current_thread();
	}
	catch(...)
	{
		myThread.join();
		throw;
	}
	myThread.join();
}

int main()
{
	try
	{
		f();
	}
	catch(std::exception& except)
	{
		std::cerr << except.what() << std::endl;
	}
}
