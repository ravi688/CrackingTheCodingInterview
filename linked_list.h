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
linked_list_node_t* linked_list_node_create_next(linked_list_node_t* node, void* satellite_data);
void linked_list_node_destroy(linked_list_node_t* node, void (*callback)(linked_list_node_t* node, void* userData), void* userData);
static inline linked_list_t* linked_list_create(void* satellite_data) { return linked_list_node_create(satellite_data, NULL); }
void linked_list_destroy(linked_list_t* list, void (*callback)(linked_list_node_t* node, void* userData), void* userData);

// getters
static inline void* linked_list_node_get_satellite_data(linked_list_node_t* node) { return node->satellite_data; }

// algorithms
void linked_list_node_traverse(linked_list_node_t* node, void (*callback)(linked_list_node_t* node, void* userData), void* userData);
void linked_list_node_traverse_postorder(linked_list_node_t* node, void (*callback)(linked_list_node_t* node, void* userData), void* userData);
void linked_list_node_reverse(linked_list_node_t* node);
