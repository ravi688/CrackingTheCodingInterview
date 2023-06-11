
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // malloc

#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))

static void print_array(int* array, int length)
{
	for(int i = 0; i < length; i++)
		printf("%d ", array[i]);
	puts("");
}

static inline void swap(int* const v1, int* const v2)
{
	int temp = *v1;
	*v1 = *v2;
	*v2 = temp;
}

#define RIGHT(index) (((index + 1) << 1) - 1)
#define LEFT(index) ((index + 1) << 1)
#define PARENT(index) (((index + 1) >> 1) - 1)

static void max_heapify(int* array, int length, int index)
{
	int left = LEFT(index);
	int right = RIGHT(index);
	int swap_index = index;
	if((left < length) && (array[index] < array[left]))
	{
		swap(&array[index], &array[LEFT(index)]);
		swap_index = left;
	}
	if((right < length) && (array[index] < array[right]))
	{
		swap(&array[index], &array[RIGHT(index)]);
		swap_index = right;
	}
	if(swap_index != index)
		max_heapify(array, length, swap_index);
}

static void build_max_heap(int* array, int length)
{
	for(int i = (length / 2); i > 0; --i)
		max_heapify(array, length, i - 1);
}

static void heap_sort(int* array, int length)
{
	build_max_heap(array, length);
	for(int i = length - 1; i > 0; --i)
	{
		swap(&array[0], &array[i]);
		max_heapify(array, i, 0);
	}
}

static void selection_sort(int* array, int length)
{
	for(int i = 0; i < length; i++)
	{
		int min_index = i;
		int min_value = array[i];
		for(int j = i + 1; j < length; j++)
		{
			if(min_value > array[j])
			{
				min_value = array[j];
				min_index = j;
			}
		}
		swap(&array[i], &array[min_index]);
	}
}

static void insertion_sort(int* array, int length)
{
	for(int i = 1; i < length; i++)
	{
		int key = array[i];
		int j = i - 1;
		while((j >= 0) && (key < array[j]))
		{
			array[j + 1] = array[j];
			--j;
		}
		array[j + 1] = key;
	}
}

static void __merge_sort(int* array, int length, int* buffer)
{
	switch(length)
	{
		case 0:
		case 1:
			return;
		case 2:
			// we want to sort the array into ascending order
			if(array[0] > array[1])
				swap(&array[0], &array[1]);
			return;
		default:
			break;
	}

	int middle = length >> 1; // divide by 2
	__merge_sort(array, middle + 1, buffer);
	__merge_sort(array + middle + 1, length - (middle + 1), buffer);

	int len1 = middle + 1;
	int* arr1 = array;
	int len2 = length - (middle + 1);
	int* arr2 = array + middle + 1;

	// merge the two sorted arrays
	for(int i = 0, j = 0, k = 0; i < length; i++)
	{
		if((j < len1) && (k < len2))
		{
			if(arr1[j] < arr2[k])
				buffer[i] = arr1[j++];
			else
				buffer[i] = arr2[k++];
		}
		else if(j < len1)
			buffer[i] = arr1[j++];
		else
			buffer[i] = arr2[k++];
	}
	for(int i = 0; i < length; i++)
		array[i] = buffer[i];
}

static void merge_sort(int* array, int length)
{
	int* buffer = (int*)malloc(sizeof(int) * length);
	__merge_sort(array, length, buffer);
	free(buffer);
}

/* time complexity: T(n) = n + T(n - k) + T(k), where k < n
 * worst case: O(n^2)
 * best case: O(nlog2(n)) (binary tree like structure)
 */
static void quick_sort(int* array, int length)
{
	if(length <= 0) return;

	/* partition the array */
	int pivot = array[0];
	int j = 1;
	for(int i = 1; i < length; i++)
	{
		if(array[i] < pivot)
		{
			swap(&array[i], &array[j]);
			j++;
		}
	}
	swap(&array[j - 1], &array[0]);

	int pivot_index = j - 1;

	/* quick sort the right side parition */
	quick_sort(array + pivot_index + 1, length - (pivot_index + 1));

	/* quick sort the left side partiion */
	quick_sort(array, pivot_index);
}

static bool binary_search(int* array, int length, int value)
{
	int right = length;
	int left = 0;
	while(left <= right)
	{
		int middle = (right + left) / 2;
		if(value == array[middle])
			return true;
		else if(value > array[middle])
			left = middle + 1;
		else if(value < array[middle])
			right = middle - 1;
	}
	return false;
}

static inline int abs_int(int v1, int v2)
{
	int diff = v1 - v2;
	return (diff >= 0) ? diff : (-diff);
}

static int get_num_pairs2(int* array, int length, int diff)
{
	int num_pairs = 0;
	for(int i = 0; i < length; i++)
	{
		for(int j = i + 1; j < length; j++)
		{
			if(abs_int(array[i], array[j]) == diff)
				num_pairs++;
		}
	}
	return num_pairs;
}

static int get_num_pairs(int* array, int length, int diff)
{
	/* time complexity: O(n^2) */
	/* sort array as it that will help us improve the search algorithm from O(n) -> O(log2(n)) */
	quick_sort(array, length);


	/* time complexity: log(n - 1) + log(n - 2) + log(n - 3) + log(1) = 
						log(1) + log(2) + log(3) + ... + log(n - 1) = O((n - 1)log(n - 1))
	*/

	/* count the number of pairs */
	int num_pairs = 0;
	for(int i = 0; i < length; i++)
	{
		/* log(n - i - 1)*/
		if(binary_search(array + i + 1, length - i - 1, array[i] + diff))
			num_pairs++;
	}
	return num_pairs;
}

int main()
{
	int array[] = { 1, 7, 5, 9, 2, 12, 3 };
	int diff = 2;
	printf("Given Array: ");
	print_array(array, ARRAY_SIZE(array));
	int num_pairs = get_num_pairs(array, ARRAY_SIZE(array), diff);
	printf("Num Pairs: %d\n", num_pairs);
	return 0;
}
