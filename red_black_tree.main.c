#include "red_black_tree.h"

#include <stdlib.h>

static int* create_int(int value)
{
	int* ptr = (int*)malloc(sizeof(int));
	*ptr = value;
	return ptr;
}

static void destroy_int(binary_node_t* node, void* userData)
{
	free(binary_node_get_satellite_data(node));
}

static binary_tree_t* build_binary_search_tree(const int* array, int array_size)
{
	int i = 0;
	binary_node_t* root = binary_search_tree_insert(NULL, create_int(array[i++]), COMPARE_CALLBACK(compare_ints), NULL);
	while(i < array_size)
	{
		(void)binary_search_tree_insert(root, create_int(array[i]), COMPARE_CALLBACK(compare_ints), NULL);
		++i;
	}
	return root;
}


int main(int argc, const char* argv[])
{
	int array[12] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	binary_tree_t* tree = build_binary_search_tree(array, 12);
	return 0;
}
