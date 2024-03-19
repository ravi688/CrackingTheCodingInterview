#include "linked_list.h"

#include <stdlib.h> // malloc
#include <string.h> // memset

// constructors and destructors
linked_list_node_t* linked_list_node_create(void* satellite_data, linked_list_node_t* next)
{
	linked_list_node_t* node = (linked_list_node_t*)malloc(sizeof(linked_list_node_t));
	memset(node, 0, sizeof(linked_list_node_t));
	node->satellite_data = satellite_data;
	node->next = next;
	return node;
}

linked_list_node_t* linked_list_node_create_next(linked_list_node_t* node, void* satellite_data)
{
	linked_list_node_t* next = linked_list_node_create(satellite_data, NULL);
	node->next = next;
	return next;
}

void linked_list_node_destroy(linked_list_node_t* node, void (*callback)(linked_list_node_t* node, void* userData), void* userData)
{
	if(callback != NULL)
		callback(node, userData);
	free(node);
}

void linked_list_destroy(linked_list_t* list, void (*callback)(linked_list_node_t* node, void* userData), void* userData)
{
	if(list->next != NULL)
		linked_list_destroy(list->next, callback, userData);
	linked_list_node_destroy(list, callback, userData);
}

// algorithms
void linked_list_node_traverse(linked_list_node_t* node, void (*callback)(linked_list_node_t* node, void* userData), void* userData)
{
	callback(node, userData);
	if(node->next != NULL)
		linked_list_node_traverse(node->next, callback, userData);
}

void linked_list_node_traverse_postorder(linked_list_node_t* node, void (*callback)(linked_list_node_t* node, void* userData), void* userData)
{
	if(node->next != NULL)
		linked_list_node_traverse_postorder(node->next, callback, userData);
	callback(node, userData);
}

linked_list_node_t* linked_list_node_reverse(linked_list_node_t* node)
{
	// Solution no 1
	// n0 -> n1 -> n2 -> n3 -> n4 -> ...
	//  
	// n1 -> n0 -> n2 -> n3 -> n4 -> ...
	// n2 -> n1 -> n0 -> n3 -> n4 -> ...
	// n3 -> n2 -> n1 -> n0 -> n4 -> ...
	// n4 -> n3 -> n2 -> n1 -> n0 -> ...
	//  
	// p1 = n0
	// p2 = n0->next
	// while p2 != null:
	// 		n0->next = p2->next
	// 		t = p2->next
	// 		p2->next = p1
	//		p1 = p2
	// 		p2 = t

	linked_list_node_t* p1 = node;
	linked_list_node_t* p2 = node->next;
	while(p2 != NULL)
	{
		node->next = p2->next;
		linked_list_node_t* t = p2->next;
		p2->next = p1;
		p1 = p2;
		p2 = t;
	}
	return p1;
}
