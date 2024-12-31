#include <iostream>
#include <span> // for std::span<>
#include <algorithm> // for std::ranges::sort()
#include <vector> // for std::vector<>
#include <queue> // for std::queue<>
#include <initializer_list> // for std::initializer_list<>
#include <type_traits>

// Binary Tree Node holding data value of type T
template<typename T>
struct BTNode
{
	T value { };
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
	node->right = createBST(array, index + 1, end);
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

template<typename T1, typename T2>
static constexpr std::pair<T1, T2> minmax(T1 v1, T2 v2)
{
	if(v1 < v2) return { v1, v2 };
	return { v2, v1 };
}

template<typename T1, typename T2>
static constexpr auto diff(T1 v1, T2 v2)
{
	auto p = minmax(v1, v2);
	return p.second - p.first;
}

template<typename VisitorType, typename NodeType, typename ValueType>
concept BTNodePredicateVisitor = requires(VisitorType visitor, NodeType* node, ValueType& value)
{
	{ visitor(node, value) } -> std::convertible_to<bool>;
};

template<BTNodeType Node, typename ValueType>
static bool isForAllNodes(Node* node, ValueType& value, BTNodePredicateVisitor<Node, ValueType> auto visitor) noexcept
{
	if(!node) return true;
	if(!visitor(node, value))
		return false;
	if(!isForAllNodes(node->left, value, visitor))
		return false;
	return isForAllNodes(node->right, value, visitor);
}

// Solution no 1
template<BTNodeType Node>
static bool validateBST1(const Node* node) noexcept
{
	if(!node) return true;
	// NOTE: 'node' is const qualified and we want the type of one of its member variables to be const qualified too
	using ValueType = typename std::add_const<decltype(node->value)>::type;
	bool isLeftLess = isForAllNodes(node->left, node->value, [](const Node* node, ValueType& value) noexcept
	{
		return node->value < value;
	});
	bool isRightGreater = isForAllNodes(node->right, node->value, [](const Node* node, ValueType& value) noexcept
	{
		return node->value >= value;
	});
	return isLeftLess && isRightGreater && validateBST1(node->left) && validateBST1(node->right);
}

template<BTNodeType Node>
static bool validateBST2(const Node* node) noexcept
{
	return false;
}

struct Solution1
{
	template<BTNodeType Node>
	bool operator()(Node* node) noexcept
	{
		std::cout << "Solution 1: \n";
		return validateBST1(node);
	}
};

struct Solution2
{
	template<BTNodeType Node>
	bool operator()(Node* node) noexcept
	{
		std::cout << "Solution 2: \n";
		return validateBST2(node);
	}
};

template<typename Sol, typename T>
static void runValidateBST(BTNode<T>* bst) noexcept
{
	std::cout << "Input Binary Tree: " << *bst << "\n";
	bool isBalanced = Sol { }(bst);
	std::cout << "isValid: " << std::boolalpha << isBalanced << "\n";
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
	std::cout << "**----Validate-BST----**\n";
	std::cout << "Input Set 1: \n";
	runValidateBST<Solution1>(bst);
	runValidateBST<Solution2>(bst);
	std::cout << "Input Set 2: \n";
	auto* node = BTNodeGetRightMost(bst);
	node->right = new BTNode<T> { 2355 };
	node->right->right = new BTNode<T> { 5443 };
	node->right->right->right = new BTNode<T> { -443 };
	node->right->right->right->right = new BTNode<T> { -6490 };
	std::cout << "In order Traversal: "; BTNodeInOrderTraverse<const BTNode<T>>(bst, [](const BTNode<T>* node) noexcept
	{
		std::cout << node->value << " ";
	});
	std::cout << "\n";
	runValidateBST<Solution1>(bst);
	runValidateBST<Solution2>(bst);
	destroyBST(bst);
}

int main()
{
	run<int>({ 100, 2, 3, 4, 0, 45, 32, 56, 7, 8, 10, -1, 2, -4 });
	return 0;
}
