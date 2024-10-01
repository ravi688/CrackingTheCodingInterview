#include <iostream> // for std::cout
#include <vector> // for std::vector
#include <concepts> // std::convertible_to
#include <functional> // for std::function type erasure
#include <utility> // for std::declval
#include <cstdint> // for std::uint64_t

template<typename T>
concept ReadOnlyContainer = requires(const T& container)
{
	container.cbegin();
	container.cend();
	{ container.size() } -> std::integral;
};


template<ReadOnlyContainer T>
std::ostream& operator<<(std::ostream& stream, const T& container) noexcept
{
	stream << "{ ";
	for(decltype(container.size()) i = 0; auto& value : container)
	{
		stream << value;
		if((i + 1) != container.size())
			stream << ", ";
		++i;
	}
	stream << " }";
	return stream;
}

template<typename T>
concept ReadOnlyVector = requires(const T& vector)
{
	typename T::value_type;
	typename T::size_type;
	{ vector.size() } -> std::convertible_to<typename T::size_type>;
	vector.operator[](std::declval<typename T::size_type>());
};

// Time complexity: T(n) = 2 * T(n - 1) + c
template<ReadOnlyVector T>
void traverse_subsets(const T& set, typename T::size_type index, std::vector<typename T::value_type>& buf, std::function<void(void)>& callback) noexcept
{
	if(index == set.size())
	{
		callback();
		return;
	}
	buf.push_back(set[index]);
	traverse_subsets(set, index + 1, buf, callback);
	buf.pop_back();
	traverse_subsets(set, index + 1, buf, callback);
}

struct subset_traverser_1
{
	template<ReadOnlyVector T>
	void operator()(const T& set, std::vector<typename T::value_type>& buf, std::function<void(void)>& callback) noexcept
	{
		traverse_subsets(set, 0, buf, callback);
	}
};

// Time Complexity: T(n) = n * 2^n
template<ReadOnlyVector T>
void traverse_subsets2(const T& set, std::vector<typename T::value_type>& buf, std::function<void(void)>& callback) noexcept
{
	auto n = set.size();
	auto bitCount = static_cast<std::uint64_t>(1) << n;
	for(decltype(bitCount) i = 0; i < bitCount; ++i)
	{
		buf.clear();
		for(typename T::size_type j = 0; j < n; ++j)
			if(i & (static_cast<std::uint64_t>(1) << j))
				buf.push_back(set[j]);
		callback();
	}
}

struct subset_traverser_2
{
	template<ReadOnlyVector T>
	void operator ()(const T& set, std::vector<typename T::value_type>& buf, std::function<void(void)>& callback) noexcept
	{
		traverse_subsets2(set, buf, callback);
	}
};

template<typename T>
typename std::add_lvalue_reference<T>::type decllval()
{
	return *reinterpret_cast<T*>(nullptr);
}

template<typename S, typename T>
concept SubsetTraverser = ReadOnlyVector<T> && requires(S s, T t)
{
	std::is_default_constructible_v<S>;
	s(t, decllval<std::vector<typename T::value_type>>(), decllval<std::function<void(void)>>());
};

template<ReadOnlyVector T, SubsetTraverser<T> S = subset_traverser_1>
std::vector<std::vector<int>> get_subsets(const T& set) noexcept
{
	std::vector<std::vector<typename T::value_type>> subsets;
	subsets.reserve(1 << set.size());
	std::vector<typename T::value_type> buf;
	buf.reserve(set.size());
	// NOTE: here std::function<void> results in in-complete type definition, so we are using std::function<void(void)>
	std::function<void(void)> fn = [&buf, &subsets]()
	{
		std::vector<typename T::value_type> cloned = buf;
		subsets.push_back(std::move(cloned));
	};
	S { } (set, buf, fn);
	return subsets;
}

std::vector<std::vector<int>> get_subsets_int(const std::vector<int>& set) noexcept
{
	std::vector<std::vector<int>> subsets;
	std::size_t n = set.size();
	subsets.reserve(1 << n);
	std::vector<int> buf;
	buf.reserve(n);
	for(std::size_t i = 0; i < (1 << n); ++i)
	{
		buf.clear();
		for(std::size_t j = 0; j < n; ++j)
			if(i & (1 << j))
				buf.push_back(set[j]);
		subsets.push_back(std::vector<int> { buf });
	}
	return subsets;
}

int main()
{
	std::vector<int> set = { 1, 2, 3 };
	for(int i = 0; i < 10; i++)
		set.push_back(i);
	std::cout << "Given set: " << set << std::endl;
	std::vector<std::vector<int>> subsets = get_subsets<std::vector<int>, subset_traverser_1>(set);
	std::cout << "Subsets: " << std::endl;
	for(auto& subset : subsets)
		std::cout << subset << std::endl;
	return 0;
}
