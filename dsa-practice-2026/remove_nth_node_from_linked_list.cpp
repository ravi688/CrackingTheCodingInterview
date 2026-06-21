#include <vector>
#include <iostream>
#include <stdexcept>

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

LLNode* removeNthNodeFromEnd(LLNode* node, std::size_t n)
{
	if(n < 1)
		return nullptr;

	// Traverse to the nth pointer from the end of the linked list
	
	LLNode* nthFromStart = node;
	while(n && nthFromStart)
	{
		nthFromStart = nthFromStart->next;
		--n;
	}
	LLNode* nthFromEnd = node;
	LLNode* prev = nullptr;
	while(nthFromStart != nullptr)
	{
		prev = nthFromEnd;
		nthFromEnd = nthFromEnd->next;
		nthFromStart = nthFromStart->next;
	}

	std::cout << "nth from the end = " << nthFromEnd->value << "\n";

	// Remove the nth node from end

	if(prev)
	{
		prev->next = nthFromEnd->next;
		delete nthFromEnd;
		return node;
	}
	else
	{
		auto* newHead = nthFromEnd->next;
		delete nthFromEnd;
		return newHead;
	}
}

int main(int argc, const char** argv)
{
	auto* ll = createLL({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
	std::cout << "Given linked lists: \n";
	printLL(ll);

	if(argc < 2)
	{
		throw std::runtime_error("Not enough arguments are provided");
	}
	else
	{
		std::size_t n = std::stoul(argv[1]);
		std::cout << "Value to be removed, " <<  n << " st/th from the end\n";
		auto* newLL = removeNthNodeFromEnd(ll, n);
		std::cout << "New linked list: \n";
		printLL(newLL);
	}
	return EXIT_SUCCESS;
}
