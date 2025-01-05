#include <map> // for std::multimap<>
#include <iostream>
#include <string> // for std::string

int main()
{
	std::multimap<int, std::string> map;
	map.insert({ 4, "A"});
	map.insert({ 5, "B"});
	map.insert({ 10, "C"});
	map.insert({ 11, "D"});
	map.insert({ -1, "F"});
	for(const auto& pair : map)
		std::cout << "{ " << pair.first << ", " << pair.second << " }\n";
	std::cout << std::flush;
	return 0;
}
