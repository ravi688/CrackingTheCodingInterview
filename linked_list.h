#pragma once

#include <stdio.h>
#include <stdbool.h>

// forward declaration
typedef struct linked_list_node_t linked_list_node_t;

// binary node type definition
typedef struct linked_list_node_t
{
	// pointer to payload data
	void* satellite_data;

	// pointer to the next node
	linked_list_node_t* next;
} linked_list_node_t;

// typedef alias of linked_list_node_t
typedef linked_list_node_t linked_list_t;

// constructors and destructors 
linked_list_node_t* linked_list_node_create(void* satellite_data, linked_list_node_t* next);
void linked_list_node_destroy(linked_list_node_t* node, void (*callback)(linked_list_node_t* node, void* userData), void* userData);
static inline linked_list_t* linked_list_create(void* satellite_data) { return linked_list_node_create(satellite_data, NULL); }
void linked_list_destroy(linked_list_t* list, void (*callback)(linked_list_node_t* node, void* userData), void* userData);

// getters
static inline void* linked_list_node_get_satellite_data(linked_list_node_t* node) { return node->satellite_data; }

// algorithms
typedef int (*comparer_t)(void* value, void* compare_value, void* userData);
#define COMPARE_CALLBACK(callback) (comparer_t)(callback)

int linked_list_node_get_length(linked_list_node_t* node);
void linked_list_node_traverse(linked_list_node_t* node, void (*callback)(linked_list_node_t* node, void* userData), void* userData);
void linked_list_node_traverse_postorder(linked_list_node_t* node, void (*callback)(linked_list_node_t* node, void* userData), void* userData);
linked_list_node_t* linked_list_node_reverse(linked_list_node_t* node);
linked_list_node_t* linked_list_node_insert_front(linked_list_node_t* node, void* value);
linked_list_node_t* linked_list_node_insert_after(linked_list_node_t* node, void* after, void* value, comparer_t compare, void* userData);
linked_list_node_t* linked_list_node_insert_last(linked_list_node_t* node, void* value);
linked_list_node_t* linked_list_node_sort(linked_list_node_t* node, comparer_t compare, void* userData);
