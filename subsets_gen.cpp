#include <iostream> // for std::cout
#include <vector> // for std::vector
#include <concepts> // std::convertible_to
#include <functional> // for std::function type erasure
#include <utility> // for std::declval

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

template<ReadOnlyVector T>
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
	traverse_subsets(set, 0, buf, fn);
	return subsets;
}

int main()
{
	std::vector<int> set = { 1, 2, 3 };
	std::cout << "Given set: " << set << std::endl;
	std::vector<std::vector<int>> subsets = get_subsets(set);
	std::cout << "Subsets: " << std::endl;
	for(auto& subset : subsets)
		std::cout << subset << std::endl;
	return 0;
}
