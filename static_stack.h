#pragma once

#include <stdbool.h>

// type definition
typedef struct static_stack_t
{
	void* buffer;
	int capacity;
	int element_size;
	int top;
} static_stack_t;

// constructor and destructors
static_stack_t* static_stack_create(int element_size, int capacity);
void static_stack_destroy(static_stack_t* stack);

// algorithms
bool static_stack_get_count(static_stack_t* stack);
static inline bool static_stack_is_empty(static_stack_t* stack) { return static_stack_get_count(stack) == 0; }
bool static_stack_push(static_stack_t* stack, void* value);
bool static_stack_pop(static_stack_t* stack, void* value);
bool static_stack_peek(static_stack_t* stack, void* value);
