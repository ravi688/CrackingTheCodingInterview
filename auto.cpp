#include <iostream>

struct MyClass
{
	MyClass() noexcept
	{
		std::cout << "MyClass default constructor" << std::endl;
	}
	MyClass(const MyClass& obj) noexcept
	{
		std::cout << "MyClass const copy-constructor" << std::endl;
	}
	MyClass(MyClass& obj) noexcept
	{
		std::cout << "MyClass copy-constructor" << std::endl;
	}
	MyClass(const MyClass&& obj) noexcept
	{
		std::cout << "MyClass const move-constructor" << std::endl;
	}
	MyClass(MyClass&& obj) noexcept
	{
		std::cout << "MyClass move-constructor" << std::endl;
	}
	~MyClass() noexcept
	{
		std::cout << "MyClass destructor" << std::endl;
	}
};


int main()
{
	const MyClass obj1;
	// Calls MyClass(const MyClass& obj);
	MyClass obj2 { obj1 };

	const MyClass obj4 { };
	// Calls MyClass(const MyClass&& obj);
	MyClass obj3 { std::move(obj4) };
	return 0;
}
