
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



static int compare_ints(int* value, const int* compare_value)
{
	if((*value) == (*compare_value))
		return 0;
	else if((*value) > *(compare_value))
		return 1;
	else
		return -1;
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

static void print_node_as_int(binary_node_t* node, void* userData)
{
	int value = binary_node_get_satellite_data_as_int(node);
	printf("%d ", value);
}

#define ARRAY_SIZE(__array__) sizeof(__array__) / sizeof(__array__[0])

static void* clone_int(binary_node_t* node, void* userData)
{
	return create_int(binary_node_get_satellite_data_as_int(node));
}

int main(const char* argv[], int arg)
{
	binary_tree_t* tree = build_binary_tree();
	printf("PreOrder: "); binary_node_traverse_preorder(tree, TRAVERSE_CALLBACK(print_node_as_int), NULL); puts("");
	printf("InOrder: "); binary_node_traverse_inorder(tree, TRAVERSE_CALLBACK(print_node_as_int), NULL); puts("");
	binary_tree_t* clone_tree = binary_node_clone(tree, DATA_CLONE_CALLBACK(clone_int), NULL);
	printf("InOrder2: "); binary_node_traverse_inorder2(tree, TRAVERSE_CALLBACK(print_node_as_int), NULL); puts("");
	binary_tree_destroy(clone_tree, DESTROY_CALLBACK(destroy_int), NULL);
	printf("PostOrder: "); binary_node_traverse_postorder(tree, TRAVERSE_CALLBACK(print_node_as_int), NULL); puts("");
	binary_tree_destroy(tree, DESTROY_CALLBACK(destroy_int), NULL);

	int array[] = { 5, 6, 2, 67, 100, 4, -1 };


	/*
				5 								    6
			  /   \  							  /   \
			 2     6 		 					 2     67
		    / \     \  			---->     		/  \    \
		 -1    4     67 					   -1  4    100
		              \
		               100
		
		-1 4 100

		 -1                           4                       100
		   \        Rotation        /  \     Rotation        /
		    4        ---->         -1   100  ----->         4
		     \                                             /
		     100                                          -1
	 */
	tree = build_binary_search_tree(array, ARRAY_SIZE(array));
	printf("BST InOrder: "); binary_node_traverse_inorder(tree, TRAVERSE_CALLBACK(print_node_as_int), NULL); puts("");
	
	int search_value = -1;
	if(binary_search_tree_search(tree, &search_value, COMPARE_CALLBACK(compare_ints), NULL) != NULL)
		printf("BST contains %d\n", search_value);
	else
		printf("BST doesn't contain %d\n", search_value);

	printf("BST::count: %d\n", binary_tree_get_count(tree));

	printf("Level Order: "); binary_node_traverse_level_order(tree, TRAVERSE_CALLBACK(print_node_as_int), NULL); puts("");
	printf("Level Order2: "); binary_node_traverse_level_order2(tree, TRAVERSE_CALLBACK(print_node_as_int), NULL); puts("");
	printf("Boundary Order: "); binary_node_traverse_boundry(tree, TRAVERSE_CALLBACK(print_node_as_int), NULL); puts("");
	printf("Boundary Order2: "); binary_node_traverse_boundry2(tree, TRAVERSE_CALLBACK(print_node_as_int), NULL); puts("");

	int max_width = binary_tree_get_max_width(tree);
	printf("Max Width: %d\n", max_width);

	int i = 0;
	while((tree != NULL) && (binary_tree_get_count(tree) > 0))
	{
		search_value = array[i++];
		if(binary_search_tree_remove(tree, &search_value, COMPARE_CALLBACK(compare_ints), NULL, DESTROY_CALLBACK(destroy_int), NULL, &tree))
		{
			printf("Removal of %d from BST is success\n", search_value);
		}
		else
			printf("Removal of %d from BST is failed\n", search_value);
		if(tree == NULL)
			printf("BST InOrder: <empty>\n");
		else
		{
			printf("BST InOrder: "); binary_node_traverse_inorder(tree, TRAVERSE_CALLBACK(print_node_as_int), NULL); puts("");
		}
	}

	if(tree != NULL)
		binary_tree_destroy(tree, DESTROY_CALLBACK(destroy_int), NULL);
	return 0;
}
