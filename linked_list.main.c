#include "linked_list.h"
#include <stdlib.h> // malloc

static int* create_int(int value)
{
	int* ptr = (int*)malloc(sizeof(int));
	*ptr = value;
	return ptr;
}

static void destroy_int(linked_list_node_t* node, void* userData)
{
	free(linked_list_node_get_satellite_data(node));
}

static void print_int(linked_list_node_t* node, void* userData)
{
	printf("%d ", *(int*)node->satellite_data);
}

int main(int argc, const char* argv[])
{
	linked_list_t* list = linked_list_create(create_int(5));
	linked_list_node_create_next(
		linked_list_node_create_next(
			linked_list_node_create_next(
				linked_list_node_create_next(
					linked_list_node_create_next(
						linked_list_node_create_next(list,
							create_int(-100)),
						create_int(-5)),
				create_int(10)),
			create_int(500)),
		create_int(30)),
	create_int(-40));

	linked_list_node_traverse(list, print_int, NULL);

	linked_list_destroy(list, destroy_int, NULL);
	return 0;
}