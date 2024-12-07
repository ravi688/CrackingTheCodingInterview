// Compiles without any issues; so we can have std::pair of references, and why shouldn't it be either!

#include <utility>

int main()
{
	int value1 = 0;
	float value2 = 0;
	std::pair<int&, float&> values { value1, value2 };
	return 0;
}
