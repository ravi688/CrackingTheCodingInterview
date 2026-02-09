#include <iostream>
#include <cctype>

char to_lowercase(char uch)
{
	return 'a' + (uch - 'A');
}

int main([[maybe_unused]] int argc, [[maybe_unused]] const char** argv)
{
TAKE_INPUT:
	std::cout << "Type a uppper case character: ";
	char uch;
	std::cin >> uch;
	if(!std::isalpha(uch) || !std::isupper(uch))
	{
			std::cout << "Invalid upper case character\n";
			goto TAKE_INPUT;
	}
	char lch = to_lowercase(uch);
	std::cout << uch << " -> " << lch << std::endl;
	return 0;
}
