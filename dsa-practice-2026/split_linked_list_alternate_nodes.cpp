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
	const auto [s1, s2] = splitLinkedList(l);

	printLinkedList(s1);
	printLinkedList(s2);
	return 0;
}
