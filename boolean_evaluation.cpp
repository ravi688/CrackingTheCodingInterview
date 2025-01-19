#include <iostream>
#include <string_view>
#include <chrono>
#include <unordered_map>

static std::pair<std::size_t, std::size_t> evaluate(const std::string_view expression)
{
	if(expression.size() == 0)
		return { 0, 0 };
	if(expression.size() == 1)
	{
		if(expression[0] == '0')
			return { 0, 1 };
		else return { 1, 0 };
	}
	std::size_t numWaysTrue = 0;
	std::size_t numWaysFalse = 0;
	// Build the expression tree for each operator as root (of a sub-tree)
	for(std::size_t i = 0; const auto& ch : expression)
	{
		if(ch != '1' && ch != '0')
		{
			// Build the left expression tree
			auto leftResult = evaluate(expression.substr(0, i));
			// Build the right expression tree
			auto rightResult = evaluate(expression.substr(i + 1));
			switch(ch)
			{
				case '|':
					// The number of ways of making 'a | b' true =
					//	number of ways of making 'a' true * number of ways of making 'b' true
					// 	+ number of ways of making 'a' true * number of ways of making 'b' false
					// 	+ number of ways of making 'b' true * number of ways of making 'a' false
					numWaysTrue += leftResult.first * rightResult.first
							+ leftResult.first * rightResult.second + rightResult.first * leftResult.second;
					// The number of ways of making 'a | b' false =
					//	number of ways of making 'b' false * number of ways of making 'b' false
					numWaysFalse += leftResult.second * rightResult.second;
					break;
				case '&':
					// The number of ways of making 'a & b' true =
					//	number of ways of making 'a' true * number of ways of making 'b' true
					numWaysTrue += leftResult.first * rightResult.first;
					// The number of ways of making 'a & b' false =
					//	number of ways of making 'a' false * number of ways of making 'b' false
					//	+ number of ways of making 'b' false * number of ways of making 'a' true
					//	+ number of ways of making 'a' false * number of ways of making 'b' true
					numWaysFalse += leftResult.second * rightResult.second
							+ leftResult.second * rightResult.first + rightResult.second * leftResult.first;
					break;
				case '^':
					// The number of ways of making 'a ^ b' true =
					//	number of ways of making 'a' true * number of ways of making 'b' false
					//	+ number of ways of making 'b' true * number of ways of making 'a' false
					// The number of ways of making 'a ^ b' false =
					//	number of ways of making 'a' true * number of ways of making 'b' true
					//	+ number of ways of making 'b' false * number of ways of making 'b' false
					numWaysTrue += leftResult.first * rightResult.second + rightResult.first * leftResult.second;
					numWaysFalse += leftResult.first * rightResult.first + leftResult.second * rightResult.second;
					break;
			}
		}
		++i;
	}
	return { numWaysTrue, numWaysFalse };
}

template<typename hasher = std::hash<std::string_view>>
static std::pair<std::size_t, std::size_t> evaluateMemo(const std::string_view expression,
				std::unordered_map<std::string_view, std::pair<std::size_t, std::size_t>, hasher>& memo)
{
	if(expression.size() == 0)
		return { 0, 0 };
	if(expression.size() == 1)
	{
		if(expression[0] == '0')
			return { 0, 1 };
		else return { 1, 0 };
	}
	if(auto it = memo.find(expression); it != memo.end())
		return it->second;
	std::size_t numWaysTrue = 0;
	std::size_t numWaysFalse = 0;
	// Build the expression tree for each operator as root (of a sub-tree)
	for(std::size_t i = 0; const auto& ch : expression)
	{
		if(ch != '1' && ch != '0')
		{
			// Build the left expression tree
			auto leftResult = evaluateMemo<hasher>(expression.substr(0, i), memo);
			// Build the right expression tree
			auto rightResult = evaluateMemo<hasher>(expression.substr(i + 1), memo);
			switch(ch)
			{
				case '|':
					// The number of ways of making 'a | b' true =
					//	number of ways of making 'a' true * number of ways of making 'b' true
					// 	+ number of ways of making 'a' true * number of ways of making 'b' false
					// 	+ number of ways of making 'b' true * number of ways of making 'a' false
					numWaysTrue += leftResult.first * rightResult.first
							+ leftResult.first * rightResult.second + rightResult.first * leftResult.second;
					// The number of ways of making 'a | b' false =
					//	number of ways of making 'b' false * number of ways of making 'b' false
					numWaysFalse += leftResult.second * rightResult.second;
					break;
				case '&':
					// The number of ways of making 'a & b' true =
					//	number of ways of making 'a' true * number of ways of making 'b' true
					numWaysTrue += leftResult.first * rightResult.first;
					// The number of ways of making 'a & b' false =
					//	number of ways of making 'a' false * number of ways of making 'b' false
					//	+ number of ways of making 'b' false * number of ways of making 'a' true
					//	+ number of ways of making 'a' false * number of ways of making 'b' true
					numWaysFalse += leftResult.second * rightResult.second
							+ leftResult.second * rightResult.first + rightResult.second * leftResult.first;
					break;
				case '^':
					// The number of ways of making 'a ^ b' true =
					//	number of ways of making 'a' true * number of ways of making 'b' false
					//	+ number of ways of making 'b' true * number of ways of making 'a' false
					// The number of ways of making 'a ^ b' false =
					//	number of ways of making 'a' true * number of ways of making 'b' true
					//	+ number of ways of making 'b' false * number of ways of making 'b' false
					numWaysTrue += leftResult.first * rightResult.second + rightResult.first * leftResult.second;
					numWaysFalse += leftResult.first * rightResult.first + leftResult.second * rightResult.second;
					break;
			}
		}
		++i;
	}
	std::pair<std::size_t, std::size_t> result = { numWaysTrue, numWaysFalse };
	memo.insert({ expression, result });
	return result;
}

template<typename hasher = std::hash<std::string_view>>
static std::pair<std::size_t, std::size_t> evaluateMemo(const std::string_view expression)
{
	std::unordered_map<std::string_view, std::pair<std::size_t, std::size_t>, hasher> memo;
	return evaluateMemo<hasher>(expression, memo);
}

struct Solution1
{
	auto operator()(const std::string_view expression)
	{
		return evaluate(expression);
	}
};

struct Solution2
{
	auto operator()(const std::string_view expression)
	{
		return evaluateMemo<>(expression);
	}
};

// For this optimization to work correctly in general case , all string_view(s) which are identical by value must have identical base address (pointer returned by data() method).
// For memoization purpose, this would still work but may lead to inconsistent performance results if there are many string_view(s) identical by value but different by their base addresses (data()).
struct SVEfficientHash
{
	std::size_t operator()(const std::string_view str) const
	{
		auto h1 = std::hash<std::size_t>{}(str.size());
		auto h2 = std::hash<const char*>{}(str.data());
		return h1 ^ h2;
	}
};

struct Solution3
{
	auto operator()(const std::string_view expression)
	{
		return evaluateMemo<SVEfficientHash>(expression);
	}
};

template<typename Sol>
static void runNumWays(const std::string_view expression)
{
	auto start = std::chrono::steady_clock::now();
	auto result = Sol { } (expression);
	auto end = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - start).count();
	std::cout << "Time taken: " << elapsed << " ms\n";
	std::cout << "Num ways of true = " << result.first << "\n";
	std::cout << "Num ways of false = " << result.second << "\n";
}

static void run(const std::string_view expression)
{
	static std::size_t runCount = 0;
	std::cout << "------------RUN: " << runCount << " ------------\n";
	++runCount;
	std::cout << "INPUT: " << expression << "\n";
	std::cout << "**Solution no 1**\n";
	runNumWays<Solution1>(expression);
	std::cout << "**Solution no 2**\n";
	runNumWays<Solution2>(expression);
	std::cout << "**Solution no 3**\n";
	runNumWays<Solution3>(expression);
}

int main()
{
	run( "0&0&0&1^1|0");
	run( "0&0&0&1^1|0|0^1&1&0|1^0^0^0^0|1&1");
	run( "0&0&0&1^1|0");
	run("1^0|0|1");
	return 0;
}

