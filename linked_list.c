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
linked_list_node_t* linked_list_node_nth_end(linked_list_node_t* node, int nth)
{
	// Solution on 1 (two pointers approach):
	//
	//  p1 = node
	//	i = 1
	// 	while i != nth && p1 != NULL:
	//		p1 = p1->next
	//		++i
	// 	p2 = node
	// 	while p1 != NULL
	//		p1 = p1->next
	//		p2 = p2->next
	//	return p2

	linked_list_node_t* p1 = node;
	int i = 1;
	while((i < nth) && (p1 != NULL))
	{
		p1 = p1->next;
		++i;
	}
	linked_list_node_t* p2 = node;
	while((p1 != NULL) && (p1->next != NULL))
	{
		p1 = p1->next;
		p2 = p2->next;
	}
	return p2;
}

int linked_list_node_get_length(linked_list_node_t* node)
{
	int count = 0;
	while(node != NULL)
	{
		++count;
		node = node->next;
	}
	return count;
}

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

static linked_list_node_t* get_last(linked_list_node_t* node)
{
	// go to the last node
	while(node->next != NULL)
		node = node->next;
	return node;	
}

linked_list_node_t* linked_list_node_insert_last(linked_list_node_t* node, void* value)
{
	node = get_last(node);
	return linked_list_node_insert_front(node, value);
}

static linked_list_node_t* get_node(linked_list_node_t* node, int index)
{
	int i = 0;
	while((node != NULL) && (i < index))
	{
		node = node->next;
		++i;
	}
	return node;
}

static linked_list_node_t* sorted_merge(linked_list_node_t* first, linked_list_node_t* second, comparer_t compare, void* userData)
{
	linked_list_node_t* final = NULL;
	linked_list_node_t* n3 = NULL;
	linked_list_node_t* n1 = first;
	linked_list_node_t* n2 = second;
	while((n1 != NULL) && (n2 != NULL))
	{
		if(compare(n1->satellite_data, n2->satellite_data, userData) < 0)
		{
			if(n3 == NULL)
			{
				n3 = n1;
				final = n3;
			}
			else
			{
				n3->next = n1;
				n3 = n1;
			}
			n1 = n1->next;
		}
		else
		{
			if(n3 == NULL)
			{
				n3 = n2;
				final = n3;
			}
			else
			{
				n3->next = n2;
				n3 = n2;
			}
			n2 = n2->next;
		}
	}

	if(n1 != NULL)
		n3->next = n1;

	if(n2 != NULL)
		n3->next = n2;

	return final;
}

linked_list_node_t* linked_list_node_sort(linked_list_node_t* node, comparer_t compare, void* userData)
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
	// 	if left_list->value < right_list->value:
	// 		return sorted_merge(left_list, right_list)
	// 	else:
	// 		return sorted_merge(right_list, left_list)

	int len = linked_list_node_get_length(node);
	if(len <= 1)
		return node;
	linked_list_node_t* middle_node = get_node(node, (len - 1) >> 1);

	linked_list_node_t* left_list = node;
	linked_list_node_t* right_list = middle_node->next;
	middle_node->next = NULL;
	left_list = linked_list_node_sort(left_list, compare, userData);
	right_list = linked_list_node_sort(right_list, compare, userData);

	if(compare(left_list->satellite_data, right_list->satellite_data, userData) <= 0)
		return sorted_merge(left_list, right_list, compare, userData);
	else
		return sorted_merge(right_list, left_list, compare, userData);
}
