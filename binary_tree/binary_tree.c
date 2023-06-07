
#include "binary_tree.h"

#include <stdlib.h>
#include <string.h> // memset

static binary_node_t* create_node()
{
	binary_node_t* node = (binary_node_t*)malloc(sizeof(binary_node_t));
	memset(node, 0, sizeof(binary_node_t));
	return node;
}

static void destroy_node(binary_node_t* node)
{
	free(node);
}

binary_tree_t* binary_tree_create()
{
	return create_node();
}

void binary_tree_destroy(binary_tree_t* tree)
{
	if(tree == NULL) return;
	binary_tree_destroy(tree->left);
	binary_tree_destroy(tree->right);
	destroy_node(tree);
}

binary_node_t* binary_node_create_left(binary_node_t* node)
{
	return (node->left = create_node());
}

binary_node_t* binary_node_create_right(binary_node_t* node)
{
	return (node->right = create_node());
}
