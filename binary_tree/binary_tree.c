
#include "binary_tree.h"

#include <stdlib.h>
#include <string.h> // memset

binary_node_t* binary_node_create(void* satellite_data, binary_node_t* left, binary_node_t* right)
{
	binary_node_t* node = (binary_node_t*)malloc(sizeof(binary_node_t));
	memset(node, 0, sizeof(binary_node_t));
	node->satellite_data = satellite_data;
	node->left = left;
	node->right = right;
	if(left != NULL)
		left->parent = node;
	if(right != NULL)
		right->parent = node;
	return node;
}

typedef struct destroy_callback_data_t
{
	void (*callback)(binary_node_t* node, void* userData);
	void* userData;
} destroy_callback_data_t;

static void destroy_node_only(binary_node_t* node, destroy_callback_data_t* data)
{
	if(data->callback != NULL)
		data->callback(node, data->userData);
	free(node);
}

void binary_node_destroy(binary_node_t* node, void (*callback)(binary_node_t* node, void* userData), void* userData)
{
	destroy_callback_data_t callback_data = { .callback = callback, .userData = userData };
	/* DFS traversal */
	binary_node_traverse_inorder(node, TRAVERSE_CALLBACK(destroy_node_only), &callback_data);
}


binary_node_t* binary_node_create_left(binary_node_t* node, void* satellite_data)
{
	return binary_node_set_left(node, binary_node_create(satellite_data, NULL, NULL));
}

binary_node_t* binary_node_create_right(binary_node_t* node, void* satellite_data)
{
	return binary_node_set_right(node, binary_node_create(satellite_data, NULL, NULL));
}

void sum_callback_handler(binary_node_t* node, int* sum)
{
	*sum += binary_node_get_satellite_data_as_int(node);
}

int binary_node_get_sum_as_int(binary_node_t* node)
{
	int sum = 0;
	binary_node_traverse_inorder(node, TRAVERSE_CALLBACK(sum_callback_handler), (void*)(&sum));
}

void binary_node_traverse_preorder(binary_node_t* node, void (*callback)(binary_node_t* node, void* userData), void* userData)
{
	// Pre order traversal:
	//		Visits the parent first recursively
	// 		Then the left recursively
	// 		Then the right recursively
	
	callback(node, userData);

	binary_node_t* left = binary_node_get_left(node);
	if(left != NULL)
		binary_node_traverse_preorder(left, callback, userData);

	binary_node_t* right = binary_node_get_right(node);
	if(right != NULL)
		binary_node_traverse_preorder(right, callback, userData);
}

void binary_node_traverse_inorder(binary_node_t* node, void (*callback)(binary_node_t* node, void* userData), void* userData)
{
	// In order traversal:
	//  	Visits the left most first recursively
	//		Then the right recursively
	//		Then the parent of the left most recursively

	binary_node_t* left = binary_node_get_left(node);
	if(left != NULL)
		binary_node_traverse_inorder(left, callback, userData);

	binary_node_t* right = binary_node_get_right(node);
	if(right != NULL)
		binary_node_traverse_inorder(right, callback, userData);

	callback(node, userData);
}

void binary_node_traverse_postorder(binary_node_t* node, void (*callback)(binary_node_t* node, void* userData), void* userData)
{
	// Post order traversal:
	// 		Visits the left most first recursively
 	// 		Then the parent of that left most recursively
 	// 		Then the right recursively

 	binary_node_t* left = binary_node_get_left(node);
 	if(left != NULL)
 		binary_node_traverse_postorder(left, callback, userData);

 	callback(node, userData);

 	binary_node_t* right = binary_node_get_right(node);
 	if(right != NULL)
 		binary_node_traverse_postorder(right, callback, userData);
}

void binary_search_tree_insert(binary_tree_t* tree, void* value, int (*compare)(void* value, void* compare_value))
{
	
}
