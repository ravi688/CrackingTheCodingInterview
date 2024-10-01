#include <iostream> // for std::cout
#include <vector> // for std::vector
#include <concepts> // std::convertible_to
#include <functional> // for std::function type erasure
#include <utility> // for std::declval
#include <cstdint> // for std::uint64_t
#include <chrono> // for std::chrono::time_point<std::chrono::steady_clock> std::chrono::duration, std::chrono::duration_cast etc.

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
		std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
		traverse_subsets(set, 0, buf, callback);
		auto end = std::chrono::steady_clock::now();
		auto timeElapsed = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end - start).count();
		std::cout << "ST1, Time Elapsed: " << timeElapsed << " ms" << std::endl;
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
		std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
		traverse_subsets2(set, buf, callback);
		auto end = std::chrono::steady_clock::now();
		auto timeElapsed = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end - start).count();
		std::cout << "ST2, Time Elapsed: " << timeElapsed << " ms" << std::endl;
	}
};


// NOTE: it just puts indices of the elements, not their values
// Time Complexity: T(n) = n * 2^n
template<ReadOnlyVector T> requires (std::integral<typename T::value_type>)
void traverse_subsets3(const T& set, std::vector<typename T::value_type>& buf, std::function<void(void)>& callback) noexcept
{
	auto n = set.size();
	auto bitCount = static_cast<std::uint64_t>(1) << n;
	for(decltype(bitCount) i = 0; i < bitCount; ++i)
	{
		buf.clear();
		for(typename T::value_type j = 0; j < n; ++j)
			if(i & (static_cast<std::uint64_t>(1) << j))
				buf.push_back(j);
		callback();
	}
}

struct subset_traverser_3
{
	template<ReadOnlyVector T>
	void operator ()(const T& set, std::vector<typename T::value_type>& buf, std::function<void(void)>& callback) noexcept
	{
		std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
		traverse_subsets3(set, buf, callback);
		auto end = std::chrono::steady_clock::now();
		auto timeElapsed = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end - start).count();
		std::cout << "ST3, Time Elapsed: " << timeElapsed << " ms" << std::endl;
	}
};

template<ReadOnlyVector T> requires (std::integral<typename T::value_type>)
void traverse_subsets4(const T& set, std::vector<typename T::value_type>& buf, std::function<void(void)>& callback) noexcept
{
	auto n = set.size();
	auto bitCount = static_cast<std::uint64_t>(1) << n;
	for(decltype(bitCount) i = 0; i < bitCount; ++i)
	{
		buf.clear();
		for(int j = 0; j < n; ++j)
			if(i & (static_cast<std::uint64_t>(1) << j))
				buf.push_back(j);
		callback();
	}
}

struct subset_traverser_4
{
	template<ReadOnlyVector T>
	void operator ()(const T& set, std::vector<typename T::value_type>& buf, std::function<void(void)>& callback) noexcept
	{
		std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
		traverse_subsets4(set, buf, callback);
		auto end = std::chrono::steady_clock::now();
		auto timeElapsed = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end - start).count();
		std::cout << "ST4, Time Elapsed: " << timeElapsed << " ms" << std::endl;
	}
};


template<ReadOnlyVector T> requires (std::integral<typename T::value_type>)
void traverse_subsets5(const T& set, std::vector<typename T::value_type>& buf, std::function<void(void)>& callback) noexcept
{
	auto n = set.size();
	auto bitCount = static_cast<std::uint64_t>(1) << n;
	for(decltype(bitCount) i = 0; i < bitCount; ++i)
	{
		buf.clear();
		for(unsigned int j = 0; j < n; ++j)
			if(i & (static_cast<std::uint64_t>(1) << j))
				buf.push_back(j);
		callback();
	}
}

struct subset_traverser_5
{
	template<ReadOnlyVector T>
	void operator ()(const T& set, std::vector<typename T::value_type>& buf, std::function<void(void)>& callback) noexcept
	{
		std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
		traverse_subsets5(set, buf, callback);
		auto end = std::chrono::steady_clock::now();
		auto timeElapsed = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end - start).count();
		std::cout << "ST5, Time Elapsed: " << timeElapsed << " ms" << std::endl;
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
std::vector<std::vector<typename T::value_type>> get_subsets(const T& set) noexcept
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

// Output on Core i5 8th gen

// With intType = int;
// Given set: { 1, 2, 3, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 }
// ST1, Time Elapsed: 3571.2 ms
// ST2, Time Elapsed: 6276.73 ms
// ST3, Time Elapsed: 9437.55 ms <--- slower than ST4 as it contains unsigned to signed conversion
// ST4, Time Elapsed: 5915.98 ms <--- faster than ST3!
// ST5, Time Elapsed: 9484.11 ms <--- same as ST3 as it also contains unsigned to signed conversion
//
// With intType = unsigned int;
// Given set: { 1, 2, 3, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 }
// ST1, Time Elapsed: 3553.49 ms
// ST2, Time Elapsed: 6367.11 ms
// ST3, Time Elapsed: 9557.46 ms <--- slower than ST5, though it doesn't contain any sign conversion but it does bit-width conversion
// ST4, Time Elapsed: 9564.46 ms <--- slower than ST5 as there is signed to unsigned conversion
// ST5, Time Elapsed: 6111.33 ms <--- faster than ST3 as there is no unsigned to signed conversion
//
// If we use 'unsigned int' in the inner loop in ST3, with intType = unsigned int, ST3 becomes as fast as ST5, because there would no bit-width conversion then I guess.



// Conclusion:
// I think convertion from unsigned to signed (or opposite), and bit-width conversion might be additional overheads
// And they can add significant difference in performance.


int main()
{
	using intType = unsigned int;
	std::vector<intType> set = { 1, 2, 3 };
	for(int i = 0; i < 20; i++)
		set.push_back(i);
	std::cout << "Given set: " << set << std::endl;
	auto result1 = get_subsets<std::vector<intType>, subset_traverser_1>(set);
	auto result2 = get_subsets<std::vector<intType>, subset_traverser_2>(set);
	auto result3 = get_subsets<std::vector<intType>, subset_traverser_3>(set);
	auto result4 = get_subsets<std::vector<intType>, subset_traverser_4>(set);
	auto result5 = get_subsets<std::vector<intType>, subset_traverser_5>(set);
//	std::cout << "Subsets: " << std::endl;
//	for(auto& subset : subsets)
//		std::cout << subset << std::endl;
	return 0;
}
