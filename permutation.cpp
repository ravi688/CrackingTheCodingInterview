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
void traverse_permutations_1(const T& v, std::vector<typename T::value_type>& p, B& choosen, std::function<void(void)>& callback) noexcept
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
		traverse_permutations_1(v, p, choosen, callback);
		p.pop_back();
		choosen[i++] = false;
	}
}

// Iterative method

template<typename T>
T getNFactorial(T n) noexcept
{
	T result = 1;
	while(n > 1)
	{
		result *= n;
		--n;
	}
	return result;
}

template<ReadOnlyVector T>
void traverse_permutations_2(const T& v, std::vector<typename T::value_type>& p, std::function<void(void)>& callback) noexcept
{
	using size_type = typename T::size_type;
	size_type n = v.size();
	for(size_type i = 0; i < n; ++i)
		p.push_back(v[i]);
	size_type numPermutes = getNFactorial(n);
	for(size_type i = 0; i < numPermutes; ++i)
	{
		std::next_permutation(p.begin(), p.end());
		callback();
	}
}

struct permutation_traverser_1
{
	template<ReadOnlyVector T>
	void operator() (const T& v, std::vector<typename T::value_type>& p, std::function<void(void)>& callback) noexcept
	{
		std::vector<bool> b(v.size(), false);
		traverse_permutations_1(v, p, b, callback);
	}
};

struct permutation_traverser_2
{
	template<ReadOnlyVector T>
	void operator() (const T& v, std::vector<typename T::value_type>& p, std::function<void(void)>& callback) noexcept
	{
		traverse_permutations_2(v, p, callback);
	}
};

template<typename T>
typename std::add_lvalue_reference<T>::type decllval() noexcept
{
	return *reinterpret_cast<T*>(nullptr);
}

template<typename T, typename V>
concept PermutationTraverser = ReadOnlyVector<V> && std::is_default_constructible<T>::value && requires(T ps)
{
	ps.operator()(decllval<V>(), decllval<std::vector<typename V::value_type>>(), decllval<std::function<void(void)>>());
};

template<ReadOnlyVector T, PermutationTraverser<T> PS = permutation_traverser_1>
std::vector<std::vector<typename T::value_type>> get_permutations(const T& v) noexcept
{
	std::vector<std::vector<typename T::value_type>> permutations;
	std::vector<typename T::value_type> p;
	p.reserve(v.size());
	std::function<void(void)> fn =  [&p, &permutations]()
	{
		permutations.push_back(std::vector<typename T::value_type> { p });
	};
	PS { } (v, p, fn);
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
	std::cout << "Recursive Method: " << std::endl;
	std::vector<std::vector<int>> permutations = get_permutations(set);
	for(auto& p : permutations)
		std::cout << p << std::endl;
	std::cout << "Iterative Method: " << std::endl;
	permutations = get_permutations<decltype(set), permutation_traverser_2>(set);
	for(auto& p : permutations)
		std::cout << p << std::endl;
	return 0;
}
