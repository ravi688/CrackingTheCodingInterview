
using System;

using DataStructures;

class BinaryTreeSum
{
	static void BuildBinaryTree(BinaryTree<NodeValueInt> tree)
	{
		BinaryNode<NodeValueInt> n0 = tree.root; n0.SetSatelliteData(new NodeValueInt(-1));
		
		BinaryNode<NodeValueInt> n1 = n0.CreateLeft(new NodeValueInt(2));
		BinaryNode<NodeValueInt> n2 = n0.CreateRight(new NodeValueInt(4));

		BinaryNode<NodeValueInt> n3 = n1.CreateLeft(new NodeValueInt(5));
		BinaryNode<NodeValueInt> n4 = n1.CreateRight(new NodeValueInt(6));
		BinaryNode<NodeValueInt> n5 = n2.CreateLeft(new NodeValueInt(67));
		BinaryNode<NodeValueInt> n6 = n2.CreateRight(new NodeValueInt(100));
	}

	private static void Main(string[] args)
	{
		BinaryTree<NodeValueInt> tree = new BinaryTree<NodeValueInt>(NodeValueInt.zero);
		BuildBinaryTree(tree);
		Console.WriteLine("Sum: {0}", tree.GetSum().value);
	}
}