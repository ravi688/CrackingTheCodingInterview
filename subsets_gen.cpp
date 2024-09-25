#include <iostream> // for std::cout
#include <vector> // for std::vector
#include <concepts> // std::convertible_to
#include <functional> // for std::function type erasure

template<typename T>
concept ReadOnlyContainer = requires(const T& container)
{
	container.cbegin();
	container.cend();
	{ container.size() } -> std::convertible_to<std::size_t>;
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

void traverse_subsets(const std::vector<int>& set, std::size_t index, std::vector<int>& buf, std::function<void(void)>& callback) noexcept
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

std::vector<std::vector<int>> get_subsets(const std::vector<int>& set) noexcept
{
	std::vector<std::vector<int>> subsets;
	subsets.reserve(1 << set.size());
	std::vector<int> buf;
	buf.reserve(set.size());
	// NOTE: here std::function<void> results in in-complete type definition, so we are using std::function<void(void)>
	std::function<void(void)> fn = [&buf, &subsets]()
	{
		std::vector<int> cloned = buf;
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
