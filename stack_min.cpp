#include <iostream>
#include <vector>
#include <exception>
#include <initializer_list>

template<typename T>
class StackMin
{
	std::vector<T> m_minTrace;
	std::vector<T> m_values;
	public:
		void push(T value)
		{
			if(m_minTrace.empty())
				m_minTrace.push_back(value);
			else if(m_minTrace.back() > value)
				m_minTrace.push_back(value);
			else
				m_minTrace.push_back(m_minTrace.back());
			m_values.push_back(value);
		}

		T& top()
		{
			if(empty())
				throw std::out_of_range("Attempted to call top() inside an empty stack");
			return m_values.back();
		}

		T& min()
		{
			if(empty())
				throw std::out_of_range("Attempted to call min() inside an empty stack");
			return m_minTrace.back();
		}

		void pop()
		{
			if(empty())
				throw std::out_of_range("Attempted to call pop() inside an empty stack");
			m_minTrace.pop_back();
			m_values.pop_back();
		}

		std::size_t size()
		{
			return m_values.size();
		}

		bool empty() { return size() == 0; }
};

template<typename T>
static std::ostream& operator<<(std::ostream& stream, std::initializer_list<T> values)
{
	stream << "{ ";
	for(decltype(values.size()) i = 0; const auto& value : values)
	{
		stream << value;
		if(++i != values.size())
			stream << ", ";
	}
	stream << " }";
	return stream;
}

template<typename T>
static void run(std::initializer_list<T> values)
{
	std::cout << "Input: " << values << "\n";
	StackMin<T> stack;
	for(const auto& value : values)
		stack.push(value);
	std::cout << "Output: { ";
	while(!stack.empty())
	{
		std::cout << "[ " << stack.min() << ", " << stack.top() << " ]";
		stack.pop();
		if(stack.size())
			std::cout << ", ";
	}
	std::cout << std::endl;
}

int main()
{
	run<int>({ 4, 5, 1, 7, 8, 4, 2, 4, 5, 6, 7 });
	return 0;
}
