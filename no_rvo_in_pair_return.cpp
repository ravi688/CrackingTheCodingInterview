#include <iostream>

class MyClass
{
	public:
		MyClass() noexcept
		{
			std::cout << "Default Constructor is called" << std::endl;
		}
		MyClass(const MyClass& obj) noexcept
		{
			std::cout << "Copy Constructor is called" << std::endl;
		}
		MyClass(MyClass&& obj) noexcept
		{
			std::cout << "Move Constructor is called" << std::endl;
		}
		~MyClass() noexcept
		{
			std::cout << "Destructor is called" << std::endl;
		}
};

static std::pair<MyClass, float> test() noexcept
{
	MyClass obj;
	// Move constructor of MyClass is called, thus avoiding copy
	return { std::move(obj), 56.0f };
	// If commented out, then Copy constructor of MyClass is called, which will be inefficient
	// return { obj, 56.0f };
}

int main()
{
	auto value = test();
	return 0;
}
