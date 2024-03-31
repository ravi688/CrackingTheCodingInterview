#include "red_black_tree.h"

#include <stdlib.h> // for malloc
#include <string.h> // for memcpy and memset

rb_node_t* rb_node_create(void* value, rb_node_t* left, rb_node_t* right, rb_node_t* parent)
{
	rb_node_t* node = (rb_node_t*)malloc(sizeof(rb_node_t));
	memset(node, 0, sizeof(rb_node_t));
	node->satellite_data = value;
	node->color = COLOR_RED;
	node->left = left;
	node->right = right;
	if(left != NULL)
		left->parent = node;
	if(right != NULL)
		right->parent = node;
	node->parent = parent;
	return node;
}

void* rb_node_destroy(rb_node_t* node)
{
	void* ptr = node->satellite_data;
	free(node);
	return ptr;
}

// Properties of a Red Black Tree
// 1. Root node is always black
// 2. Leaves are always black (NIL)
// 3. If a node is red then its parent is black
// 4. Number of black nodes from a node to any of its descendent leaves are the same
// NOTE: if a node has a single child then it must a red child!
// NOTE: Every perfect binary tree that consists only of black nodes is a red-black tree
// There are two types of voilations possible while inserting or deleting nodes:
//		1. Red voilation, no two adjacent nodes can have red color
//		2. Black voilation, every simple path from a node to any of its descendants must have the same number of black nodes
rb_node_t* rb_node_insert2(rb_node_t* node, void* value, comparer_t compare, void* userData)
{
	// Solution no 1
	//
	// rb_node_insert(node, value):
	//		if node is null:
	//			node = create_node(value)
	//			node->color = black
	//			node->parent = null
	//			return node
	//		if node->value < value:
	//			if node->right == null:
	//				node->right = create_node(value)
	//				node->right->color = red
	//				node->right->parent = node
	//				return node
	//			else:
	//				node->right = rb_node_insert(&node->right, value)
	//				node->right->parent = node
	//		else:
	//			if node->left == null:
	//				node->left = create_node(value)
	//				node->left->color = red
	//				node->left->parent = node
	//				return node
	//			else:
	//				node->left = rb_node_insert(&node->left, value)
	//				node->left->parent = node
	//
	// 		if node->color == black:
	//			if node->right && node->right->color == red:
	//				parent = node->parent
	//				if node->right->right && node->right->right->color == red:
	//					if parent:
	//						if parent->left == node:
	//							parent->left = node->right
	//						else:
	//							parent->right = node->right
	//					node->right->parent = parent
	//					node->right->left = node
	//					node->parent = node->right
	//					node->parent->color = black
	//					node->color = red
	//					return node->parent
	//				else if node->right->left && node->right->left->color == red:
	//					t = node->right
	//					node->right = node->right->left
	//					node->right->left->parent = node
	//					node->right->right = t
	//					t->parent = node->right
	//					if parent:
	//						if parent->left = node:
	//							parent->left = node->right
	//						else:
	//							parent->right = node->right
	//					node->right->parent = parent
	//					node->right->left = node
	//					node->parent = node->right
	//					node->parent->color = black
	//					node->parent->left = red
	//					return node->parent
	//			else if node->left && node->left->color == red:
	//				if node->left->left && node->left->left->color == red:
	//					if parent:
	//						if parent->left == node:
	//							parent->left = node->left
	//						else:
	//							parent->right = node->left
	//					node->left->parent = parent
	//					node->left->right = node
	//					node->parent = node->left
	//					node->parent->color = black
	//					node->color = red
	//					return node->parent
	//				else if node->left->right && node->left->right->color == red:
	//					t = node->left
	//					node->left = node->left->right
	//					node->left->right->parent = node
	//					node->left->left = t
	//					t->parent = node->left
	//					if parent:
	//						if parent->left = node:
	//							parent->left = node->left
	//						else:
	//							parent->right = node->left
	//					node->left->parent = parent
	//					node->left->right = node
	//					node->parent = node->left
	//					node->parent->color = black
	//					node->color = red
	//					return node->parent

}

rb_node_t* rb_node_insert(rb_node_t* node, void* value, comparer_t compare, void* userData)
{
	if(node == NULL)
	{
		rb_node_t* new_node = rb_node_create(value, NULL, NULL, NULL);
		// Red Black Tree Property: Root is always black
		new_node->color = COLOR_BLACK;
		return new_node;
	}
	else if(compare(value, node->satellite_data, userData) > 0)
	{
		rb_node_t* right = node->right;
		if(right == NULL)
		{
			rb_node_t* new_node = rb_node_create(value, NULL, NULL, node);
			node->right = new_node;
			return node;
		}
		else
		{
			// if right child is not null, then delegate the insertion to the right child/subtree
			node->right = rb_node_insert(right, value, compare, userData);
			node->right->parent = node;
		}
	}
	else
	{
		rb_node_t* left = node->left;
		if(left == NULL)
		{
			rb_node_t* new_node = rb_node_create(value, NULL, NULL, node);
			node->left = new_node;
			return node;
		}
		else
		{
			// if left child is not null, then delegate the insertion to the left child/subtree
			node->left = rb_node_insert(left, value, compare, userData);
			node->left->parent = node;
		}
	}
	if (node->color == COLOR_BLACK)
	{
		rb_node_t* parent = node->parent;
		if(node->left && node->right && (parent == NULL))
		{
			if((node->left->color == COLOR_RED) && (node->right->color == COLOR_RED))
			{
				node->left->color = COLOR_BLACK;
				node->right->color = COLOR_BLACK;
				return node;
			}
		}
		if (node->right && (node->right->color == COLOR_RED))
		{
			if (node->right->right && (node->right->right->color == COLOR_RED))
			{
				if (parent)
				{
					if (parent->left == node)
						parent->left = node->right;
					else
						parent->right = node->right;
				}
				node->right->parent = parent;
				rb_node_t* t = node->right->left;
				node->parent = node->right;
				node->right->left = node;
				node->right = t;
				if(t != NULL)
					t->parent = node;
				node->parent->color = COLOR_BLACK;
				node->color = COLOR_RED;
				return node->parent;
			}
			else if (node->right->left && (node->right->left->color == COLOR_RED))
			{
				rb_node_t* t = node->right;
				node->right = node->right->left;
				node->right->left->parent = node;
				node->right->right = t;
				t->parent = node->right;
				if (parent)
				{
					if (parent->left = node)
						parent->left = node->right;
					else
						parent->right = node->right;
				}
				node->right->parent = parent;
				t = node->right->left;
				node->parent = node->right;
				node->right->left = node;
				node->right = t;
				if(t != NULL)
					t->parent = node;
				node->parent->color = COLOR_BLACK;
				node->color = COLOR_RED;
				return node->parent;
			}
		}
		else if (node->left && (node->left->color == COLOR_RED))
		{
			if (node->left->left && (node->left->left->color == COLOR_RED))
			{
				if (parent)
				{
					if (parent->left == node)
						parent->left = node->left;
					else
						parent->right = node->left;
				}
				node->left->parent = parent;
				rb_node_t* t = node->left->right;
				node->parent = node->left;
				node->left->right = node;
				node->left = t;
				if(t != NULL)
					t->parent = node;
				node->parent->color = COLOR_BLACK;
				node->color = COLOR_RED;
				return node->parent;
			}
			else if (node->left->right && (node->left->right->color == COLOR_RED))
			{
				rb_node_t* t = node->left;
				node->left = node->left->right;
				node->left->right->parent = node;
				node->left->left = t;
				t->parent = node->left;
				if (parent)
				{
					if (parent->left = node)
						parent->left = node->left;
					else
						parent->right = node->left;
				}
				node->left->parent = parent;
				t = node->left->right;
				node->parent = node->left;
				node->left->right = node;
				node->left = t;
				if(t != NULL)
					t->parent = node;
				node->parent->color = COLOR_BLACK;
				node->color = COLOR_RED;
				return node->parent;
			}
		}
	}
	return node;
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
	if(node->satellite_data	!= NULL)
	{
		data = malloc(data_size);
		memcpy(data, node->satellite_data, data_size);
	}
	return binary_node_create(data, bin_left, bin_right);
}
