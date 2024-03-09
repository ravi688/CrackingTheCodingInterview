#include "static_queue.h"
#include <stdlib.h> // malloc
#include <string.h> // memset

static_queue_t* static_queue_create(int element_size, int capacity)
{
	static_queue_t* queue = (static_queue_t*)malloc(sizeof(static_queue_t));
	memset(queue, 0, sizeof(static_queue_t));
	queue->capacity = capacity;
	queue->element_size	= element_size;
	queue->buffer = malloc(queue->element_size * queue->capacity);
	memset(queue->buffer, 0, queue->element_size * queue->capacity);
	// points to the next vacant place at which new element can be pushed
	queue->front = 0;
	// points to the next filled place from which an element can be popped
	queue->back = 0;
	return queue;	
}

void static_queue_destroy(static_queue_t* queue)
{
	free(queue->buffer);
	free(queue);
}

void static_queue_push(static_queue_t* queue, void* value)
{
	if(queue->front >= queue->capacity)
	{
		// make a vacant place on the left most side
		if(queue->back == 0)
			queue->back = 1;

		// shift all the elements to the left most to fill the vacant places on the back side
		memmove(queue->buffer, queue->buffer + queue->back, queue->element_size * (queue->front - queue->back));
		queue->front -= queue->back;
		queue->back = 0;
	}
	memcpy(queue->buffer + queue->front * queue->element_size, value, queue->element_size);
	queue->front += 1;
}

bool static_queue_pop(static_queue_t* queue, void* value)
{
	if(static_queue_get_count(queue) > 0)
	{
		memcpy(value, queue->buffer + queue->back * queue->element_size, queue->element_size);
		queue->back += 1;
		return true;
	}
	return false;
}

bool static_queue_is_empty(static_queue_t* queue)
{
	return static_queue_get_count(queue) <= 0;
}

int static_queue_get_count(static_queue_t* queue)
{
	return queue->front - queue->back;
}
