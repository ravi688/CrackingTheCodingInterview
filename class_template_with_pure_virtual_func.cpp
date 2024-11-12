#include <iostream>

template<typename T>
class MyTemplateClass
{
	public:
		virtual void myFunction() = 0;
};

class MyDerivedClass : public MyTemplateClass<int>
{
	public:
		virtual void myFunction() override
		{
			std::cout << "myFunction from MyDerivedClass" << std::endl;
		}
};

int main()
{
	MyDerivedClass instance;
	instance.myFunction();
	return 0;
}
