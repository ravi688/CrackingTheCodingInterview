#pragma once

// Self Balancing Tree : Red-Black Tree
// Description: Self-Balancing Trees are height-balanced binary search trees that automatically 
// keep the height as small as possible when insertion and deletion operations are performed on the trees.
// RB Trees take advantage of the property of BST to make rotations and eventually balance the tree.

#include "binary_tree/binary_tree.h"
#include <stdint.h>

// type definition of color_t
typedef enum color_t : uint8_t
{
	COLOR_RED,
	COLOR_BLACK
} color_t;

// forward declaration for rb_node_t struct
typedef struct rb_node_t rb_node_t;

// type definition of rb_node_t
typedef struct rb_node_t
{
	void* satellite_data;
	color_t color;
	rb_node_t* left;
	rb_node_t* right;
} rb_node_t;

#ifdef COMPARE_CALLBACK
#	undef COMPARE_CALLBACK
#endif
#define COMPARE_CALLBACK(callback) (comparer_t)(callback)
typedef int (*comparer_t)(rb_node_t* value, rb_node_t* cmp_value, void* userData);

rb_node_t* rb_node_insert(rb_node_t* root, void* value, comparer_t compare, void* userData);
rb_node_t* rb_node_delete(rb_node_t* root, void* value, comparer_t compare, void* userData);
rb_node_t* rb_node_search(rb_node_t* root, void* value, comparer_t compare, void* userData);
binary_node_t* rb_node_to_binary_node(rb_node_t* node, int data_size);