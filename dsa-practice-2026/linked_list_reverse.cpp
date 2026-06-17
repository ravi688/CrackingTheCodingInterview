#include <vector>
#include <iostream>

// Singly linked list node
struct LLNode
{
	int value;
	LLNode* next = nullptr;
};


LLNode* createLL(const std::vector<int>& a)
{
	LLNode* head = nullptr;
	LLNode* prev = nullptr;
	for(const int e : a)
	{
		auto* node = new LLNode();
		if(!head)
			head = node;
		node->value = e;
		if(prev)
			prev->next = node;
		prev = node;
	}
	return head;
}

void destroyLL(LLNode* node)
{
	while(node)
	{
		auto* next = node->next;
		delete node;
		node = next;
	}
}

void printLL(const LLNode* node)
{
	while(node)
	{
		std::cout << node->value << " ";
		node = node->next;
	}
	std::cout << "\n";
}

// Solution no 1: Iteratively reverse the nodes
// Time complexity: O(n)
// Space complexity: O(1)
LLNode* reverseLL(LLNode* node)
{
	// n1 -> n2 -> n3
	// n2 -> n1 -> n3
	// n3 -> n2 -> n1
	
	LLNode* head = node;
	while(node && node->next)
	{
		auto* t = node->next->next;
		node->next->next = head;
		head = node->next;
		node->next = t;
	}
	return head;
}

// Solution no 2: Recursively reverse the sub-list
// Time complexity: O(n)
// Space complexity: O(n)
LLNode* reverseLLRecursive(LLNode* node)
{
	if(node == nullptr || node->next == nullptr)
		return node;
	LLNode* head = reverseLLRecursive(node->next);
	node->next->next = node;
	node->next = nullptr;
	return head;
}

int main(int argc, const char** argv)
{
	auto* ll = createLL({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
	std::cout << "Given linked list: ";
	printLL(ll);
	std::cout << "**------------Solution no 1------------**\n";
	ll = reverseLL(ll);
	std::cout << "Reversed linked list: ";
	printLL(ll);
	std::cout << "**------------Solution no 2------------**\n";
	ll = reverseLL(ll);
	ll = reverseLLRecursive(ll);
	std::cout << "Reversed linked list: ";
	printLL(ll);

	return EXIT_SUCCESS;
}
