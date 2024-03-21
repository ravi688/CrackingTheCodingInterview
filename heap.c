#include "heap.h"

#include <stdlib.h> // malloc
#include <string.h> // memset

// constructors and destructors
heap_t* heap_create(void* ptr, int stride, int count, int capacity)
{
	heap_t* heap = (heap_t*)malloc(sizeof(heap_t));
	memset(heap, 0, sizeof(heap_t));
	heap->ptr = ptr;
	heap->stride = stride;
	heap->count = count;
	heap->capacity = capacity;
	return heap;
}

void heap_destroy(heap_t* heap)
{
	free(heap);
}

#define GET(heap, index) (heap->ptr + heap->stride * (index - 1))

static void _heap_heapify_top_down(heap_t* heap, comparer_t compare, void* userData, int parent)
{
	// calculate the indices of left and right child
	int left = parent << 1;
	int right = left + 1;

	if((left > heap->count) || (right > heap->count))
		return;

	// determine the node with largested value
	int max = parent;
	if(compare(GET(heap, parent), GET(heap, left), userData) < 0)
		max = left;
	if(compare(GET(heap, max), GET(heap, right), userData) < 0)
		max = right;

	// do we need to swap?
	if(max != parent)
	{
		// swap
		char buffer[heap->stride];
		memcpy(buffer, GET(heap, max), heap->stride);
		memcpy(GET(heap, max), GET(heap, parent), heap->stride);
		memcpy(GET(heap, parent), buffer, heap->stride);
		// heapify the affected sub-tree
		_heap_heapify_top_down(heap, compare, userData, max);
	}
}

// algorithms
void heap_build(heap_t* heap, comparer_t compare, void* userData)
{
	// heapify the internal nodes in bottotm up manner
	// we can skip the leaves because there are no childs of leaves
	for(int i = ((heap->count >> 1) - 1); i >= 1; --i)
		_heap_heapify_top_down(heap, compare, userData, i);
}

void heap_heapify(heap_t* heap, int parent, comparer_t compare, void* userData)
{
	_heap_heapify_top_down(heap, compare, userData, parent);
}

bool heap_root(heap_t* heap, void* out, comparer_t compare, void* userData)
{
	// if there are no elements, return false
	if(heap->count <= 0)
		return false;

	// swap the root with the last
	char buffer[heap->stride];
	memcpy(buffer, GET(heap, heap->count), heap->stride);
	memcpy(GET(heap, heap->count), GET(heap, 1), heap->stride);
	memcpy(GET(heap, 1), buffer, heap->stride);

	// remove the last
	--heap->count;

	// heapify from the root node, as it might now have smaller value than its children
	_heap_heapify_top_down(heap, compare, userData, 1);

	return true;
}

static void _heap_heapify_bottom_up(heap_t* heap, comparer_t compare, void* userData, int index)
{
	// calculate index of the parent
	int parent = (heap->count - 1) >> 1;

	// if there are no parents then return
	if(parent <= 0)
		return;

	// determine the node with largested value
	if(compare(GET(heap, parent), GET(heap, index), userData) < 0)
	{
		// swap
		char buffer[heap->stride];
		memcpy(buffer, GET(heap, parent), heap->stride);
		memcpy(GET(heap, parent), GET(heap, index), heap->stride);
		memcpy(GET(heap, index), buffer, heap->stride);
		// heapify the affected anscestors
		_heap_heapify_bottom_up(heap, compare, userData, parent);
	}
}

bool heap_insert(heap_t* heap, void* in, comparer_t compare, void* userData)
{
	// if all vacant places have been filled, then return false
	if(heap->count >= heap->capacity)
		return false;

	// add a new element in the last
	++heap->count;
	memcpy(GET(heap, heap->count), in, heap->stride);

	// bubble up the newly inserted element
	_heap_heapify_bottom_up(heap, compare, userData, heap->count);
}

static void _heap_swap(heap_t* heap, int index1, int index2);

bool heap_remove(heap_t* heap, void* value, comparer_t compare, void* userData)
{
	// find the element matching the 'compare' criteria
	for(int i = 1; i <= heap->count; i++)
		if(compare(value, GET(heap, i), userData) == 0)
		{
			// swap the matching element with the last element
			_heap_swap(heap, i, heap->count);
			// remove the last element (now a.k.a the matching element)
			--heap->count;
			// heapify the subtree from where the matching elemnet was found
			_heap_heapify_top_down(heap, compare, userData, i);
			return true;
		}
	// no element found
	return false;
}

static void _heap_swap(heap_t* heap, int index1, int index2)
{
	char buffer[heap->stride];
	memcpy(buffer, GET(heap, index1), heap->stride);
	memcpy(GET(heap, index1), GET(heap, index2), heap->stride);
	memcpy(GET(heap, index2), buffer, heap->stride);
}

void heap_sort(heap_t* heap, comparer_t compare, void* userData)
{
	// we need to preserve the element count in this heap, even after sort
	int count = heap->count;

	// loop until there are elements in the heap,
	// we can skip the last one (the top) as that would only left at the top
	while(heap->count > 1)
	{
		// swap the top element with the last element
		_heap_swap(heap, 1, heap->count);
		// remove the last element (now a.k.a top element)
		--heap->count;
		// bubble down the top element (a.k.a the last element)
		_heap_heapify_top_down(heap, compare, userData, 1);
	}

	// restore the heap's count
	heap->count = count;
}

