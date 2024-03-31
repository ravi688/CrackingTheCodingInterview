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

static inline int int_max(int v1, int v2)
{
	return (v1 > v2) ? v1 : v2;
}

static int get_height(avl_node_t* node)
{
	if(node == NULL)
		return 0;
	int left_height = get_height(node->left);
	int right_height = get_height(node->right);
	return int_max(left_height, right_height) + 1;
}

static avl_node_t* rotate_left(avl_node_t* node)
{
	if(node->parent != NULL)
	{
		if(node->parent->left == node)
			node->parent->left = node->right;
		else
			node->parent->right = node->right;
	}
	if(node->right != NULL)
	{
		node->right->parent = node->parent;
		avl_node_t* t1 = node->right->left;;
		node->right->left = node;
		node->parent = node->right;
		avl_node_t* t = node->right;
		node->right = t1;
		if(node->right != NULL)
			node->right->parent = node;
		return t;
	}
	return node;
}

static avl_node_t* rotate_right(avl_node_t* node)
{
	if(node->parent != NULL)
	{
		if(node->parent->left == node)
			node->parent->left = node->left;
		else
			node->parent->right = node->left;
	}
	if(node->left != NULL)
	{
		node->left->parent = node->parent;
		avl_node_t* t1 = node->left->right;
		node->left->right = node;
		node->parent = node->left;
		avl_node_t* t = node->left;
		node->left = t1;
		if(node->left != NULL)
			node->left->parent = node;
		return t;
	}
	return node;
}

// An AVL tree ensures that for a particular node the height of its one of the subtrees is always differ by 1 or less from its other subtree
avl_node_t* avl_node_insert(avl_node_t* node, void* value, comparer_t compare, void* userData)
{
	// Solution no 1
	//
	//  get_height(node):
	//		if node == null:
	//			return 0
	//		left_height = get_height(node->left)
	//		right_height = get_height(node->right)
	//		return max(left_height, right_height) + 1
	//
	//	rotate_left(node)
	//		if node->parent != null::
	//			if node->parent->left == node:
	//				node->parent->left = node->right
	//			else:
	//				node->parent->right = node->right
	//		if node->right != null:
	//			node->right->parent = node->parent
	//			t = node->right->left
	//			node->right->left = node
	//			node->parent = node->right
	//			node->right = t
	//			if node->right->left != null:
	//				node->right->left->parent = node
	//
	//	rotate_right(node):
	//		if node->parent != null:
	//			if node->parent->left == node:
	//				node->parent->left = node->left
	//			else:
	//				node->parent->right = node->left
	//		if node->left != null:
	//			node->left->parent = node->parent
	//			t = node->left->right
	//			node->left->right = node
	//			node->parent = node->left
	//			node->left = node->left->right
	//			if node->left->right != null:
	//				node->left->right->parent = node
	//
	//	avl_node_insert(node, value):
	//		if node->value < value:
	//			if node->right == null:
	//				node->right = create_avl_node(value)
	//				node->right->parent = node
	//				return
	//			else
	//				avl_node_insert(node->right, value)
	//		else:
	//			if node->left == null:
	//				node->left = create_avl_node(value)
	//				node->left->parent = node
	//				return
	//			else:
	//				avl_node_insert(node->left, value)
	//
	//		diff = get_height(node->left) - get_height(node->right)
	//		if diff < -1:
	//			rotate_left(node)
	//		else if diff > 1:
	//			rotate_right(node)

	if(node == NULL)
	{
		node = avl_node_create(value, NULL, NULL, NULL);
		return node;
	}

	if(compare(value, node->satellite_data, userData) > 0)
	{
		if(node->right == NULL)
		{
			node->right = avl_node_create(value, NULL, NULL, node);
			return node;
		}
		else
			node->right = avl_node_insert(node->right, value, compare, userData);
	}
	else
	{
		if(node->left == NULL)
		{
			node->left = avl_node_create(value, NULL, NULL, node);
			return node;
		}
		else
			node->left = avl_node_insert(node->left, value, compare, userData);
	}

	int diff = get_height(node->left) - get_height(node->right);
	if(diff < -1)
		node = rotate_left(node);
	else if(diff > 1)
		node = rotate_right(node);

	return node;
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
