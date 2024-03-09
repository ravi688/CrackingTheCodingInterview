
#include "../binary_tree/binary_tree.h"
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

/*
 		 -1
 		/   \
 	   2     4
 	  / \   / \
 	 5   6 67  100

	Preorder: -1, 2, 5, 6, 4, 67, 100
	Inorder: 5, 6, 2, 67, 100, 4, -1
	Postorder: 5, 2, 6, -1, 67, 4, 100

*/

static binary_tree_t* build_binary_tree()
{
	binary_tree_t* n0 = binary_tree_create(create_int(-1));
		binary_node_t* n1 = binary_node_create_left(n0, create_int(2));
			binary_node_t* n3 = binary_node_create_left(n1, create_int(5));
			binary_node_t* n4 = binary_node_create_right(n1, create_int(6));
		binary_node_t* n2 = binary_node_create_right(n0, create_int(4));
			binary_node_t* n5 = binary_node_create_left(n2, create_int(67));
			binary_node_t* n6 = binary_node_create_right(n2, create_int(100));
	return n0;
}

static void print_node_as_int(binary_node_t* node, void* userData)
{
	int value = binary_node_get_satellite_data_as_int(node);
	printf("%d ", value);
}

int main(const char* argv[], int arg)
{
	binary_tree_t* tree = build_binary_tree();
	printf("PreOrder: "); binary_node_traverse_preorder(tree, TRAVERSE_CALLBACK(print_node_as_int), NULL); puts("");
	printf("InOrder: "); binary_node_traverse_inorder(tree, TRAVERSE_CALLBACK(print_node_as_int), NULL); puts("");
	printf("PostOrder: "); binary_node_traverse_postorder(tree, TRAVERSE_CALLBACK(print_node_as_int), NULL); puts("");
	binary_tree_destroy(tree, DESTROY_CALLBACK(destroy_int), NULL);
	return 0;
}
