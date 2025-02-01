#include <iostream>
#include <optional> // for std::optional<>
#include <set> // for std::set<>
#include <vector> // for std::vector<>
#include <span> // for std::span<>
#include <algorithm> // for std::for_each

// Tracker interface
template<typename T>
class ITracker
{
	public:
		virtual void track(const T& value) noexcept = 0;
		virtual std::optional<std::size_t> getRankOfNumber(const T& value) noexcept = 0;
};

// Use red black tree to maintain sorted order
template<typename T>
class Solution1 : public ITracker<T>
{
	private:
		std::multiset<T> m_data;
	public:
		// Time complexity: Logarithmic
		virtual void track(const T& value) noexcept override
		{
			m_data.insert(value);
		}
		// Time complexity: Logarithmic or Linear in the worst case because of std::distance(m_data.begin(), it)
		virtual std::optional<std::size_t> getRankOfNumber(const T& value) noexcept override
		{
			auto range = m_data.equal_range(value);
			if(range.first == range.second)
				return { };
			return { std::distance(m_data.begin(), range.first) + std::distance(range.first, range.second) - 1 };
		}
};

template<typename T>
class Solution2 : public ITracker<T>
{
	private:
		using RankData = std::pair<T, std::size_t>;
		std::vector<RankData> m_data;
	public:
		virtual void track(const T& value) noexcept override
		{
			std::size_t thisRank = 0;
			for(auto& rankData : m_data)
				if(value < rankData.first)
					rankData.second += 1;
				else if(value > rankData.first)
					thisRank += 1;
				else
				{
					rankData.second += 1;
					thisRank += 1;
				}
			m_data.push_back({ value, thisRank });
		}
		virtual std::optional<std::size_t> getRankOfNumber(const T& value) noexcept override
		{
			auto it = std::ranges::find_if(m_data, [&](const auto& _value) noexcept { return _value.first == value; });
			if(it == m_data.end())
				return { };
			return { it->second };
		}
};

template<typename T>
static std::ostream& operator<<(std::ostream& stream, const std::optional<T>& value)
{
	if(!value)
		stream << "<null>";
	else
		stream << *value;
	return stream;
}

template<template<typename> typename Sol, typename T>
static void runRankFromStream(const std::span<T> values)
{
	Sol<T> sol;
	for(std::size_t i = 0; const auto& value : values)
	{
		sol.track(value);
		++i;
		std::for_each(values.begin(), std::next(values.begin(), i), [&sol](const auto& value)
		{
			std::cout << "{ " << value << ", " << sol.getRankOfNumber(value) << " }, ";
		});
		std::cout << "\n";
	}
}

template<typename T>
static void run(const std::span<T> values)
{
	std::cout << "**Solution no 1**\n";
	runRankFromStream<Solution1, T>(values);
	std::cout << "**Solution no 2**\n";
	runRankFromStream<Solution2, T>(values);
}

int main()
{
	std::vector<int> values { 4, 32, 29, 4, 10, 45, 50, 60, 1, 3, 4, 1, 0 };
	run<int>(values);
	return 0;
}
