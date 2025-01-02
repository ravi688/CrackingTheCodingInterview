#include <iostream>
#include <span> // for std::span<>
#include <algorithm> // for std::ranges::sort()
#include <vector> // for std::vector<>
#include <queue> // for std::queue<>
#include <initializer_list> // for std::initializer_list<>
#include <type_traits>
#include <optional> // for std::optional<>

// Binary Tree Node holding data value of type T
template<typename T>
struct BTNode
{
	T value { };
	BTNode<T>* parent { };
	BTNode<T>* left { };
	BTNode<T>* right { };
};

// NOTE: if we were to write 'const std::span<T>' here, then T will be deduced as 'const int' when calling createBST with const data
//	if we write 'const std::span<const T>', the compiler deduces T as 'int'
template<typename T>
static BTNode<T>* createBST(const std::span<const T> array, std::size_t start, std::size_t end) noexcept
{
	if(start >= end)
		return nullptr;
	std::size_t index = (end + start) / 2;
	auto* node = new BTNode<T> { array[index] };
	node->left = createBST(array, start, index);
	if(node->left)
		node->left->parent = node;
	node->right = createBST(array, index + 1, end);
	if(node->right)
		node->right->parent = node;
	return node;
}

template<template<typename> typename T, typename U>
concept VectorLike = requires(T<U>& array)
{
	{ array.size() } -> std::integral;
	{ array.data() } -> std::same_as<typename std::add_pointer<U>::type>;
};

template<typename T, template<typename> typename Vector> requires(VectorLike<Vector, T>)
static BTNode<T>* createBST(const Vector<T>& array) noexcept
{
	return createBST(std::span { array.data(), array.size() }, 0, array.size());
}

template<typename T>
concept BTNodeType = requires(std::add_pointer_t<T> node)
{
	{ node->left } -> std::convertible_to<std::add_pointer_t<T>>;
	{ node->right } -> std::convertible_to<std::add_pointer_t<T>>;
};

template<BTNodeType Node>
static void _BTNodeGetCount(const Node* node, std::size_t& outCount) noexcept
{
	if(!node) return;
	++outCount;
	_BTNodeGetCount(node->left, outCount);
	_BTNodeGetCount(node->right, outCount);
}

template<BTNodeType Node>
static std::size_t BTNodeGetCount(const Node* node) noexcept
{
	std::size_t count = 0;
	_BTNodeGetCount(node, count);
	return count;
}

template<typename T, typename Node>
concept BTNodeVisitor = requires(T visitor, Node* node)
{
	{ visitor(node) } -> std::same_as<void>;
};

template<BTNodeType Node>
static void BTNodeInOrderTraverse(Node* node, BTNodeVisitor<Node> auto visitor)
{
	if(!node)
		return;
	BTNodeInOrderTraverse(node->left, visitor);
	visitor(node);
	BTNodeInOrderTraverse(node->right, visitor);
}

template<typename T>
static void destroyBST(BTNode<T>* node) noexcept
{
	if(!node)
		return;
	destroyBST(node->left);
	destroyBST(node->right);
	delete node;
}

template<typename T, template<typename> typename Iteratable>
static std::ostream& operator<<(std::ostream& stream, const Iteratable<T>& values) noexcept
{
	stream << "{ ";
	for(decltype(values.size()) i = 0; const auto& value : values)
	{
		stream << value;
		if(++i < values.size())
			stream << ", ";
	}
	stream << " }";
	return stream;
}

template<typename T>
static std::ostream& operator<<(std::ostream& stream, BTNode<T>& bst) noexcept
{
	stream << "\n";
	stream << "Node Count: " << BTNodeGetCount(&bst) << "\n";
	std::size_t level = 0;
	std::queue<BTNode<T>*> queue;
	queue.push(&bst);
	while(!queue.empty())
	{
		stream << "[" << level << "]: ";
		// Only pop out the nodes for this level
		std::size_t count = queue.size();
		// Print the level
		while(count)
		{
			BTNode<T>* node = queue.front();
			queue.pop();
			stream << node->value;
			--count;
			if(count)
				stream << ", ";
			// Add the next level nodes in the back of the queue
			if(node->left)
				queue.push(node->left);
			if(node->right)
				queue.push(node->right);
		}
		if(!queue.empty())
			stream << "\n";
		++level;
	}
	return stream;
}

template<typename T>
static BTNode<T>* BTNodeGetRightMost(BTNode<T>* node) noexcept
{
	if(node->right)
		return BTNodeGetRightMost(node->right);
	return node;
}

template<BTNodeType Node>
static std::size_t BTNodeGetHeight(Node* node) noexcept
{
	if(!node) return 0;
	return std::max(BTNodeGetHeight(node->left), BTNodeGetHeight(node->right)) + ((node->left || node->right) ? 1 : 0);
}

template<BTNodeType Node>
static std::size_t BTNodeGetDepth(Node* node) noexcept
{
	if(!node || !node->parent)
		return 0;
	return BTNodeGetDepth(node->parent) + 1;
}

template<BTNodeType Node>
static Node* BTNodeMoveUp(Node* node, std::size_t climbSteps) noexcept
{
	if(!climbSteps)
		return node;
	return BTNodeMoveUp(node->parent, climbSteps - 1);
}

template<BTNodeType Node>
static bool BTContains(Node* bt, Node* n) noexcept
{
	if(!bt) return false;
	if(bt == n)
		return true;
	return BTContains(bt->left, n) || BTContains(bt->right, n);
}

template<BTNodeType Node>
static bool isOnLeft(Node* node) noexcept
{
	return node->parent->left == node;
}

template<BTNodeType Node>
static Node* findCommonAncestor(Node* bt, Node* n1, Node* n2) noexcept
{
	// Calculate depths of both the nodes
	auto depth1 = BTNodeGetDepth(n1);
	auto depth2 = BTNodeGetDepth(n2);
	// Bring both nodes at the same depth
	if(depth2 > depth1)
		n2 = BTNodeMoveUp(n2, depth2 - depth1);
	else if(depth1 > depth2)
		n1 = BTNodeMoveUp(n1, depth1 - depth2);
	while(n1->parent != n2->parent)
	{
		n1 = n1->parent;
		n2 = n2->parent;
	}
	return n1->parent ? n1->parent : n1;
}

struct Solution1
{
	template<BTNodeType Node>
	Node* operator()(Node* node, Node* n1, Node* n2) noexcept
	{
		std::cout << "Solution 1: \n";
		return findCommonAncestor(node, n1, n2);
	}
};

template<typename Sol, typename T>
static void runCommonAncestor(BTNode<T>* bst, BTNode<T>* n1, BTNode<T>* n2) noexcept
{
	std::cout << "Input Binary Tree: " << *bst << "\n";
	std::cout << "Given nodes: " << n1->value << ", " << n2->value << "\n";
	BTNode<T>* node = Sol { }(bst, n1, n2);
	std::cout << "Common Ancestor node: " << node->value << "\n";
}

template<typename T>
static BTNode<T>* BTFindNode(BTNode<T>* node, const T& value) noexcept
{
	if(!node)
		return nullptr;
	if(node->value == value)
		return node;
	auto* foundNode = BTFindNode(node->left, value);
	if(foundNode)
		return foundNode;
	return BTFindNode(node->right, value);
}

template<typename T>
static void run(std::initializer_list<T> initValues) noexcept
{
	std::cout << "Input: " << initValues << "\n";
	std::vector<T> values (initValues);
	std::ranges::sort(values, std::less<> { });
	std::cout << "Sorted Output: " << values << "\n";
	BTNode<T>* bst = createBST(values);
	std::cout << "BST Output: " << *bst << "\n";
	std::cout << "In order Traversal: "; BTNodeInOrderTraverse<const BTNode<T>>(bst, [](const BTNode<T>* node) noexcept
	{
		std::cout << node->value << " ";
	});
	std::cout << "\n";
	std::cout << "**----Common Ancestor----**\n";
	std::cout << "Input Set 1: \n";
	runCommonAncestor<Solution1>(bst, bst, BTFindNode(bst, 0));
	std::cout << "Input Set 2: \n";
	auto* node = BTNodeGetRightMost(bst);
	node->right = new BTNode<T> { 2355 };
	node->right->parent = node;
	node->right->right = new BTNode<T> { 5443 };
	node->right->right->parent = node->right;
	node->right->right->right = new BTNode<T> { -443 };
	node->right->right->right->parent = node->right->right;
	node->right->right->right->right = new BTNode<T> { -6490 };
	node->right->right->right->right->parent = node->right->right->right;
	std::cout << "In order Traversal: "; BTNodeInOrderTraverse<const BTNode<T>>(bst, [](const BTNode<T>* node) noexcept
	{
		std::cout << node->value << " ";
	});
	std::cout << "\n";
	runCommonAncestor<Solution1>(bst, node, node->right->right->right->right);
	destroyBST(bst);
}

int main()
{
	run<int>({ 100, 2, 3, 4, 0, 45, 32, 56, 7, 8, 10, -1, 2, -4 });
	return 0;
}
