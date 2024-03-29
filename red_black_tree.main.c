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

static int compare_ints(int* value, const int* compare_value)
{
	if((*value) == (*compare_value))
		return 0;
	else if((*value) > *(compare_value))
		return 1;
	else
		return -1;
}

static void print_node_as_int(binary_node_t* node, void* userData)
{
	int value = binary_node_get_satellite_data_as_int(node);
	printf("%d ", value);
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

// very ineffective - not a red black tree balancing
static void binary_node_balance(binary_node_t* node)
{
	// Solution no 1:
	//
	// balance(node):
	//		if node == null:
	//			return
	// 	 	left = node->left
	//		if left != null:
	//			balance(left)
	//		right = node->right
	//		if right != null:
	//			balance(right)
	//		
	//		if node->right != null && node->right->left == null 
	//			&& node->right->right != null && node->right->right->left != null
	// 			&& node->right->right->right != null:
	//			t = node->right
	//			node->right = node->right->right
	//			node->right->left = t
	//
	//		if node->left != null && node->left->right == null
	//			&& node->left->left != null && node->left->left->right != null
	//			&& node->left->left->left != null:
	//			t = node->left
	//			node->left = node->left->left
	//			node->left->right = t

	if(node == NULL)
		return;
	binary_node_t* left = node->left;
	binary_node_balance(left);
	binary_node_t* right = node->right;
	binary_node_balance(right);

	if((node->right != NULL) && (node->right->left == NULL)
		&& (node->right->right != NULL) && (node->right->right->left == NULL)
		&& (node->right->right->right != NULL))
	{
		binary_node_t* t = node->right;
		node->right = node->right->right;
		binary_node_t* t2 = node->right->left;
		node->right->left = t;
		t->right = t2;
	}

	if((node->left != NULL) && (node->left->right == NULL)
		&& (node->left->left != NULL) && (node->left->left->right == NULL)
		&& (node->left->left->left != NULL))
	{
		binary_node_t* t = node->left;
		node->left = node->left->left;
		binary_node_t* t2 = node->left->right;
		node->left->right = t;
		t->left = t2;
	}
}

int main(int argc, const char* argv[])
{
	int array[12] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	binary_tree_t* tree = build_binary_search_tree(array, 12);
	puts("Before Balancing: \n");
	binary_node_dump(tree, TRAVERSE_CALLBACK(print_node_as_int), NULL);

	/*

		1                            2                      3                 3                 3
		 \                          / \                    / \               / \               / \
		  2                        1   3                  2   4             2   4             2   6
		   \                            \                /     \           /     \           /   / \
		    3              --->          4        --->  1       5  --->   1       6   --->  1   5   7
		     \                            \                      \               / \           /
		      4                            5                      6             5   7         4
		       \                            \                      \
		        5                            6                      7
		         \                            \
		          6                            7
		           \
		            7

			1             2
		   / \   --->    / \
		  b   2         1   a
			 / \       /
			e   3     b
			   / \
			  d   a
	*/


	binary_node_balance(tree);

	puts("After Balancing: \n");
	binary_node_dump(tree, TRAVERSE_CALLBACK(print_node_as_int), NULL);


	return 0;
}
