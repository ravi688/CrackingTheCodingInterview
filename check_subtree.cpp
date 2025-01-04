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
static void BTNodeInOrderTraverse(Node* node, BTNodeVisitor<Node> auto visitor, bool isVisitNullNodes = false)
{
	if(!node)
	{
		if(isVisitNullNodes)
			visitor(nullptr);
		return;
	}
	BTNodeInOrderTraverse(node->left, visitor, isVisitNullNodes);
	visitor(node);
	BTNodeInOrderTraverse(node->right, visitor, isVisitNullNodes);
}


template<typename T, typename Node>
concept BTNodePredVisitor = requires(T visitor, Node* node)
{
	{ visitor(node) } -> std::same_as<bool>;
};

template<BTNodeType Node>
static bool BTNodeInOrderTraverseUntil(Node* node, BTNodePredVisitor<Node> auto visitor, bool isVisitNullNodes = false)
{
	if(!node)
	{
		if(isVisitNullNodes)
			return visitor(nullptr);
		return true;
	}
	if(!BTNodeInOrderTraverseUntil(node->left, visitor, isVisitNullNodes))
		return false;
	if(!visitor(node))
		return false;
	if(!BTNodeInOrderTraverseUntil(node->right, visitor, isVisitNullNodes))
		return false;
	return true;
}

template<BTNodeType Node>
static void BTNodePreOrderTraverse(Node* node, BTNodeVisitor<Node> auto visitor, bool isVisitNullNodes = false)
{
	if(!node)
	{
		if(isVisitNullNodes)
			visitor(nullptr);
		return;
	}
	BTNodePreOrderTraverse(node->left, visitor, isVisitNullNodes);
	visitor(node);
	BTNodePreOrderTraverse(node->right, visitor, isVisitNullNodes);
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

template<template<typename> typename T, typename U>
concept Iteratable = requires(T<U>& range)
{
	{ *range.begin() } -> std::convertible_to<U>;
	{ *range.end() } -> std::convertible_to<U>;
	{ range.size() } -> std::integral;
};

template<typename T, template<typename> typename Range> requires(Iteratable<Range, T>)
static std::ostream& operator<<(std::ostream& stream, const Range<T>& values) noexcept
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
static std::ostream& operator<<(std::ostream& stream, const BTNode<T>& bst) noexcept
{
	stream << "\n";
	stream << "Node Count: " << BTNodeGetCount(&bst) << "\n";
	std::size_t level = 0;
	std::queue<const BTNode<T>*> queue;
	queue.push(&bst);
	while(!queue.empty())
	{
		stream << "[" << level << "]: ";
		// Only pop out the nodes for this level
		std::size_t count = queue.size();
		// Print the level
		while(count)
		{
			const BTNode<T>* node = queue.front();
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

template<typename T>
static std::string buildInOrderString(const BTNode<T>* node)
{
	std::string str;
	BTNodePreOrderTraverse(node, [&str](const BTNode<T>* node) noexcept
	{
		if(node)
			str.append(std::to_string(node->value));
		else
			str.append("X");
	}, true);
	return str;
}

template<typename T>
static bool checkSubTree1(const BTNode<T>* bt1, const BTNode<T>* bt2)
{
	std::string str1 = buildInOrderString(bt1);
	std::string str2 = buildInOrderString(bt2);
	std::cout << "str1: " << str1.c_str() << "\n";
	std::cout << "str2: " << str2.c_str() << "\n";
	return str1.find(str2) != std::string::npos;
}

template<typename T>
static bool matchBT(const BTNode<T>* bt1, const BTNode<T>* bt2)
{
	// If both the nodes are null then they are equal,
	// If exactly one of them is null then they aren't equal
	if(!bt1 || !bt2)
		return bt1 == bt2;
	return (bt1->value == bt2->value) && matchBT(bt1->left, bt2->left) && matchBT(bt1->right, bt1->right);
}

template<typename T>
static bool checkSubTree2(const BTNode<T>* bt1, const BTNode<T>* bt2)
{
	// Empty tree (null) is always a sub-tree
	if(!bt2)
		return true;
	return !BTNodeInOrderTraverseUntil(bt1, [bt2](const BTNode<T>* node) noexcept -> bool
	{
		bool isFound = (node->value == bt2->value) && matchBT(node, bt2);
		// Keep traversing until a matching subtree is found
		return !isFound;
	});
}

struct Solution1
{
	template<BTNodeType Node>
	bool operator()(Node* bt1, Node* bt2) noexcept
	{
		std::cout << "Solution 1: \n";
		return checkSubTree1(bt1, bt2);
	}
};

struct Solution2
{
	template<BTNodeType Node>
	bool operator()(Node* bt1, Node* bt2) noexcept
	{
		std::cout << "Solution 2: \n";
		return checkSubTree2(bt1, bt2);
	}
};

template<typename Sol, typename T>
static void runCheckSubTree(const BTNode<T>* bt1, const BTNode<T>* bt2) noexcept
{
	if(bt1)
		std::cout << "Input Binary Tree 1: " << *bt1 << "\n";
	else
		std::cout << "Input Binary Tree 1: (null)" << "\n";
	if(bt2)
		std::cout << "Input Binary Tree 2: " << *bt2 << "\n";
	else
		std::cout << "Input Binary Tree 2: (null)" << "\n";
	bool isSubTree = Sol { }(bt1, bt2);
	std::cout << "isSubTree: " << std::boolalpha << isSubTree << "\n";
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
	std::cout << "**----Check-SubTree----**\n";
	std::cout << "Input Set 1: \n";
	// EXPECT: True
	runCheckSubTree<Solution1>(bst, bst);
	// EXPECT: True
	runCheckSubTree<Solution2>(bst, bst);
	std::cout << "Input Set 2: \n";
	// EXPECT: False
	runCheckSubTree<Solution1>(bst->right, bst->left);
	// EXPECT: FAlse
	runCheckSubTree<Solution2>(bst->right, bst->left);
	std::cout << "Input Set 3: \n";
	auto* node = BTNodeGetRightMost(bst);
	node->right = new BTNode<T> { 2355 };
	node->right->right = new BTNode<T> { 5443 };
	node->right->right->right = new BTNode<T> { -443 };
	node->right->right->right->right = new BTNode<T> { -6490 };
	// EXPECT: True
	runCheckSubTree<Solution1>(bst->right, node->right->right);
	// EXPECT: tRUE
	runCheckSubTree<Solution2>(bst->right, node->right->right);
	std::cout << "Input Set 4: \n";
	// EXPECT: True
	runCheckSubTree<Solution1>(bst, static_cast<BTNode<T>*>(nullptr));
	// EXPECT: True
	runCheckSubTree<Solution2>(bst, static_cast<BTNode<T>*>(nullptr));
	std::cout << "Input Set 5: \n";
	// EXPECT: True
	runCheckSubTree<Solution1>(bst, bst->right->right);
	// EXPECT: True
	runCheckSubTree<Solution2>(bst, bst->right->right);
	destroyBST(bst);
}

int main()
{
	run<int>({ 100, 2, 3, 4, 0, 45, 32, 56, 7, 8, 10, -1, 2, -4 });
	return 0;
}
