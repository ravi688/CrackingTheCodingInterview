#include <map>
#include <string>

static std::map<std::string, int> getMap()
{
	return { };
}

int main()
{
	auto map = getMap();
	using ElementType = std::pair<std::string, int>;
	// Produces warning about temporary creation
	for(const ElementType &value : map)
	{
	}
	// Doesn't create temporaries
	for(const std::map<std::string, int>::value_type &value : map)
	{
	}
	return 0;
}
