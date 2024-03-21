#pragma

#include <stdbool.h>

typedef struct heap_t
{
	void* ptr;
	int stride;
	int count;
	int capacity;
} heap_t;


// constructors and destructors
heap_t* heap_create(void* ptr, int stride, int count, int capacity);
void heap_destroy(heap_t* heap);

typedef int (*comparer_t)(void* value, void* compare_value, void* userData);
#define COMPARE_CALLBACK(callback) (comparer_t)(callback)

// algorithms
void heap_build(heap_t* heap, comparer_t compare, void* userData);
void heap_heapify(heap_t* heap, int parent, comparer_t compare, void* userData);
bool heap_root(heap_t* heap, void* out, comparer_t compare, void* userData);
bool heap_insert(heap_t* heap, void* in, comparer_t compare, void* userData);
bool heap_remove(heap_t* heap, void* value, comparer_t compare, void* userData);
void heap_sort(heap_t* heap, comparer_t compare, void* userData);
