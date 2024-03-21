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


static int compare_ints(int* value, const int* compare_value)
{
	if((*value) == (*compare_value))
		return 0;
	else if((*value) > *(compare_value))
		return 1;
	else
		return -1;
}


int main(int argc, const char* argv[])
{
	linked_list_t* list = linked_list_create(create_int(5));
	linked_list_node_insert_front(
		linked_list_node_insert_front(
			linked_list_node_insert_front(
				linked_list_node_insert_front(
					linked_list_node_insert_front(
						linked_list_node_insert_front(list,
						create_int(-100)),
					create_int(-5)),
				create_int(10)),
			create_int(500)),
		create_int(30)),
	create_int(-40));

	printf("Before reverse: \n"); linked_list_node_traverse(list, print_int, NULL); puts("");
	list = linked_list_node_reverse(list);
	printf("After reverse: \n"); linked_list_node_traverse(list, print_int, NULL); puts("");
	linked_list_node_insert_front(list, create_int(55));
	printf("After insert_front: \n"); linked_list_node_traverse(list, print_int, NULL); puts("");
	linked_list_node_insert_after(list, create_int(-5), create_int(-6), COMPARE_CALLBACK(compare_ints), NULL);
	printf("After insert_after: \n"); linked_list_node_traverse(list, print_int, NULL); puts("");
	linked_list_node_insert_last(list, create_int(200));
	printf("After insert_last: \n"); linked_list_node_traverse(list, print_int, NULL); puts("");
	list = linked_list_node_sort(list, COMPARE_CALLBACK(compare_ints), NULL);
	printf("After sort:\n"); linked_list_node_traverse(list, print_int, NULL);  puts("");

	linked_list_destroy(list, destroy_int, NULL);
	return 0;
}