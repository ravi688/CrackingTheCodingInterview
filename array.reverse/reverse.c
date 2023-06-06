
#include <stdio.h>

#define ARRAY_SIZE(array) (sizeof(array)) / sizeof((array)[0])

static void swap(int* const v1, int* const v2)
{
	int temp = *v1;
	*v1 = *v2;
	*v2 = temp;
}

static void reverse(int* const array, int size)
{
	int half_size = size >> 1;
	for(int i = 0; i < half_size; i++)
		swap(&array[i], &array[size - i - 1]);
}

static void print_array(int* const array, int size)
{
	for(int i = 0; i < size; i++)
		printf("%d ", array[i]);
}

int main()
{
	int array[] = { 100, 200, 400, 500, -45, 35, -1, -2 };

	puts("Original Array: ");
	print_array(array, ARRAY_SIZE(array));
	puts("");
	reverse(array, ARRAY_SIZE(array));
	puts("Reversed Array: ");
	print_array(array, ARRAY_SIZE(array));
	puts("");
	return 0;
}