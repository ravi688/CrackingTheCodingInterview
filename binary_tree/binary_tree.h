
#include <stdio.h>

typedef struct binary_node_t binary_node_t;

typedef struct binary_node_t
{
	void* satellite_data;
	binary_node_t* left;
	binary_node_t* right;
} binary_node_t;

typedef binary_node_t binary_tree_t;

binary_tree_t* binary_tree_create();
void binary_tree_destroy(binary_tree_t* tree);

binary_node_t* binary_node_create_left(binary_node_t* node);
binary_node_t* binary_node_create_right(binary_node_t* node);
