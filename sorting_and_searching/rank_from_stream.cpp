#include <iostream>
#include <optional> // for std::optional<>
#include <set> // for std::multiset<>
#include <vector> // for std::vector<>
#include <unordered_map> // for std::unordered_multimap<>
#include <span> // for std::span<>
#include <algorithm> // for std::for_each
#include <chrono> // for timing

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
	protected:
		using RankData = std::pair<T, std::size_t>;
		std::vector<RankData> m_data;
	public:
		// Time complexity: Linear
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
		// Time complexity: Linear
		virtual std::optional<std::size_t> getRankOfNumber(const T& value) noexcept override
		{
			auto it = std::ranges::find_if(m_data, [&](const auto& _value) noexcept { return _value.first == value; });
			if(it == m_data.end())
				return { };
			return { it->second };
		}
};

template<typename T>
class Solution3 : public Solution2<T>
{
	private:
		std::unordered_multimap<T, std::size_t> m_lookupTable;
	public:
		// Time complexity: Linear
		virtual void track(const T& value) noexcept override
		{
			Solution2<T>::track(value);
			m_lookupTable.insert({ value, Solution2<T>::m_data.size() - 1 });
		}
		// Time complexity: Constant
		virtual std::optional<std::size_t> getRankOfNumber(const T& value) noexcept override
		{
			auto it = m_lookupTable.find(value);
			if(it == m_lookupTable.end())
				return { };
			return Solution2<T>::m_data[it->second].second;
		}
};

template<typename V, typename T>
concept VisitorPred = requires(V& callable, T& value)
{
	{ callable(value) } -> std::convertible_to<bool>;
};

template<typename T>
class RankBSTNode
{
	private:
		T m_value;
		// Number of duplicates of this nodes including itself
		std::size_t m_duplicateCount { 1 };
		// Number of nodes in its subtree including itself
		std::size_t m_count { 1 };
		RankBSTNode<T>* m_left { };
		RankBSTNode<T>* m_right { };
	public:
		RankBSTNode(const T& value, std::size_t dupCount = 1) : m_value(value), m_duplicateCount(dupCount) { }
		RankBSTNode<T>* getLeft() noexcept { return m_left; }
		RankBSTNode<T>* getRight() noexcept { return m_right; }
		std::size_t getDuplicateCount() const noexcept { return m_duplicateCount; }
		T& getValue() noexcept { return m_value; }
		const T& getValue() const noexcept { return m_value; }
		std::size_t getCount() const noexcept { return m_count; }
		RankBSTNode<T>* createLeft(const T& value)
		{
			m_left = new RankBSTNode<T>(value);
			return m_left;
		}
		RankBSTNode<T>* createRight(const T& value, std::size_t dupCount = 1)
		{
			m_right = new RankBSTNode<T>(value, dupCount);
			return m_right;
		}
		RankBSTNode<T>* add(const T& value, std::size_t dupCount = 1)
		{
			m_count += 1;
			if(m_value <= value)
			{
				if(m_value == value)
				{
					m_duplicateCount += 1;
					dupCount += 1;
				}
				return m_right ? m_right->add(value, dupCount) : createRight(value, dupCount);
			}
			else // if m_value > value
				return m_left ? m_left->add(value) : createLeft(value);
		}
		template<VisitorPred<RankBSTNode<T>*> V>
		bool inorderTraverse(const V& visitor)
		{
			if(m_left)
				if(!m_left->inorderTraverse(visitor))
					return false;
			if(!visitor(this))
				return false;
			if(m_right)
				if(!m_right->inorderTraverse(visitor))
					return false;
			return true;
		}
		std::optional<std::size_t> getRank(const T& value) const
		{
			if(m_value == value)
				return { (m_left ? m_left->getCount() : 0) + m_duplicateCount - 1 };
			if(m_value > value)
			{
				if(m_left)
					return m_left->getRank(value);
				return { };
			}
			// else if m_value < value
			return { (m_left ? m_left->getCount() : 0) + 1 + (m_right ? m_right->getRank(value).value_or(0) : 0) };
		}
};

template<typename T>
class RankBST
{
	public:
		using node_type = RankBSTNode<T>;
	private:
		RankBSTNode<T>* m_root { };
		static void deleteNodeRecursive(RankBSTNode<T>* node)
		{
			if(!node) return;
			deleteNodeRecursive(node->getLeft());
			deleteNodeRecursive(node->getRight());
			delete node;
		}
	public:
		~RankBST()
		{
			deleteNodeRecursive(m_root);
		}
		void add(const T& value)
		{
			if(!m_root)
			{
				m_root = new RankBSTNode<T>(value);
				return;
			}
			m_root->add(value);
		}
		std::optional<std::size_t> getRank(const T& value)
		{
			std::size_t rank = 0;
			bool isFound = false;
			typename RankBST<T>::node_type* foundNode = nullptr;
			m_root->inorderTraverse([&rank, &foundNode, &value](const auto& node)
			{
				const auto& _value = node->getValue();
				if(_value < value)
					++rank;
				else // if _value >= value
				{
					if(_value == value)
						foundNode = node;
					return false;
				}
				return true;
			});
			if(!foundNode) return { };
			return { rank + foundNode->getDuplicateCount() - 1 };
		}
		std::optional<std::size_t> getRank2(const T& value)
		{
			if(!m_root) return { };
			return m_root->getRank(value);
		}
};

template<typename T>
class Solution4 : public ITracker<T>
{
	private:
		RankBST<T> m_rankBST;
	public:
		// Time Complexity: Logarithmic
		virtual void track(const T& value) noexcept override
		{
			m_rankBST.add(value);
		}
		// Time Complexity: Linear
		virtual std::optional<std::size_t> getRankOfNumber(const T& value) noexcept override
		{
			return m_rankBST.getRank(value);
		}
};

// Most efficient as it has both the track() and getRankOfNumber() in logarithmic time complexity.
template<typename T>
class Solution5 : public ITracker<T>
{
	private:
		RankBST<T> m_rankBST;
	public:
		// Time Complexity: Logarithmic
		virtual void track(const T& value) noexcept override
		{
			m_rankBST.add(value);
		}
		// Time Complexity: Logarithmic
		virtual std::optional<std::size_t> getRankOfNumber(const T& value) noexcept override
		{
			return m_rankBST.getRank2(value);
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
	float averageTrackTime = 0;
	float averageGetRankTime = 0;
	for(std::size_t i = 0; const auto& value : values)
	{
		std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
		sol.track(value);
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - start).count();
		averageTrackTime += elapsed;
		++i;
		std::for_each(values.begin(), std::next(values.begin(), i), [&sol, &averageGetRankTime](const auto& value)
		{
			auto start = std::chrono::steady_clock::now();
			auto rank = sol.getRankOfNumber(value);
			auto end = std::chrono::steady_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - start).count();
			averageGetRankTime += elapsed;
			std::cout << "{ " << value << ", " << rank << " }, ";
		});
		std::cout << "\n";
	}
	averageTrackTime /= values.size();
	std::cout << "Average time taken by track(): " << averageTrackTime << " ms\n";
	averageGetRankTime /= (values.size() * (values.size() + 1) / 2.0f);
	std::cout << "Average time taken by getRankOfNumber(): " << averageGetRankTime << " ms\n";
}

template<typename T>
static void run(const std::span<T> values)
{
	std::cout << "**Solution no 1**\n";
	runRankFromStream<Solution1, T>(values);
	std::cout << "**Solution no 2**\n";
	runRankFromStream<Solution2, T>(values);
	std::cout << "**Solution no 3**\n";
	runRankFromStream<Solution3, T>(values);
	std::cout << "**Solution no 4**\n";
	runRankFromStream<Solution4, T>(values);
	std::cout << "**Solution no 5**\n";
	runRankFromStream<Solution5, T>(values);
}

int main()
{
	std::vector<int> values { 4, 32, 29, 4, 10, 45, 50, 60, 1, 3, 4, 1, 0 };
	run<int>(values);
	return 0;
}
