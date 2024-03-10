
#include "binary_tree.h"
#include "../static_queue.h"

#include <stdlib.h>
#include <string.h> // memset
#include <assert.h>

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
	if(node->parent != NULL)
	{
		if(node->parent->left == node)
			node->parent->left = NULL;
		else if(node->parent->right == node)
			node->parent->right = NULL;
	}

	if(node->left != NULL)
		node->left->parent = NULL;
	if(node->right != NULL)
		node->right->parent = NULL;

	if(data->callback != NULL)
		data->callback(node, data->userData);
	free(node);
}

void binary_node_destroy(binary_node_t* node, void (*callback)(binary_node_t* node, void* userData), void* userData)
{
	destroy_callback_data_t callback_data = { .callback = callback, .userData = userData };
	destroy_node_only(node, &callback_data);
}

void binary_tree_destroy(binary_tree_t* node, void (*callback)(binary_node_t* node, void* userData), void* userData)
{
	destroy_callback_data_t callback_data = { .callback = callback, .userData = userData };
	/* DFS traversal */
	binary_node_traverse_postorder(node, TRAVERSE_CALLBACK(destroy_node_only), &callback_data);
}


binary_node_t* binary_node_create_left(binary_node_t* node, void* satellite_data)
{
	return binary_node_set_left(node, binary_node_create(satellite_data, NULL, NULL));
}

binary_node_t* binary_node_create_right(binary_node_t* node, void* satellite_data)
{
	return binary_node_set_right(node, binary_node_create(satellite_data, NULL, NULL));
}

binary_node_t* binary_node_set_left(binary_node_t* node, binary_node_t* left)
{
	if(left->parent != NULL)
	{
		if(left->parent->left == left)
			left->parent->left = NULL;
		else if(left->parent->right == left)
			left->parent->right = NULL;
	}
	node->left = left; 
	left->parent = node;
	return left; 
}

binary_node_t* binary_node_set_right(binary_node_t* node, binary_node_t* right)
{ 
	if(right->parent != NULL)
	{
		if(right->parent->left == right)
			right->parent->left = NULL;
		else if(right->parent->right == right)
			right->parent->right = NULL;
	}
	node->right = right; 
	right->parent = node; 
	return right; 
}

void sum_callback_handler(binary_node_t* node, int* sum)
{
	*sum += binary_node_get_satellite_data_as_int(node);
}

int binary_node_get_sum_as_int(binary_node_t* node)
{
	int sum = 0;
	binary_node_traverse_inorder(node, TRAVERSE_CALLBACK(sum_callback_handler), (void*)(&sum));
	return sum;
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

void binary_node_traverse_postorder(binary_node_t* node, void (*callback)(binary_node_t* node, void* userData), void* userData)
{
	// Post order traversal:
	//  	Visits the left most first recursively
	//		Then the right recursively
	//		Then the parent of the left most recursively

	binary_node_t* left = binary_node_get_left(node);
	if(left != NULL)
		binary_node_traverse_postorder(left, callback, userData);

	binary_node_t* right = binary_node_get_right(node);
	if(right != NULL)
		binary_node_traverse_postorder(right, callback, userData);

	callback(node, userData);
}

void binary_node_traverse_inorder(binary_node_t* node, void (*callback)(binary_node_t* node, void* userData), void* userData)
{
	// In order traversal:
	// 		Visits the left most first recursively
 	// 		Then the parent of that left most recursively
 	// 		Then the right recursively

 	binary_node_t* left = binary_node_get_left(node);
 	if(left != NULL)
 		binary_node_traverse_inorder(left, callback, userData);

 	callback(node, userData);

 	binary_node_t* right = binary_node_get_right(node);
 	if(right != NULL)
 		binary_node_traverse_inorder(right, callback, userData);
}

static inline int int_max(int v1, int v2)
{
	return (v1 > v2) ? v1 : v2;
}

static bool binary_node_is_leaf(binary_node_t* node)
{
	return (binary_node_get_left(node) == NULL) && (binary_node_get_right(node) == NULL);
}

static int binary_node_get_height(binary_node_t* node)
{
	if((node == NULL) || binary_node_is_leaf(node))
		return 0;
	int left_height = (node->left != NULL) ? binary_node_get_height(node->left) : 0;
	int right_height = (node->right != NULL) ? binary_node_get_height(node->right) : 0;
	return 1 + int_max(left_height, right_height);
}

static void __binary_node_traverse_level(binary_node_t* node, int i, int* cur_level, void (*callback)(binary_node_t* node, void* userData), void* userData)
{
	if(i != *cur_level)
	{
		*cur_level += 1;
		binary_node_t* left = binary_node_get_left(node);
		if(left != NULL)
			__binary_node_traverse_level(left, i, cur_level, callback, userData);
		binary_node_t* right = binary_node_get_right(node);
		if(right != NULL)
			__binary_node_traverse_level(right, i, cur_level, callback, userData);
		*cur_level -= 1;
	}
	else callback(node, userData);
}

static void binary_node_traverse_level(binary_node_t* node, int i, void (*callback)(binary_node_t* node, void* userData), void* userData)
{
	int cur_level = 0;
	__binary_node_traverse_level(node, i, &cur_level, callback, userData);
}

void binary_node_traverse_level_order(binary_node_t* node, void (*callback)(binary_node_t* node, void* userData), void* userData)
{
	// Level order traversal:
	/* 				0
				  /  \
				 0    0
				/ \  /  \
 			   0  0 0   0

 	First solution:

	print nodes which are at level 'level':
		for each child of the current node:
			if current level is not equal to level:
				increment level
				traverse down to the tree
				decrement level
			else
				print node


 	for each level of depth [0, floor(log2(n))]
 		print nodes which are at level 'level'


 	Where is the inefficiency?
 			We are traversing the rest of the initial tree redundantly (or repeatedly) for each level printing

 	How can we eliminate that repeate work?
 			By preserving the state of the traversed nodes - use stack or queues etc.


 	Second solution:

 	 queue = 0
	
	 add root node in the queue

	 while queue is not empty:
 	 	node = pop from queue's back
 	 	print the node
 	 	for each child of the node :
 	 		add in the queue's front

 	*/

	// Time Complexity: O(n + sum{i=0, i=ceil(log2(n)), 2^i - 1})
	int height = binary_node_get_height(node);
	for(int i = 0; i <= height; i++)
		binary_node_traverse_level(node, i, callback, userData);
}

void binary_node_traverse_level_order2(binary_node_t* node, void (*callback)(binary_node_t* node, void* userData), void* userData)
{
	int height = binary_node_get_height(node);
	static_queue_t* queue = static_queue_create(sizeof(binary_node_t*), (1 << (height + 1)) - 1);

	static_queue_push(queue, &node);

	while(!static_queue_is_empty(queue))
	{
		binary_node_t* _node;
		bool result = static_queue_pop(queue, &_node);
		assert(result);
		callback(_node, userData);
		binary_node_t* left = binary_node_get_left(_node);
		if(left != NULL)
			static_queue_push(queue, &left);
		binary_node_t* right = binary_node_get_right(_node);
		if(right != NULL)
			static_queue_push(queue, &right);
	}
	static_queue_destroy(queue);
}

static void binary_node_traverse_preorder_left_edge_exclude_leaf(binary_node_t* node, void (*callback)(binary_node_t* node, void* userData), void* userData)
{
	if(!binary_node_is_leaf(node))
	{
		callback(node, userData);
		binary_node_t* left = binary_node_get_left(node);
		if(left != NULL)
			binary_node_traverse_preorder_left_edge_exclude_leaf(left, callback, userData);
		else
		{
			binary_node_t* right = binary_node_get_right(node);
			if(right != NULL)
				binary_node_traverse_preorder_left_edge_exclude_leaf(right, callback, userData);
		}
	}
}

static void binary_node_traverse_postorder_right_edge_exclude_leaf(binary_node_t* node, void (*callback)(binary_node_t* node, void* userData), void* userData)
{
	if(!binary_node_is_leaf(node))
	{
		binary_node_t* right = binary_node_get_right(node);
		if(right != NULL)
			binary_node_traverse_postorder_right_edge_exclude_leaf(right, callback, userData);
		else
		{
			binary_node_t* left = binary_node_get_left(node);
			if(left != NULL)
				binary_node_traverse_postorder_right_edge_exclude_leaf(left, callback, userData);
		}
		callback(node, userData);
	}
}

static void binary_node_traverse_leaves(binary_node_t* node, void (*callback)(binary_node_t* node, void* userData), void* userData)
{
	if(binary_node_is_leaf(node))
		callback(node, userData);
	else
	{
		binary_node_t* left = binary_node_get_left(node);
		if(left != NULL)
			binary_node_traverse_leaves(left, callback, userData);
		binary_node_t* right = binary_node_get_right(node);
		if(right != NULL)
			binary_node_traverse_leaves(right, callback, userData);
	}
}

void binary_node_traverse_boundry(binary_node_t* node, void (*callback)(binary_node_t* node, void* userData), void* userData)
{
	// Algorithm:
	// 1. Print the left most edge excluding the leaf pre-order
	// 2. Print the leaves from left to right
	// 3. Print the right most edge excluding the leaf post-order

	binary_node_traverse_preorder_left_edge_exclude_leaf(node, callback, userData);
	binary_node_traverse_leaves(node, callback, userData);
	binary_node_traverse_postorder_right_edge_exclude_leaf(node, callback, userData);
}

static void accumulate_count(binary_node_t* node, int* count)
{
	*count += 1;
}

int binary_tree_get_count(binary_tree_t* tree)
{
	if(tree == NULL)
		return 0;
	int count = 0;
	binary_node_traverse_postorder(tree, TRAVERSE_CALLBACK(accumulate_count), &count);
	return count;
}

binary_node_t* binary_search_tree_insert(binary_tree_t* tree, void* value, comparer_t compare_callback, void* userData)
{
	if(tree == NULL)
		return binary_node_create(value, NULL, NULL);

	void* this_value = binary_node_get_satellite_data(tree);
	if(compare_callback(value, this_value, userData) <= 0)
	{
		binary_node_t* left = binary_node_get_left(tree);
		if(left != NULL)
			return binary_search_tree_insert(left, value, compare_callback, userData);
		return binary_node_create_left(tree, value);
	}
	else
	{
		binary_node_t* right = binary_node_get_right(tree);
		if(right != NULL)
			return binary_search_tree_insert(right, value, compare_callback, userData);
		return binary_node_create_right(tree, value);
	}
}

static binary_node_t* binary_node_get_left_most(binary_node_t* node)
{
	if(node->left == NULL)
		return node;
	return binary_node_get_left_most(node->left);
}

static binary_node_t* binary_node_get_inorder_successor(binary_node_t* node)
{
	if(node->right == NULL)
		return node;
	if(node->right->left == NULL)
		return node->right;
	return binary_node_get_left_most(node->right->left);
}

bool binary_search_tree_remove(binary_tree_t* tree, void* value, comparer_t compare_callback, void* userData1, void (*destroyCallback)(binary_node_t*, void* userData), void* userData2, binary_node_t** new_root)
{
	binary_node_t* node = binary_search_tree_search(tree, value, compare_callback, userData1);
	if(node != NULL)
	{
		if(binary_node_is_leaf(node))
		{
			if(node->parent == NULL)
				*new_root = NULL;
			binary_node_destroy(node, destroyCallback, userData2);
		}
		else
		{
			binary_node_t* successor = binary_node_get_inorder_successor(node);
			if(successor == node)
			{
				*new_root = binary_node_get_left(node);
				binary_node_destroy(node, destroyCallback, userData2);
				return true;
			}
			void* temp = node->satellite_data;
			node->satellite_data = successor->satellite_data;
			successor->satellite_data = temp;
			if(binary_node_is_leaf(successor))
				binary_node_destroy(successor, destroyCallback, userData2);
			else
			{
				binary_node_t* right = binary_node_get_right(successor);
				assert(right != NULL);

				binary_node_t* parent = binary_node_get_parent(successor);
				if(binary_node_get_left(parent) == successor)
				{
					binary_node_set_left(parent, right);
				}
				else
				{
					assert(binary_node_get_right(parent) == successor);
					binary_node_set_right(parent, right);
				}
				binary_node_destroy(successor, destroyCallback, userData2);
			}
		}
		return true;
	}
	return false;
}

binary_node_t* binary_search_tree_search(binary_node_t* tree, void* value, comparer_t compare_callback, void* userData)
{
	void* data = binary_node_get_satellite_data(tree);
	int result = compare_callback(value, data, userData);
	if(result == 0)
		return tree;
	else if((result < 0) && (binary_node_get_left(tree) != NULL))
		return binary_search_tree_search(binary_node_get_left(tree), value, compare_callback, userData);
	else if((result > 0) && (binary_node_get_right(tree) != NULL))
		return binary_search_tree_search(binary_node_get_right(tree), value, compare_callback, userData);
	else
		return NULL;
}
