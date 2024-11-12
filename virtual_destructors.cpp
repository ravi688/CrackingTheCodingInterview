#include <iostream>

class Base1
{
	public:
		virtual ~Base1() noexcept
		{
			std::cout << "From Base 1" << std::endl;
		}
};

class Base2 : public Base1
{
	public:
		~Base2() noexcept
		{
			std::cout << "From Base 2" << std::endl;
		}
};

class Derived : public Base2
{
	public:
 		~Derived() noexcept
		{
			std::cout << "From Derived" << std::endl;
		}
	
};

int main()
{
	Base1* instance = new Derived();
	delete instance;
	return 0;
}
