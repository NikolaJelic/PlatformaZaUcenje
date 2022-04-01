#pragma once

#include <map>
#include <numeric>
#include <queue>
#include <string>
#include <vector>
#include <course.hpp>
#include <user.hpp>

class Graph {
	std::map<std::string, std::vector<std::string>> nodes_edges{};

  public:
	bool contains(std::string const& key) const;
	void create_graph(std::vector<User> users);
	std::vector<std::string> shortest_path(std::string const& root, std::string const& goal);
	size_t average_distance(std::string const& first_course, std::string const& second_course);
};
