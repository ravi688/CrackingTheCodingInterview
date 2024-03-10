#include "static_stack.h"
#include <stdlib.h> // malloc
#include <string.h> // memset

// constructor and destructors
static_stack_t* static_stack_create(int element_size, int capacity)
{
	static_stack_t* stack = (static_stack_t*)malloc(sizeof(static_stack_t));
	memset(stack, 0, sizeof(static_stack_t));
	stack->element_size = element_size;
	stack->capacity = capacity;
	stack->buffer = malloc(element_size  * capacity);
	memset(stack->buffer, 0, element_size * capacity);
	return stack;
}

void static_stack_destroy(static_stack_t* stack)
{
	free(stack->buffer);
	free(stack);
}

// algorithms
bool static_stack_get_count(static_stack_t* stack)
{
	return stack->top;
}

bool static_stack_push(static_stack_t* stack, void* value)
{
	if(stack->top >= stack->capacity)
		return false;
	memcpy(stack->buffer + stack->element_size * stack->top, value, stack->element_size);
	stack->top += 1;
	return true;
}

bool static_stack_pop(static_stack_t* stack, void* value)
{
	if(stack->top <= 0)
		return false;
	stack->top -= 1;
	memcpy(value, stack->buffer + stack->element_size * stack->top, stack->element_size);
	return true;
}

bool static_stack_peek(static_stack_t* stack, void* value)
{
	if(stack->top <= 0)
		return false;
	memcpy(value, stack->buffer + stack->element_size * (stack->top - 1), stack->element_size);
	return true;
}
