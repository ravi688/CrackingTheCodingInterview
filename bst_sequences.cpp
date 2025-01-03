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

// WARN: This solution has a serious flaw, it just swaps the subtrees recursively! So it doesn't produce exhaustive list of
// possible sequences.
template<typename T, std::integral SizeType>
static void generateBSTSequences(BTNode<T>* node, SizeType& n,  std::vector<T>& seq, std::vector<std::vector<T>>& seqs) noexcept
{
	if(!node) return;
	// The parent node must have been added first
	seq.push_back(node->value);
	auto cnt = seq.size();
	// If this was the last node then we have just build a possible sequence
	if(cnt == n)
	{
		// so add this another possible sequence
		seqs.push_back(seq);
		return;
	}
	// Recursively Permute the two subtrees as it doesn't matter all node of which subtree is added after the parent node.
	generateBSTSequences(node->left, n, seq, seqs);
	generateBSTSequences(node->right, n, seq, seqs);
	// We need to permute the two subtrees only if the node (parent) has exactly two non-null children,
	// This is to avoid a case where only one child is non-null in which case we would end up generating duplicates sequences.
	if(node->left && node->right)
	{
		seq.resize(cnt);
		generateBSTSequences(node->right, n, seq, seqs);
		generateBSTSequences(node->left, n, seq, seqs);
	}
}


template<BTNodeType Node>
static std::size_t BTNodeGetCount(Node* node) noexcept
{
	std::size_t i = 0;
	BTNodeInOrderTraverse(node, [&i]([[maybe_unused]] Node* _) noexcept
	{
		i += 1;
	});
	return i;
}

template<typename T>
static std::vector<std::vector<T>> generateBSTSequences(BTNode<T>* node) noexcept
{
	std::vector<std::vector<T>> seqs;
	std::vector<T> seq;
	std::size_t n = BTNodeGetCount(node);
	generateBSTSequences(node, n, seq, seqs);
	// NRVO would kick-in here
	return seqs;
}

template<typename T>
static void generateWeaves(const std::span<const T> s1, const std::span<const T> s2,
				std::size_t index1, std::size_t index2,
				std::vector<T>& buffer,
				std::vector<std::vector<T>>& seqs)
{
	if(buffer.size() >= (s1.size() + s2.size()))
	{
		seqs.push_back(buffer);
		return;
	}
	for(auto i = index1; i < s1.size(); ++i)
	{
		buffer.push_back(s1[i]);
		generateWeaves(s1, s2, i + 1, index2, buffer, seqs);
		buffer.pop_back();
	}
	for(auto i = index2; i < s2.size(); ++i)
	{
		buffer.push_back(s2[i]);
		generateWeaves(s1, s2, index1, i + 1, buffer, seqs);
		buffer.pop_back();
	}
}

template<typename T>
static void generateWeaves(const std::span<const T> s1, const std::span<const T> s2, std::vector<std::vector<T>>& seqs)
{
	std::vector<T> buffer;
	generateWeaves(s1, s2, 0, 0, buffer, seqs);
}

template<typename T>
static void push_front(std::vector<T>& v, const T& value)
{
	v.insert(v.begin(), value);
}

template<typename T>
static std::vector<std::vector<T>> generateBSTSequences2(BTNode<T>* node) noexcept
{
	// Always think theoretically, so if we have null node that means there is only one sequence which
	// could lead to a null node (or an empty bst), it is the empty sequence.
	if(!node)
	{
		std::vector<std::vector<T>> seqs;
		seqs.push_back(std::vector<T>({ }));
		return seqs;
	}
	std::vector<std::vector<T>> leftSeqs = generateBSTSequences2(node->left);
	std::vector<std::vector<T>> rightSeqs = generateBSTSequences2(node->right);
	// Otherwise weave the sequences in all possible pair combinations
	std::vector<std::vector<T>> seqs;
	for(const auto& seq1 : leftSeqs)
		for(const auto& seq2 : rightSeqs)
			generateWeaves(std::span { seq1 }, std::span { seq2 }, seqs);
	// Parent node value is always added first in a valid BST
	for(auto& seq : seqs)
		push_front(seq, node->value);
	return seqs;
}

struct Solution1
{
	template<typename T>
	std::vector<std::vector<T>> operator()(BTNode<T>* node) noexcept
	{
		std::cout << "Solution 1: \n";
		return generateBSTSequences(node);
	}
};

struct Solution2
{
	template<typename T>
	std::vector<std::vector<T>> operator()(BTNode<T>* node) noexcept
	{
		std::cout << "Solution 2: \n";
		return generateBSTSequences2(node);
	}
};

template<typename Sol, typename T>
static void runBSTSequences(BTNode<T>* bst) noexcept
{
	std::cout << "Input Binary Tree: " << *bst << "\n";
	std::vector<std::vector<T>> sequences = Sol { }(bst);
	std::cout << "Sequence count: " << sequences.size() << "\n";
	for(const auto& seq : sequences)
		std::cout << seq << "\n";
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
	std::cout << "**----BST Sequences----**\n";
	std::cout << "Input Set 1: \n";
	runBSTSequences<Solution1>(bst);
	runBSTSequences<Solution2>(bst);
	std::cout << "Input Set 2: \n";
	auto* node = BTNodeGetRightMost(bst);
	node->right = new BTNode<T> { 2355 };
	node->right->right = new BTNode<T> { 5443 };
	node->right->right->right = new BTNode<T> { -443 };
	node->right->right->right->right = new BTNode<T> { -6490 };
	runBSTSequences<Solution1>(bst);
	runBSTSequences<Solution2>(bst);
	destroyBST(bst);
	std::cout << "Input Set 3: \n";
	{
		auto* node = new BTNode<T> { 2 };
		node->left = new BTNode<T> { 1 };
		node->right = new BTNode<T> { 3 };
		node->right->right = new BTNode<T> { 4 };
		runBSTSequences<Solution1>(node);
		runBSTSequences<Solution2>(node);
		destroyBST(node);
	}
}

int main()
{
	run<int>({ 100, 2, 3, 4, -1, -2, -3 });
	return 0;
}
