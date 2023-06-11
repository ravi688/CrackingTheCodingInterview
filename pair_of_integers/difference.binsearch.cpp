
#include <iostream>


template<typename T, size_t LENGTH>
static void print_array(T (&array)[LENGTH])
{
	for(size_t i = 0; i < LENGTH; i++)
		std::cout << array[i] << " ";
	std::cout << std::endl;
}

template<typename T>
static void swap(T& v1, T& v2)
{
	T temp = v1;
	v1 = v2;
	v2 = temp;
}

template<typename T>
static void __quick_sort(T* array, size_t length)
{
	if(length <= 1)
		return;

	// paritition
	T pivot = array[0];
	size_t j = 1;
	for(size_t i = 1; i < length; i++)
	{
		if(pivot > array[i])
		{
			swap(array[i], array[j]);
			j++;
		}
	}
	swap(array[j - 1], array[0]);

	size_t pivot_index = j - 1;

	__quick_sort<T>(array, pivot_index);
	__quick_sort<T>(array + pivot_index + 1, length - (pivot_index + 1));
}

template<typename T, size_t LENGTH>
static inline void quick_sort(T (&array)[LENGTH])
{
	__quick_sort(array, LENGTH);
}

template<typename T>
static bool binary_search(T* array, size_t length, const T& value)
{
	int left = 0;
	int right = length;
	while(left <= right)
	{
		int middle = (left + right) / 2;
		if(array[middle] == value)
			return true;
		else if(value < array[middle])
			right = middle - 1;
		else
			left = middle + 1;
	}
	return false;
}

template<typename T, size_t LENGTH>
static size_t get_num_pairs(T (&array)[LENGTH], int diff)
{
	quick_sort<T, LENGTH>(array);

	size_t num_pairs = 0;

	for(size_t i = 0; i < LENGTH; i++)
	{
		if(binary_search(array + i + 1, LENGTH - (i + 1), array[i] + diff))
			num_pairs++;
	}

	return num_pairs;
}


int main()
{
	int array[] = { 1, 7, 5, 9, 2, 12, 3 };
	int diff = 2;
	std::cout << "Given Array: ";
	print_array(array);
	int num_pairs = get_num_pairs(array, diff);
	std::cout << "Num Pairs: " << num_pairs;
	return 0;
}
