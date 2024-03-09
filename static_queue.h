#pragma once

#include <stdbool.h>

typedef struct static_queue_t
{
	void* buffer;
	int capacity;
	int front;
	int back;
	int element_size;
} static_queue_t;

// constructors and destructors
static_queue_t* static_queue_create(int element_size, int capacity);
void static_queue_destroy(static_queue_t* queue);

// algorithms 
void static_queue_push(static_queue_t* queue, void* value);
bool static_queue_pop(static_queue_t* queue, void* value);
bool static_queue_is_empty(static_queue_t* queue);
int static_queue_get_count(static_queue_t* queue);
