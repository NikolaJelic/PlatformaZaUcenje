#include "graph.hpp"
#include "course.hpp"

bool Graph::contains(std::string const& key) const { return nodes_edges.contains(key); }

void Graph::create_graph(std::vector<User> users) {
	for (auto const& user : users) {
		nodes_edges.insert({user.get_username(), user.get_friends()});
	}
}

std::vector<std::string> Graph::shortest_path(std::string const& root, std::string const& goal) {
	std::queue<std::string> queue{};
	std::vector<std::string> visited{};
	std::vector<std::pair<std::string, std::string>> path{};
	bool found = false;
	queue.push(root);
	visited.push_back(root);
	while (!queue.empty() && !found) {
		auto user = queue.front();
		queue.pop();
		auto connected = nodes_edges.find(user)->second;
		for (auto const& u : connected) {
			if (std::find(visited.begin(), visited.end(), u) == visited.end()) {
				path.push_back({user, u});
				if (u == goal) {
					found = true;
				}
				visited.push_back(u);
				queue.push(u);
			}
		}
	}
	if (found) {
		std::vector<std::string> ret{};
		std::reverse(path.begin(), path.end());
		std::string temp = goal;
		for (auto const& [prev, next] : path) {
			if (next == temp) {
				ret.push_back(temp);
				temp = prev;
			}
		}
		ret.push_back(root);
		std::reverse(ret.begin(), ret.end());
		return ret;
	} else {
		return {};
	}
}

size_t Graph::average_distance(std::string const& first_course, std::string const& second_course) {
	auto courses = Course::read_courses("data/courses.txt");
	std::vector<std::string> first{}, second{};
	for (auto const& course : courses) {
		if (course.get_code() == first_course) {
			first = course.get_students();
		} else if (course.get_code() == second_course) {
			second = course.get_students();
		}
	}
	std::vector<size_t> distances{};
	for (auto const& friend_first : first) {
		for (auto const& friend_second : second) {
			distances.push_back(shortest_path(friend_first, friend_second).size());
		}
	}
	size_t ret{};
	if (distances.size()) {
		ret = std::accumulate(distances.begin(), distances.end(), 0) / distances.size();
	}
	return ret;
}
