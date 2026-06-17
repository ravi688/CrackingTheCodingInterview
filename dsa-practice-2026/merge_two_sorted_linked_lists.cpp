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

LLNode* MergeSorted(LLNode* l1, LLNode* l2)
{
	LLNode* head = nullptr;
	LLNode* merged = nullptr;
	while(l1 && l2)
	{
		if(l1->value < l2->value)
		{
			if(merged)
			{
				merged->next = l1;
				merged = merged->next;
			}
			else
			{
				merged = l1;
				head = merged;
			}
			auto* t = l1->next;
			l1->next = nullptr;
			l1 = t;
		}
		else
		{
			if(merged)
			{
				merged->next = l2;
				merged = merged->next;
			}
			else
			{
				merged = l2;
				head = merged;
			}
			auto* t = l2->next;
			l2->next = nullptr;
			l2 = t;
		}
	}

	while(l1)
	{
		merged->next = l1;
		auto* t = l1->next;
		l1->next = nullptr;
		l1 = t;
	}
	while(l2)
	{
		merged->next = l2;
		auto* t = l2->next;
		l2->next = nullptr;
		l2 = t;
	}
	return head;
}

int main(int argc, const char** argv)
{
	auto* ll1 = createLL({ 1, 3, 5, 7, 9 });
	auto* ll2 = createLL({ 2, 4, 6, 8, 10 });
	std::cout << "Given linked lists: \n";
	printLL(ll1);
	printLL(ll2);

	auto* merged = MergeSorted(ll1, ll2);
	std::cout << "Merged Sorted: ";
	printLL(merged);

	return EXIT_SUCCESS;
}
