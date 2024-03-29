#include "red_black_tree.h"

#include <stdlib.h> // for malloc
#include <string.h> // for memcpy and memset

rb_node_t* rb_node_create(void* value, rb_node_t* left, rb_node_t* right)
{
	rb_node_t* node = (rb_node_t*)malloc(sizeof(rb_node_t));
	memset(node, 0, sizeof(rb_node_t));
	node->satellite_data = value;
	node->left = left;
	node->right = right;
	return node;
}

rb_node_t* rb_node_insert(rb_node_t* root, void* value, comparer_t compare, void* userData)
{
	if(root == NULL)
	{
		rb_node_t* node = rb_node_create(value, NULL, NULL);
		// Red Black Tree Property: Root is always black
		node->color = COLOR_BLACK;
		return node;
	}
	if(compare(value, root->satellite_data, userData) > 0)
	{
		rb_node_t* right = root->right;
		if(right == NULL)
		{
			rb_node_t* node = rb_node_create(value, NULL, NULL);
			root->right = node;
			return node;
		}
		return rb_node_insert(right, value, compare, userData);
	}
	else
	{
		rb_node_t* left = root->left;
		if(left == NULL)
		{
			rb_node_t* node = rb_node_create(value, NULL, NULL);
			root->left = node;
			return node;
		}
		return rb_node_insert(left, value, compare, userData);
	}
}

rb_node_t* rb_node_remove(rb_node_t* root, void* value, comparer_t compare, void* userData)
{

}

rb_node_t* rb_node_search(rb_node_t* root, void* value, comparer_t compare, void* userData)
{

}

binary_node_t* rb_node_to_binary_node(rb_node_t* node, int data_size)
{
	// Solution no 1:
	// This problem resembles to cloning a binary tree
	//
	// convert(node):
	//		left = node->left
	//		bin_left = null
	//		if left != null:
	//			bin_left = convert(left)
	//		right = node->right
	//		bin_right = null
	//		if right != null:
	//			bin_right = convert(right)
	//		data = null
	//		if node->data != null:
	//			data = malloc(data_size)
	//			memcpy(data, node->data, data_size)
	//		return binary_node_create(data, bin_left, bin_right)

	rb_node_t* left = node->left;
	binary_node_t* bin_left = NULL;
	if(left != NULL)
		bin_left = rb_node_to_binary_node(left, data_size);
	rb_node_t* right = node->right;
	binary_node_t* bin_right = NULL;
	if(right != NULL)
		bin_right = rb_node_to_binary_node(right, data_size);
	void* data = NULL;
	if(node->satellite_data	!= NLL)
	{
		data = malloc(data_size);
		memcpy(data, node->satellite_data, data_size);
	}
	return binary_node_create(data, bin_left, bin_right);
}
