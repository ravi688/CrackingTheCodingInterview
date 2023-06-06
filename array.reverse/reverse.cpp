
#include <iostream>


template<typename T_type, int T_length>
static void print_array(const T_type (&array)[T_length])
{
	for(int i = 0; i < T_length; i++)
		std::cout << array[i] << " ";
	std::cout << std::endl;
}

template<typename T_type>
static void swap(T_type& v1, T_type& v2)
{
	T_type temp = v1;
	v1 = v2;
	v2 = temp;
}

template<typename T_type, int T_length>
static void reverse(T_type (&array)[T_length])
{
	int half_length = T_length >> 1;
	for(int i = 0; i < half_length; i++)
		swap(array[i], array[T_length - i - 1]);
}

int main()
{
	int array[] = { 4, 5, 6, -2, 3, -5, 56, 7 };
	std::cout << "Original Array: \n";
	print_array(array);
	reverse(array);
	std::cout << "Reversed Array: \n";
	print_array(array);

	return 0;
}
