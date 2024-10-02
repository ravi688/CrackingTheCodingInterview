#include <iostream>
#include <vector> // for std::vector
#include <concepts> // for std::integral
#include <utility> // for std::declval
#include <functional> // for std::functional

template<typename T, typename... U>
concept same_as_any = (std::same_as<T, U> || ...);

template<typename T>
concept ReadOnlyVector = requires(const T& v)
{
	typename T::size_type;
	typename T::value_type;
	{ v.size() } -> std::integral;
	{ v.operator[](std::declval<typename T::size_type>()) } -> same_as_any<typename T::value_type, const typename T::value_type&>;
};

template<typename T>
concept BoolVector = requires(T& v)
{
	{ v.size() } -> std::integral;
	{ v.operator[](std::declval<decltype(v.size())>()) } -> std::convertible_to<bool>;
};

template<ReadOnlyVector T, BoolVector B>
void traverse_permutations(const T& v, std::vector<typename T::value_type>& p, B& choosen, std::function<void(void)>& callback) noexcept
{
	if(p.size() == v.size())
	{
		callback();
		return;
	}
	for(decltype(v.size()) i = 0; auto& value : v)
	{
		if(choosen[i])
		{
			++i;
			continue;
		}
		p.push_back(v[i]);
		choosen[i] = true;
		traverse_permutations(v, p, choosen, callback);
		p.pop_back();
		choosen[i++] = false;
	}
}

template<ReadOnlyVector T>
std::vector<std::vector<typename T::value_type>> get_permutations(const T& v) noexcept
{
	std::vector<std::vector<typename T::value_type>> permutations;
	std::vector<typename T::value_type> p;
	p.reserve(v.size());
	std::vector<bool> b(v.size(), false);
	std::function<void(void)> fn =  [&p, &permutations]()
	{
		permutations.push_back(std::vector<typename T::value_type> { p });
	};
	traverse_permutations(v, p, b, fn);
	return permutations;
}

template<ReadOnlyVector T>
std::ostream& operator<<(std::ostream& stream, const T& v) noexcept
{
	stream << "{ ";
	for(typename T::size_type i = 0; auto& value : v)
	{
		stream << v[i];
		if((i + 1) != v.size())
			stream << ", ";
		++i;
	}
	stream << " }";
	return stream;
}


int main(int argc, const char* argv[])
{
	std::vector<int> set = { 2, 3, 4 };
	std::cout << "Given set: " << set << std::endl;
	std::vector<std::vector<int>> permutations = get_permutations(set);
	for(auto& p : permutations)
		std::cout << p << std::endl;
	return 0;
}
