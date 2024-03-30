#include "red_black_tree.h"

#include <stdlib.h> // for malloc
#include <string.h> // for memcpy and memset

rb_node_t* rb_node_create(void* value, rb_node_t* left, rb_node_t* right, rb_node_t* parent)
{
	rb_node_t* node = (rb_node_t*)malloc(sizeof(rb_node_t));
	memset(node, 0, sizeof(rb_node_t));
	node->satellite_data = value;
	node->color = COLOR_RED;
	node->left = left;
	node->right = right;
	if(left != NULL)
		left->parent = node;
	if(right != NULL)
		right->parent = node;
	node->parent = parent;
	return node;
}

void* rb_node_destroy(rb_node_t* node)
{
	void* ptr = node->satellite_data;
	free(node);
	return ptr;
}

rb_node_t* rb_node_insert(rb_node_t* root, void* value, comparer_t compare, void* userData)
{
	rb_node_t* node = NULL;
	if(root == NULL)
	{
		node = rb_node_create(value, NULL, NULL, NULL);
		// Red Black Tree Property: Root is always black
		node->color = COLOR_BLACK;
		root = node;
	}
	else if(compare(value, root->satellite_data, userData) > 0)
	{
		rb_node_t* right = root->right;
		if(right == NULL)
		{
			node = rb_node_create(value, NULL, NULL, root);
			root->right = node;
		}
		else
		{
			// if right child is not null, then delegate the insertion to the right child/subtree
			node = rb_node_insert(right, value, compare, userData);
		}
	}
	else
	{
		rb_node_t* left = root->left;
		if(left == NULL)
		{
			node = rb_node_create(value, NULL, NULL, root);
			root->left = node;
		}
		else
		{
			// if left child is not null, then delegate the insertion to the left child/subtree
			node = rb_node_insert(left, value, compare, userData);
		}
	}

	// if the newly added node is not the root node
	// and if this node is not the second added node
	if((node != root) && (root->parent != NULL))
	{
		// no adjacent nodes can be red - property voilated
		if(root->color == COLOR_RED)
		{
			// case no 1
			if((root->parent->left != NULL) && (root->parent->right != NULL))
			{
				// do recolouring
				root->color = COLOR_BLACK;
				if(root->parent->left != root)
					root->parent->left->color = COLOR_BLACK;
				else
					root->parent->right->color = COLOR_BLACK;
			}
			else
			{
				// case 2
				if(root->parent->right == root)
				{
					if(root->right == node)
					{
						// left rotation
						if(root->parent->parent != NULL)
						{
							if(root->parent->parent->left == root->parent)
								root->parent->parent->left = root;
							else // if root->parent->parent->right == root->parent
								root->parent->parent->right = root;
						}
						rb_node_t* t = root->parent;
						root->parent = root->parent->parent;
						root->left = t;
						t->parent = root;
					}
					else // if root->left == node
					{
						// right rotation
						root->parent->right = root->left;
						root->left->parent = root->parent;
						root->left->right = root;
						root->parent = root->left;
						root = root->left;

						// left rotation
						if(root->parent->parent != NULL)
						{
							if(root->parent->parent->left == root->parent)
								root->parent->parent->left = root;
							else // if root->parent->parent->right == root->parent
								root->parent->parent->right = root;
						}
						rb_node_t* t = root->parent;
						root->parent = root->parent->parent;
						root->left = t;
						t->parent = root;
					}
				}
				// case no 3
				else // if root->parent->left == root
				{
					if(root->left == node)
					{
						// right rotation
						if(root->parent->parent->left == root->parent)
							root->parent->parent->left = root;
						else
							root->parent->parent->right = root;
						rb_node_t* t = root->parent;
						root->parent = root->parent->parent;
						root->right = t;
						t->parent = root;
					}
					else // if root->right == node
					{
						// left rotation
						root->parent->left = root->right;
						root->right->parent = root->parent;
						root->right->left = root;
						root->parent = root->right;
						root = root->right;

						// right rotation
						if(root->parent->parent->left == root->parent)
							root->parent->parent->left = root;
						else
							root->parent->parent->right = root;
						rb_node_t* t = root->parent;
						root->parent = root->parent->parent;
						root->right = t;
						t->parent = root;
					}
				}
			}
		}
	}

	return root;
}

rb_node_t* rb_node_remove(rb_node_t* root, void* value, comparer_t compare, void* userData)
{

}

rb_node_t* rb_node_search(rb_node_t* root, void* value, comparer_t compare, void* userData)
{

}

binary_node_t* rb_node_to_binary_node(rb_node_t* node, int data_size)
{
	// Solution no 1:
	// This problem resembles to cloning a binary tree
	//
	// convert(node):
	//		left = node->left
	//		bin_left = null
	//		if left != null:
	//			bin_left = convert(left)
	//		right = node->right
	//		bin_right = null
	//		if right != null:
	//			bin_right = convert(right)
	//		data = null
	//		if node->data != null:
	//			data = malloc(data_size)
	//			memcpy(data, node->data, data_size)
	//		return binary_node_create(data, bin_left, bin_right)

	rb_node_t* left = node->left;
	binary_node_t* bin_left = NULL;
	if(left != NULL)
		bin_left = rb_node_to_binary_node(left, data_size);
	rb_node_t* right = node->right;
	binary_node_t* bin_right = NULL;
	if(right != NULL)
		bin_right = rb_node_to_binary_node(right, data_size);
	void* data = NULL;
	if(node->satellite_data	!= NULL)
	{
		data = malloc(data_size);
		memcpy(data, node->satellite_data, data_size);
	}
	return binary_node_create(data, bin_left, bin_right);
}
