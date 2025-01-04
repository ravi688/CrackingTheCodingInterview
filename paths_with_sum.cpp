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

template<typename T>
static BTNode<T>* BTNodeGetLeftMost(BTNode<T>* node) noexcept
{
	if(node->left)
		return BTNodeGetLeftMost(node->left);
	return node;
}

template<typename T>
static void findPathsUptoLeaf(const BTNode<T>* node, const T sum, std::vector<T>& buffer, T tempSum, std::vector<std::vector<T>>& paths)
{
	if(!node)
		return;
	buffer.push_back(node->value);

	if((tempSum + node->value) == sum)
		paths.push_back(buffer);

	findPathsUptoLeaf(node->left, sum, buffer, tempSum + node->value, paths);
	findPathsUptoLeaf(node->right, sum, buffer, tempSum + node->value, paths);

	buffer.pop_back();
}

template<typename T>
static void findPaths(const BTNode<T>* node, const T sum, std::vector<T>& buffer, std::vector<std::vector<T>>& paths)
{
	if(!node)
		return;
	findPathsUptoLeaf(node, sum, buffer, 0, paths);
	findPaths(node->left, sum, buffer, paths);
	findPaths(node->right, sum, buffer, paths);
}

template<typename T>
static std::vector<std::vector<T>> findPaths(const BTNode<T>* node, const T sum)
{
	std::vector<std::vector<T>> paths;
	std::vector<T> buffer;
	findPaths(node, sum, buffer, paths);
	return paths;
}

struct Solution1
{
	template<typename T>
	std::vector<std::vector<T>> operator()(const BTNode<T>* bt, const T sum) noexcept
	{
		std::cout << "Solution 1: \n";
		return findPaths(bt, sum);
	}
};

template<typename Sol, typename T>
static void runFindPaths(const BTNode<T>* bt, const T sum) noexcept
{
	static std::size_t runCount = 0;
	std::cout << "RUN: " << runCount << "\n";
	++runCount;
	std::cout << "Sum: " << sum << "\n";
	std::vector<std::vector<T>> paths = Sol { }(bt, sum);
	std::cout << "Path count: " << paths.size() << "\n";
	for(const auto& path : paths)
		std::cout << path << "\n";
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
	std::cout << "**----Find-All-Paths-Given-Sum----**\n";
	std::cout << "Binary Tree: " << *bst << "\n";
	runFindPaths<Solution1>(bst, 100);
	BTNodeGetLeftMost(bst)->left = new BTNode<T> { -4 };
	runFindPaths<Solution1>(bst, 0);
	runFindPaths<Solution1>(bst, 42);
	runFindPaths<Solution1>(bst, 32);
	runFindPaths<Solution1>(bst, 4);
	runFindPaths<Solution1>(bst, 7);
	runFindPaths<Solution1>(bst, 80);
	destroyBST(bst);
}

int main()
{
	run<int>({ 100, 2, 3, 4, 0, 45, 32, 56, 7, 8, 10, -1, 2, -4 });
	return 0;
}
