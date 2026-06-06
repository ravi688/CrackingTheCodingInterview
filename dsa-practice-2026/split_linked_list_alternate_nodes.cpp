#include <initializer_list> // for std::initializer_list
#include <utility> // for std::pair
#include <iostream> // for std::cout
#include <cassert> // for assert

template<typename T>
struct LinkedListNode
{
	T value;
	LinkedListNode<T>* next;
};

template<typename T>
std::pair<LinkedListNode<T>*, LinkedListNode<T>*> splitLinkedList(LinkedListNode<T>* l)
{
	LinkedListNode<T>* secondHead = nullptr;
	LinkedListNode<T>* secondTail = nullptr;
	
	auto curr = l;
	LinkedListNode<T>* prev = nullptr;
	bool alternate = false;
	
	while (curr)
	{
		if (alternate)
		{
			if (!secondHead)
				secondHead = secondTail = curr;
			else
			{
				secondTail->next = curr;
				secondTail = curr;
			}
			prev->next = curr->next;
		}

		prev = curr;
		curr = curr->next;
		alternate = !alternate;
	}
	
	if (secondTail)
		secondTail->next = nullptr;
	
	return { l, secondHead };
}

template<typename T>
std::pair<LinkedListNode<T>*, LinkedListNode<T>*> splitLinkedList2(LinkedListNode<T>* l)
{
	LinkedListNode<T>* aHead = nullptr,
		*aTail = nullptr,
		*bHead = nullptr,
		*bTail = nullptr;

	bool isAlternate = false;
	auto* head = l;
	while(head)
	{
		if(isAlternate)
		{
			if(!bHead)
				bHead = bTail = head;
			else
			{
				bTail = bTail->next = head;
			}
		}
		else
		{
			if(!aHead)
				aHead = aTail = head;
			else
			{
				aTail = aTail->next = head;
			}
		}
		isAlternate = !isAlternate;
		head = head->next;
	}
	if(aTail)
		aTail->next = nullptr;
	if(bTail)
		bTail->next = nullptr;
	return { aHead, bHead };
}

template<typename T>
void printLinkedList(const LinkedListNode<T>* l)
{
	auto s = l;
	while(s != nullptr)
	{
		std::cout << s->value << (s->next ? ", " : " ");
		s = s->next;
	}
	std::cout << std::endl;
}

template<typename T>
LinkedListNode<T>* createLinkedList(std::initializer_list<T> a)
{
	if(a.size() == 0) return nullptr;
	auto it = a.begin();
	LinkedListNode<T>* s = new LinkedListNode<T> { *it, nullptr };
	++it;
	auto* l = s;
	for(; it != a.end(); ++it)
	{
		s->next = new LinkedListNode<T> { *it, nullptr };
		s = s->next;
	}
	return l;
}

int main(int argc, const char** argv)
{
	auto* l = createLinkedList({ 1, 2, 3, 4, 5, 6, 7 });
	std::cout << "Given Linked List: \n";
	printLinkedList(l);

	std::cout << "Splitted linked lists: \n";
	std::cout << "**--------- Solution no 1 -----------**\n";
	const auto [s1, s2] = splitLinkedList(l);

	printLinkedList(s1);
	printLinkedList(s2);

	std::cout << "**--------- Solution no 2 -----------**\n";

	l = createLinkedList({1, 2, 3, 4, 5, 6, 7 });
	const auto [_s1, _s2] = splitLinkedList2(l);

	printLinkedList(_s1);
	printLinkedList(_s2);

	return 0;
}
