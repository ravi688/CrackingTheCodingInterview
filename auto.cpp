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

	MyClass& operator=(MyClass&) noexcept
	{
		std::cout << "MyClass operator=(MyClass&)" << std::endl;
		return *this;
	}
	const MyClass& operator=(const MyClass&) const noexcept
	{
		std::cout << "MyClass operator=(const MyClass&)" << std::endl;
		return *this;
	}
};

static const MyClass getValue() noexcept { return { }; }
static const MyClass* getValuePtr() noexcept { return new MyClass { }; }
static const int* getValueIntPtr() noexcept { return new int { 5 }; }
int main()
{
	const MyClass obj1;
	// Calls MyClass(const MyClass& obj);
	MyClass obj2 { obj1 };

	const MyClass obj4 { };
	// Calls MyClass(const MyClass&& obj);
	MyClass obj3 { std::move(obj4) };

	std::cout << "---" << std::endl;
	auto value = getValue();
	// The following two lines of code compiles fine
	const auto* value2 = getValuePtr();
	value2 = new MyClass { };
	// But the following won't compile
	// const auto value2 = getValuePtr();
	// value2 = new MyClass { };

	const auto* value3 = getValueIntPtr();
	// Doesn't compile; even with 'auto* value3'
	// *value3 = 10;
	value3 = new int { 10 };
	return 0;
}
