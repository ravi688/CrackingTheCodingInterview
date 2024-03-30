#pragma once

// Self Balancing Tree : Red-Black Tree
// Description: Self-Balancing Trees are height-balanced binary search trees that automatically 
// keep the height as small as possible when insertion and deletion operations are performed on the trees.
// RB Trees take advantage of the property of BST to make rotations and eventually balance the tree.
// The height of a RB Tree can be no more than 2log2(n + 1)
// One can convince himself that an ordinary complete or perfect binary tree has height of log2(n + 1)
// And an RB tree can have height of one subtree twice that of the another subtree at the same name level
// So the maximum height of a RB tree can be 2 x log2(n + 1). 
// Fact: Height of a non-perfect and non-complete binary tree is can be greater than a perfect or complete binary tree!
//
// Properties of a Red-Black Tree
// 1. Root node is black
// 2. All leaves (NIL) are black
// 3. Every simple path starting but not including from a node to any of the descendent leaves must have the same number of black nodes
// 4. If a node is red then its parent is black
// 5. Each node is either red or black


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
	rb_node_t* parent;
} rb_node_t;

// Constructors and Destructors
rb_node_t* rb_node_create(void* value, rb_node_t* left, rb_node_t* right, rb_node_t* parent);
void* rb_node_destroy(rb_node_t* node);

// Algorithms
rb_node_t* rb_node_insert(rb_node_t* root, void* value, comparer_t compare, void* userData);
rb_node_t* rb_node_remove(rb_node_t* root, void* value, comparer_t compare, void* userData);
rb_node_t* rb_node_search(rb_node_t* root, void* value, comparer_t compare, void* userData);
binary_node_t* rb_node_to_binary_node(rb_node_t* node, int data_size);
