
#include <stdio.h>
#include <string.h> // strlen
#include <stdlib.h> // malloc

static void swap(char* const v1, char* const v2)
{
	char t = *v1;
	*v1 = *v2;
	*v2 = t;
}

static void permute(char* str, char* string, int len)
{
	if(len <= 0)
	{
		printf("%s\n", str);
		return;
	}

	for(int i = 0; i < len; i++)
	{
		swap(&string[0], &string[i]);
		permute(str, string + 1, len - 1);
		swap(&string[0], &string[i]);
	}
}

int main(int argc, const char* argv[])
{
	const char* string = "ABCD";
	char* array = (char*)malloc(strlen(string) + 1);
	strcpy(array, string);
	array[strlen(string)] = 0;
	permute(array, array, strlen(string));
	return 0;
}