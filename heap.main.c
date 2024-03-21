#include "heap.h"

#include <stdio.h>


#define PRINT_ARRAY(array) print_array(array, sizeof(array) / sizeof((array)[0]))
void print_array(int* array, int count)
{
	for(int i = 0; i < count; i++)
		printf("%d ", array[i]);
	if(count > 0)
		puts("");
}

static int compare_ints(int* value, const int* compare_value)
{
	if((*value) == (*compare_value))
		return 0;
	else if((*value) > *(compare_value))
		return 1;
	else
		return -1;
}

int main(int argc, const char* argv[])
{
	int array[12] = { -1, -3, 0, 2, 1, 7, 8, -9, 10, -11, 34, -56 };
	heap_t* heap = heap_create(array, sizeof(int), 12, sizeof(array) / sizeof(int));
	printf("Original: "); PRINT_ARRAY(array);
	heap_build(heap, COMPARE_CALLBACK(compare_ints), NULL);
	printf("After build heap: "); PRINT_ARRAY(array);
	heap_sort(heap, COMPARE_CALLBACK(compare_ints), NULL);
	printf("After heap sort: "); PRINT_ARRAY(array);
	heap_remove(heap, &array[7], COMPARE_CALLBACK(compare_ints), NULL);
	printf("After remove @[7]: "); PRINT_ARRAY(array);
	heap_destroy(heap);
	return 0;
}
