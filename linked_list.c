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

linked_list_node_t* linked_list_node_insert_front(linked_list_node_t* node, void* value)
{
	linked_list_node_t* next = linked_list_node_create(value, node->next);
	node->next = next;
	return next;
}

linked_list_node_t* linked_list_node_insert_after(linked_list_node_t* node, void* after, void* value, comparer_t compare, void* userData)
{
	// Solution no 1 (Recursive)
	//
	// insert_after(node, after, value):
	// 		if after == node->value:
	// 			 new_node = create_node(value)
	// 			 new_node->next = node->next
	// 			 node->next = new_node
	// 			 return new_node
	//
	// 		if node->next != null:
	// 	  		return insert_after(node->next, after, value)
	//		else return null
	//
	// Solution no 2 (Iterative)
	//
	// insert_after(node, after, value):
	//		while node != null:
	// 			if after == node->value:
	// 			 	new_node = create_node(value)
	// 				new_node->next = node->next
	// 				node->next = new_node
	//				return new_node
	//			else:
	// 				node = node->next
	//		return null

	if(compare(after, node->satellite_data, userData) == 0)
		return linked_list_node_insert_front(node, value);

	if(node->next != NULL)
		return linked_list_node_insert_after(node->next, after, value, compare, userData);
	else
		return NULL;
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

linked_list_node_t* linked_list_node_insert_last(linked_list_node_t* node, void* value)
{
	// go to the last node
	while(node->next != NULL)
		node = node->next;

	return linked_list_node_insert_front(node, value);
}

void linked_list_node_sort(linked_list_node_t* node, comparer_t compare, void* userData)
{
	// Solution no 1 (Merge Sort, Recursive):
	//
	// merge_sort(node):
	//	middle_node, count = get_middle_node(node)
	// 	if count <= 1:
	// 		return node
	// 	left_list = node
	// 	right_list = middle_node->next
	// 	middle_node->next = null
	// 	left_list = merge_sort(left_list)
	// 	right_list = merge_sort(right_list)
	// 		if left_list->value < right_list->value:
	// 			join_last(left_list, right_list)
	// 			join_last(right_list, null)
	// 		else:
	// 			join_last(right_list, left_list)
	// 			join_last(left_list, null)
	// 	return left_list
}
