#include <iostream>
#include <span>
#include <array>
#include <vector>
#include <cstdint>
#include <unordered_map>

enum class Status : uint8_t
{
	// Project's build process is yet to be started
	Raw,
	// Project's build is in-progress
	Building,
	// Project is built successfully
	Built
};

struct Project
{
	Status status { Status::Raw };
	char id;
	std::vector<char> dependencies;
};

template<typename T>
static std::ostream& operator<<(std::ostream& stream, const std::vector<T>& values) noexcept
{
	stream << "{ ";
	for(std::size_t i = 0; const auto& value : values)
	{
		stream << value;
		if(++i < values.size())
			stream << ", ";
	}
	stream << " }";
	return stream;
}

template<typename T, typename U>
static std::ostream& operator<<(std::ostream& stream, const std::pair<T, U>& pair) noexcept;

template<typename T>
static std::ostream& operator<<(std::ostream& stream, const std::span<T>& values) noexcept
{
	stream << "{ ";
	for(std::size_t i = 0; const auto& value : values)
	{
		stream << value;
		if(++i < values.size())
			stream << ", ";
	}
	stream << " }";
	return stream;
}

template<typename T, typename U>
static std::ostream& operator<<(std::ostream& stream, const std::pair<T, U>& pair) noexcept
{
	stream << "{ " << pair.first << ", " << pair.second << " }";
	return stream;
}

static bool buildProjects(char startID, std::unordered_map<char, Project>& graph, std::vector<char>& buildOrder)
{
	auto& project = graph[startID];
	if(project.status == Status::Building)
		return false;
	if(project.status == Status::Built)
		return true;
	project.status = Status::Building;
	for(const auto& dependency : project.dependencies)
		buildProjects(dependency, graph, buildOrder);
	project.status = Status::Built;
	buildOrder.push_back(startID);
	return true;
}

static bool buildProjects(std::unordered_map<char, Project>& graph, std::vector<char>& buildOrder)
{
	for(const auto& pair : graph)
		if(!buildProjects(pair.first, graph, buildOrder))
			return false;
	return true;
}

static void run(const std::span<const char> projects, const std::span<const std::pair<char, char>> dependencies) noexcept
{
	// A graph representing dependencies would be a directed graph which may have cycles also, in which case
	// there would be no defined build order among the projects.

	std::cout << "Projects: " << projects << "\n";
	std::cout << "Dependencies: " << dependencies << "\n";

	// Build a dependency graph
	std::unordered_map<char, Project> graph;
	for(const auto& id : projects)
	{
		Project project;
		project.id = id;
		project.status = Status::Raw;
		graph.insert({ id, std::move(project) });
	}
	for(const auto& pair : dependencies)
		graph[pair.second].dependencies.push_back(pair.first);

	// Determine build order
	std::vector<char> buildOrder;
	if(!buildProjects(graph, buildOrder))
		std::cout << "No possible build order found\n";
	else
		std::cout << "Build Order: " << buildOrder << std::endl;
}

int main()
{
	constexpr std::array projects { 'a', 'b', 'c', 'd', 'e', 'f' };
	constexpr std::array<std::pair<char, char>, 5> dependencies {
	{
		{ 'a', 'd' },
		{ 'f', 'b' },
		{ 'b', 'd' },
		{ 'f', 'a' },
		{ 'd', 'c' }
	} };
	run(std::span { projects }, std::span { dependencies });
	return 0;
}
