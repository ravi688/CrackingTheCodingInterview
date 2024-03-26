
#include "../binary_tree/binary_tree.h"
#include <stdlib.h>

static int* create_int(int value)
{
	int* ptr = (int*)malloc(sizeof(int));
	*ptr = value;
	return ptr;
}

static void build(binary_tree_t* n0)
{
	n0->satellite_data = create_int(-1);

	binary_node_t* n1 = binary_node_create_left(n0); n1->satellite_data = create_int(2);
	binary_node_t* n2 = binary_node_create_right(n0); n2->satellite_data = create_int(4);

	binary_node_t* n3 = binary_node_create_left(n1); n3->satellite_data = create_int(5);
	binary_node_t* n4 = binary_node_create_right(n1); n4->satellite_data = create_int(6);
	binary_node_t* n5 = binary_node_create_left(n2); n5->satellite_data = create_int(67);
	binary_node_t* n6 = binary_node_create_right(n2); n6->satellite_data = create_int(100);
}

static void inorder(binary_tree_t* tree)
{
	binary_node_t* current = tree->root;
	
}

int main(const char* argv[], int arg)
{
	binary_tree_t* tree = binary_tree_create();
	build(tree);
	inorder(tree);
	binary_tree_destroy(tree);
	return 0;
}
