#include "avl_tree.h"

#include <stdlib.h> // for malloc
#include <string.h> // for memcpy and memset

avl_node_t* avl_node_create(void* value, avl_node_t* left, avl_node_t* right, avl_node_t* parent)
{
	avl_node_t* node = (avl_node_t*)malloc(sizeof(avl_node_t));
	memset(node, 0, sizeof(avl_node_t));
	node->satellite_data = value;
	node->left = left;
	node->right = right;
	if(left != NULL)
		left->parent = node;
	if(right != NULL)
		right->parent = node;
	node->parent = parent;
	return node;
}

void* avl_node_destroy(avl_node_t* node)
{
	void* ptr = node->satellite_data;
	free(node);
	return ptr;
}

avl_node_t* avl_node_insert(avl_node_t* root, void* value, comparer_t compare, void* userData)
{
	return NULL;
}

binary_node_t* avl_node_to_binary_node(avl_node_t* node, int data_size)
{
	// clone the left avl node to the binary node
	avl_node_t* left = node->left;
	binary_node_t* left_clone = NULL;
	if(left != NULL)
		left_clone = avl_node_to_binary_node(left, data_size);

	// clone the right avl node to the binary node
	avl_node_t* right = node->right;
	binary_node_t* right_clone = NULL;
	if(right != NULL)
		right_clone = avl_node_to_binary_node(right, data_size);

	// clone the node's value
	void* data_clone = NULL;
	if(node->satellite_data != NULL)
	{
		data_clone = malloc(data_size);
		memcpy(data_clone, node->satellite_data, data_size);
	}

	// create the binary node clone of this avl node 
	return binary_node_create(data_clone, left_clone, right_clone);
}
