
#include "../binary_tree/binary_tree.hpp"
#include <iostream>

static void BuildBinaryTree(BinaryTree<int>& tree)
{
	BinaryNode<int>* n0 = tree.getRoot(); n0->setSatelliteData(-1);

	BinaryNode<int>* n1 = n0->createLeft(2);
	BinaryNode<int>* n2 = n0->createRight(4);

	BinaryNode<int>* n3 = n1->createLeft(5);
	BinaryNode<int>* n4 = n1->createRight(6);
	BinaryNode<int>* n5 = n2->createLeft(67);
	BinaryNode<int>* n6 = n2->createRight(100);
}

int main(int argc, const char* argv[])
{
	BinaryTree<int> tree;
	BuildBinaryTree(tree);
	std::cout << "Sum: " << tree.getSum() << std::endl;


	// calls the BinaryTree<int>::operator BinaryNode<int>&() overload
	BinaryNode<int>& node = tree;
	// calls the BinaryTree<int>::operator BinaryNode<int>*() overload
	BinaryNode<int>* nodePtr = tree;
	
	return 0;
}