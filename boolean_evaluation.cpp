#include <iostream>
#include <string_view>

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
	for(std::size_t i = 0; const auto& ch : expression)
	{
		if(ch != '1' && ch != '0')
		{
			auto leftResult = evaluate(expression.substr(0, i));
			auto rightResult = evaluate(expression.substr(i + 1));
			switch(ch)
			{
				case '|':
					numWaysTrue += leftResult.first * rightResult.first
							+ leftResult.first * rightResult.second + rightResult.first * leftResult.second;
					numWaysFalse += leftResult.second * rightResult.second;
					break;
				case '&':
					numWaysTrue += leftResult.first * rightResult.first;
					numWaysFalse += leftResult.second * rightResult.second
							+ leftResult.second * rightResult.first + rightResult.second * leftResult.first;
					break;
				case '^':
					numWaysTrue += leftResult.first * rightResult.second + rightResult.first * leftResult.second;
					numWaysFalse += leftResult.first * rightResult.first + leftResult.second * rightResult.second;
					break;
			}
		}
		++i;
	}
	return { numWaysTrue, numWaysFalse };
}

struct Solution1
{
	auto operator()(const std::string_view expression)
	{
		return evaluate(expression);
	}
};

template<typename Sol>
static void runNumWays(const std::string_view expression)
{
	auto result = Sol { } (expression);
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
}

int main()
{
	run( "0&0&0&1^1|0");
	run("1^0|0|1");
	return 0;
}

