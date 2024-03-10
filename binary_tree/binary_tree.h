
#include <stdio.h>
#include <stdbool.h>

// forward declaration 
typedef struct binary_node_t binary_node_t;

// binary node type definition
typedef struct binary_node_t
{
	void* satellite_data;
	binary_node_t* left;
	binary_node_t* right;
	binary_node_t* parent;
} binary_node_t;

typedef binary_node_t binary_tree_t;

// constructors and destructors
binary_tree_t* binary_node_create(void* satellite_data, binary_node_t* left, binary_node_t* right);
void binary_node_destroy(binary_tree_t* tree, void (*callback)(binary_node_t* node, void* userData), void* userData);
static inline binary_tree_t* binary_tree_create(void* satellite_data) { return binary_node_create(satellite_data, NULL, NULL); }
#define DESTROY_CALLBACK(callback) TRAVERSE_CALLBACK(callback)
void binary_tree_destroy(binary_tree_t* tree, void (*callback)(binary_node_t* node, void* userData), void* userData);
binary_node_t* binary_node_create_left(binary_node_t* node, void* satellite_data);
binary_node_t* binary_node_create_right(binary_node_t* node, void* satellite_data);

// getters and setters (auxiliary functions)
static inline binary_node_t* binary_node_get_left(binary_node_t* node) { return node->left; }
static inline binary_node_t* binary_node_get_right(binary_node_t* node) { return node->right; }
static inline binary_node_t* binary_node_get_parent(binary_node_t* node) { return node->parent; }
binary_node_t* binary_node_set_left(binary_node_t* node, binary_node_t* left);
binary_node_t* binary_node_set_right(binary_node_t* node, binary_node_t* right);
static inline void* binary_node_get_satellite_data(binary_node_t* node) { return node->satellite_data; }
static inline int binary_node_get_satellite_data_as_int(binary_node_t* node) { return *(int*)node->satellite_data; }

// algorithms
#define TRAVERSE_CALLBACK(callback) ((void (*)(binary_node_t*, void*))(callback))
#define DATA_CLONE_CALLBACK(callback) ((void* (*)(binary_node_t*, void*))(callback))
int binary_node_get_sum_as_int(binary_node_t* node);
void binary_node_traverse_preorder(binary_node_t* node, void (*callback)(binary_node_t* node, void* userData), void* userData);
void binary_node_traverse_inorder(binary_node_t* node, void (*callback)(binary_node_t* node, void* userData), void* userData);
binary_node_t* binary_node_clone(binary_node_t* node, void* (*data_clone_callback)(binary_node_t* node, void* userData), void* userData);
void binary_node_traverse_inorder2(binary_node_t* node, void (*callback)(binary_node_t* node, void* userData), void* userData);
void binary_node_traverse_postorder(binary_node_t* node, void (*callback)(binary_node_t* node, void* userData), void* userData);
void binary_node_traverse_level_order(binary_node_t* node, void (*callback)(binary_node_t* node, void* userData), void* userData);
void binary_node_traverse_level_order2(binary_node_t* node, void (*callback)(binary_node_t* node, void* userData), void* userData);
void binary_node_traverse_boundry(binary_node_t* node, void (*callback)(binary_node_t* node, void* userData), void* userData);
void binary_node_traverse_boundry2(binary_node_t* node, void (*callback)(binary_node_t* node, void* userData), void* userData);
void binary_node_traverse_diagonal(binary_node_t* node, void (*callback)(binary_node_t* node, void* userData), void* userData);
int binary_tree_get_count(binary_tree_t* tree);

typedef int (*comparer_t)(void* value, void* compare_value, void* userData);
#define COMPARE_CALLBACK(callback) (comparer_t)(callback)
binary_node_t* binary_search_tree_insert(binary_tree_t* tree, void* value, comparer_t compare_callback, void* userData);
bool binary_search_tree_remove(binary_tree_t* tree, void* value, comparer_t compare_callback, void* userData1, void (*destroyCallback)(binary_node_t*, void* userData), void* userData2, binary_tree_t** new_root);
binary_node_t* binary_search_tree_search(binary_node_t* tree, void* value, comparer_t compare_callback, void* userData);
