#include <vector>
#include <iostream>
#include <set>

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

LLNode* getNthNode(LLNode* node, std::size_t n)
{
	--n;
	while(n && node)
	{
		node = node->next;
		--n;
	}
	return node;
}

LLNode* getLastNode(LLNode* node)
{
	if(!node) return nullptr;

	while(node->next)
	{
		node = node->next;
	}
	return node;
}

// Solution no 1: Using a check table to note down which nodes have been visited.
// Time complexity: O(nlog(n) * n)
// Space complexity: O(n)
LLNode* detectCycle(LLNode* node)
{
	std::set<LLNode*> checkTable;
	while(node)
	{
		if(checkTable.contains(node))
			return node;
		checkTable.insert(node);
		node = node->next;
	}
	return nullptr;
}

// Solution no 2: Using tortoise and hare algorithm (fast and slow pointer)
// Time complexity: O(n)
// Space complexity: O(1)
LLNode* detectCycle2(LLNode* node)
{
	auto* fastNode = node;
	auto* slowNode = node;
	while(slowNode && fastNode)
	{
		slowNode = slowNode->next;
		if(fastNode->next)
			fastNode = fastNode->next->next;
		else
			break;
		// Cycle is detected
		if(slowNode == fastNode)
		{
			// Find the start of the cycle
			while(node != slowNode)
			{
				node = node->next;
				slowNode = slowNode->next;
			}
			return slowNode;
		}
	}
	return nullptr;
}

int main(int argc, const char** argv)
{
	auto* ll = createLL({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
	std::cout << "Given linked list: ";
	printLL(ll);

	// Create a cycle in the list.
	auto* node5 = getNthNode(ll, 5);
	auto* nodeEnd = getLastNode(ll);
	nodeEnd->next = node5;

	std::cout << "**--------- Solution no 1 --------**\n";
	LLNode* cycleNode = detectCycle(ll);
	if(cycleNode)
		std::cout << "Cycle node is: " << cycleNode->value << "\n";
	else
		std::cout << "No cycle detected" << "\n";
	
	std::cout << "**---------- Solution no 2 -------**\n";
	cycleNode = detectCycle2(ll);
	if(cycleNode)
		std::cout << "Cycle node is: " << cycleNode->value << "\n";
	else
		std::cout << "No cycle detected" << "\n";

	return EXIT_SUCCESS;
}
