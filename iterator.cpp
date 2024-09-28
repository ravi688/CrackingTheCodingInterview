#include <iostream>

class Iterator
{
    typedef int value_type;
    
    int m_counter;
    
    public:
    
    Iterator(int value) : m_counter(value) { }
    
    int operator*() noexcept
    {
        return m_counter;
    }
    int operator->() noexcept
    {
        return m_counter;
    }
    
    Iterator operator++() noexcept
    {
        ++m_counter;
        return *this;
    }
    
    bool operator==(const Iterator& iterator) noexcept
    {
        return m_counter == iterator.m_counter;
    }
    bool operator!=(const Iterator& iterator) noexcept
    {
        return m_counter != iterator.m_counter;
    }
};

template<typename iterator>
class Iteratable
{
private:
	iterator m_begin;
	iterator m_end;
public:
	Iteratable(iterator begin, iterator end) noexcept : m_begin(begin), m_end(end) { }
	iterator begin() noexcept { return m_begin; }
	iterator end() noexcept { return m_end; }
};

int main()
{
    Iteratable<Iterator> values = { Iterator { 0 }, Iterator { 10 } };
    
    for(int value : values)
        std::cout << value << ", ";
    
    return 0;
}
