#include <iostream>

class Base
{
	public:
		Base() noexcept
		{
			virtualFunction();
		}

		virtual void virtualFunction() noexcept
		{
			std::cout << "Virtual Function from Base" << std::endl;
		}
};

class Derived : public Base
{
	public:
		virtual void virtualFunction() noexcept override
		{
			std::cout << "Virtual Function from Derived" << std::endl;
		}
};

int main()
{
	Derived derived { };
	std::cout << "Separate Line---" << std::endl;
	derived.virtualFunction();
	return 0;
}
