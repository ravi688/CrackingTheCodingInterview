#include <iostream>
#include <stack> // for std::stack<>

template<typename T>
static void moveDisks(std::size_t n, std::stack<T>& src, std::stack<T>& dst, std::stack<T>& buf)
{
	if(n == 0)
		return;
	// Move n - 1 disks from src to buf using dst as buffer
	moveDisks(n - 1, src, buf, dst);

	// Move the last disk from src to dst
	T value = std::move(src.top());
	src.pop();
	dst.push(std::move(value));

	// Move n - 1 disks from buf to dst using src as buffer
	moveDisks(n - 1, buf, dst, src);
}

template<typename T>
static std::stack<T> generateInput()
{
	std::stack<T> s;
	s.push(100);
	s.push(99);
	s.push(40);
	s.push(20);
	s.push(10);
	s.push(9);
	s.push(8);
	s.push(7);
	s.push(6);
	s.push(5);
	s.push(4);
	s.push(3);
	s.push(2);
	s.push(1);
	return s;
}

template<typename T>
static std::ostream& operator<<(std::ostream& stream, const std::stack<T> s)
{
	stream << "{ [top] ";
	std::stack<T> copyS { s };
	while(!copyS.empty())
	{
		stream << copyS.top();
		copyS.pop();
		if(!copyS.empty())
			stream << ", ";
	}
	stream << " }";
	return stream;
}

int main()
{
	std::stack<int> src = generateInput<int>();
	std::stack<int> dst;
	std::stack<int> buf;
	std::cout << "Input: \n";
	std::cout << "src = " << src << "\n";
	std::cout << "dst = " << dst << "\n";
	std::cout << "buf = " << buf << "\n";
	moveDisks(src.size(), src, dst, buf);
	std::cout << "Output: \n";
	std::cout << "src = " << src << "\n";
	std::cout << "dst = " << dst << "\n";
	std::cout << "buf = " << buf << "\n";
	return 0;
}
