#include <iostream> // for std::cout

class UDT
{
	public:
		UDT(float value) noexcept
		{
			std::cout << "UDT converting constructor is called" << std::endl;
		}
		UDT(const UDT& obj) noexcept
		{
			std::cout << "UDT copy constructor is called" << std::endl;
		}
};

static void function(UDT udt) noexcept { }

int main()
{
	UDT udt = static_cast<UDT>(4.5f);
	function(static_cast<UDT>(5.6f));
	function(udt);
	udt = static_cast<UDT>(7.5f);
	return 0;
}
