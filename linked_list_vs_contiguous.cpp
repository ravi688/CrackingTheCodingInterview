#include <iostream> // for st::cout
#include <vector> // for std::vector
#include <array> // for std::array
#include <initializer_list> // for std::initializer_list<T>
#include <chrono> // for timing
#include <ranges>
#include <concepts>

template<typename T>
struct Node
{
	T value;
	Node<T>* next;
};

template<typename T>
struct Node2
{
	Node2<T>* next;
	T value;
};

template<template<typename T> typename NodeType, std::ranges::range InitListType, typename T = InitListType::value_type>
static NodeType<T>* createLinkedList(const InitListType& initList) noexcept
{
	NodeType<T>* insertPoint = nullptr, *head = nullptr;
	for(const T& value : initList)
	{
		NodeType<T>* node;
		if constexpr(std::same_as<NodeType<T>, Node<T>>)
			node = new NodeType<T> { value, nullptr };
		else
			node = new NodeType<T> { nullptr, value };
		if(insertPoint)
			insertPoint->next = node;
		else	
			head = node;
		insertPoint = node;
	}
	return head;
}

template<template<typename T> typename NodeType, typename T>
static void destroyLinkedList(NodeType<T>* node) noexcept
{
	while(node)
	{
		auto next = node->next;
		delete node;
		node = next;
	}
}

template<std::ranges::range InitListType, typename T = InitListType::value_type>
static T* createArray(const InitListType& initList) noexcept
{
	T* array = new T[initList.size()];
	T* saveArray = array;
	for(const T& value : initList)
		(*array++) = value;
	return saveArray;
}

template<typename T>
static void destroyArray(T* array) noexcept
{
	delete[] array;
}

template<template<typename T> typename NodeType, typename T>
static void touchLinkedList(NodeType<T>* node) noexcept
{
	while(node)
	{
		__attribute__((unused)) T value = node->value;
		node = node->next;
	}
}

template<typename T>
static void touchArray(T* array, std::size_t N) noexcept
{
	std::size_t i = 0;
	while(i < N)
	{
		__attribute__((unused)) T value = array[i];
		++i;
	}
}

typedef long unsigned int WORD;
static constexpr std::size_t INPUT_SIZE = 10000000;
static constexpr std::size_t RUN_COUNT = 10;

int main()
{
	std::vector<WORD> initList;
	for(std::size_t i = 0; i < INPUT_SIZE; ++i)
		initList.push_back(i);

	auto* head = createLinkedList<Node>(initList);
	auto* head2 = createLinkedList<Node2>(initList);
	auto* vec = createArray(initList);
	
	for(std::size_t i = 0; i < RUN_COUNT; ++i)
	{
		std::cout << "Run: " << i << std::endl;
		std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
		touchLinkedList(head);
		auto end = std::chrono::steady_clock::now();
		float elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - start).count();
		std::cout << "Linked List traversal time: " << elapsed << " ms" << std::endl;
		start = std::chrono::steady_clock::now();
		touchLinkedList(head2);
		end = std::chrono::steady_clock::now();
		elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - start).count();
		std::cout << "Linked List2 traversal time: " << elapsed << " ms " << std::endl;
		start = std::chrono::steady_clock::now();
		touchArray(vec, INPUT_SIZE);
		end = std::chrono::steady_clock::now();
		elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - start).count();
		std::cout << "Array traversal time: " << elapsed << " ms" << std::endl;
	}

	destroyLinkedList(head2);
	destroyLinkedList(head);
	destroyArray(vec);
	return 0;
}
