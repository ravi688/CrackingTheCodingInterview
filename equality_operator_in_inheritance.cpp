#include <iostream> // for std::cout

class Base
{
	public:
		Base& operator=(const Base& base) noexcept
		{
			std::cout << "Base class copy assignment operator is called" << std::endl;
			return *this;
		}
};

class TestDataMember
{
	public:
		TestDataMember& operator=(TestDataMember& data) noexcept
		{
			std::cout << "TestDataMember class copy assignment operator is called" << std::endl;
			return *this;
		}
};

class Derived : public Base
{
	private:
		TestDataMember m_data;
	public:
};

int main()
{
	Derived derived;
	Derived derived2;
	derived = derived2;
	return 0;
}
