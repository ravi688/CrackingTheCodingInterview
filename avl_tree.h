#pragma once

// Self Balancing Tree : AVL Tree
// Description: Self-Balancing Trees are height-balanced binary search trees that automatically 
// keep the height as small as possible when insertion and deletion operations are performed on the trees.
// AVL tree have a property that the difference between the heights of left and right subtrees can't be more 
// than one. This property is ensured/maintain by rotations.

#include "binary_tree/binary_tree.h"

// forward declaration for avl_node_t struct
typedef struct avl_node_t avl_node_t;

// type definition of avl_node_t
typedef struct avl_node_t
{
	void* satellite_data;
	avl_node_t* left;
	avl_node_t* right;
	avl_node_t* parent;
} avl_node_t;

// Constructors and Destructors
avl_node_t* avl_node_create(void* value, avl_node_t* left, avl_node_t* right, avl_node_t* parent);
void* avl_node_destroy(avl_node_t* node);

// Algorithms
avl_node_t* avl_node_insert(avl_node_t* root, void* value, comparer_t compare, void* userData);
avl_node_t* avl_node_remove(avl_node_t* root, void* value, comparer_t compare, void* userData);
avl_node_t* avl_node_search(avl_node_t* root, void* value, comparer_t compare, void* userData);
binary_node_t* avl_node_to_binary_node(avl_node_t* node, int data_size);
