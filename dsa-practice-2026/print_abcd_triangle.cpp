#include <iostream>

void print_abcd_triangle(char max = 'O')
{
	char ch = 'A';
	for(int i = 0; ; ++i)
	{
		for(int j = 0; j < (i + 1); ++j, ++ch)
		{
			if(ch > max)
			{
				std::cout << "\n";
				return;
			}
			std::cout << ch << " ";
		}
		std::cout << "\n";
	}
}

int main(int argc, const char** argv)
{
	print_abcd_triangle();
	return 0;
}
