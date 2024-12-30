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
struct LLNode
{
	T value;
	LLNode<T>* next { };
};

// Solution no 1
template<typename T>
static std::vector<LLNode<BTNode<T>*>*> createListOfDepths(BTNode<T>* node)
{
	std::vector<LLNode<BTNode<T>*>*> depths;
	if(!node)
		return depths;
	std::queue<BTNode<T>*> queue;
	queue.push(node);
	while(!queue.empty())
	{
		LLNode<BTNode<T>*>* llNode { };
		LLNode<BTNode<T>*>* headNode { };
		std::size_t count = queue.size();
		while(count)
		{
			auto* node = queue.front();
			queue.pop();
			if(!llNode)
			{
				llNode = new LLNode<BTNode<T>*> { node };
				headNode = llNode;
			}
			else
			{
				llNode->next = new LLNode<BTNode<T>*> { node };
				llNode = llNode->next;
			}
			--count;
			if(node->left)
				queue.push(node->left);
			if(node->right)
				queue.push(node->right);
		}
		depths.push_back(headNode);
	}
	// Named RVO would kick-in here
	return depths;
}


template<typename T, typename Node, typename SizeType>
concept BTNodeDepthVisitor = requires(T visitor, Node* node, SizeType depth)
{
	{ visitor(node, depth) } -> std::same_as<void>;
};

template<BTNodeType Node, std::integral SizeType>
static void BTNodePreOrderDepthTraverse(Node* node, SizeType depth, BTNodeDepthVisitor<Node, SizeType> auto visitor)
{
	if(!node)
		return;
	visitor(node, depth);
	BTNodePreOrderDepthTraverse<Node, SizeType>(node->left, depth + 1, visitor);
	BTNodePreOrderDepthTraverse<Node, SizeType>(node->right, depth + 1, visitor);
}

template<BTNodeType Node, std::integral SizeType>
static void BTNodePreOrderDepthTraverse(Node* node, BTNodeDepthVisitor<Node, SizeType> auto visitor)
{
	SizeType depth = 0;
	BTNodePreOrderDepthTraverse<Node, SizeType>(node, depth, visitor);
}

template<typename T>
static std::vector<LLNode<BTNode<T>*>*> createListOfDepths2(BTNode<T>* node)
{
	std::vector<LLNode<BTNode<T>*>*> depths;
	std::vector<LLNode<BTNode<T>*>*> heads;

	// NOTE: Why are we doing pre-order traversal here? can't we do the same thing with in-order traversal?
	// The answer is no, since we are adding a new linked list based on 'size() < (depth + 1)' comparison
	// For example, in case of in-order traversal, the left-most node will be visited first which will have depth greater
	// than that of the root node (0) - but our assumption was that root level linked list must be created first, so
	// that is the reason we can't use in-order traversal and neither post-order traversal.

	// GCC is not able to deduce 'SizeType' template parameter here, so we have to provide that explicitly
	BTNodePreOrderDepthTraverse<BTNode<T>, std::size_t>(node, [&depths, &heads](BTNode<T>* node, std::size_t depth) noexcept
	{
		if(depths.size() < (depth + 1))
		{
			depths.push_back(new LLNode<BTNode<T>*> { node });
			heads.push_back(depths.back());
		}
		else
		{
			auto& ref = depths[depth];
			ref->next = new LLNode<BTNode<T>*> { node };
			ref = ref->next;
		}
	});
	return heads;
}

template<typename T>
static void destroyLL(LLNode<T>* node)
{
	if(!node)
		return;
	destroyLL(node->next);
	delete node;
}

struct Solution1
{
	template<typename T>
	std::vector<LLNode<BTNode<T>*>*> operator()(BTNode<T>* node)
	{
		return createListOfDepths(node);
	}
};

struct Solution2
{
	template<typename T>
	std::vector<LLNode<BTNode<T>*>*> operator()(BTNode<T>* node)
	{
		return createListOfDepths2(node);
	}
};

template<typename Sol, typename T>
static void runListOfDepths(BTNode<T>* bst) noexcept
{
	std::vector<LLNode<BTNode<T>*>*> depths = Sol{ }(bst);
	std::cout << "List of depths: \n";
	for(std::size_t i = 0; const auto& ll : depths)
	{
		std::cout << "[" << i << "]: " << "{ ";
		const auto* node = ll;
		while(node)
		{
			std::cout << node->value->value;
			node = node->next;
			if(node)
				std::cout << ", ";
		}
		std::cout << " }\n";
		++i;
	}
	for(auto& ll : depths)
		destroyLL(ll);
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
	std::cout << "**----list of depths----**\n";
	std::cout << "Solution 1 (using Queue): \n";
	runListOfDepths<Solution1>(bst);
	std::cout << "Solution 2 (using Pre-Order Traversal): \n";
	runListOfDepths<Solution2>(bst);
	destroyBST(bst);
}

int main()
{
	run<int>({ 100, 2, 3, 4, 0, 45, 32, 56, 7, 8, 10, -1, 2, -4 });
	return 0;
}
