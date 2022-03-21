#include "util.hpp"
#include <chrono>
#include <cstddef>
#include <filesystem>
#include <sstream>

// TODO throw exceptions instead of printing errors

std::vector<std::string> util::get_lines(const std::string& path) {
	std::vector<std::string> ret;
	std::string temp_line;
	if (auto file = std::ifstream(path)) {
		while (std::getline(file, temp_line)) {
			ret.push_back(temp_line);
		}
	} else {
		std::cout << "File couldn't be opened\n";
	}
	return ret;
}

std::unordered_map<std::string, std::string> util::insert_pairs(std::vector<std::string> file_lines) {
	std::unordered_map<std::string, std::string> key_value_pairs;
	for (const auto& str : file_lines) {
		if (auto delimeter_pos = str.find('='); delimeter_pos != std::string::npos) {
			std::string_view const key = trim_whitespace(str.substr(0, delimeter_pos));
			if (key.empty()) {
				throw std::invalid_argument("Key is empty");
			} else {
				std::string_view const value = trim_whitespace(str.substr(delimeter_pos + 1));
				key_value_pairs.insert({std::string(key), std::string(value)});
			}
		}
	}
	return key_value_pairs;
}

std::string_view util::trim_whitespace(std::string_view str) {
	if (str.empty()) {
		return {};
	}
	std::size_t start = 0;
	std::size_t finish = str.size();
	while (std::isspace(str[start])) {
		++start;
	}
	while (std::isspace(str[finish - 1])) {
		--finish;
	}
	if (finish == start) {
		return str;
	}
	return str.substr(start, finish - start);
}

std::string util::generate_chat_name(std::string const& sender, std::string const& receiver) {
	std::string name = (sender.compare(receiver) > 0 ? sender + '_' + receiver : receiver + '_' + sender);
	return name + ".txt";
}

std::vector<std::string> util::parse_list(std::string list, char delim) {
	std::vector<std::string> ret{};
	size_t start{}, end{};
	while (end <= list.length()) {
		if (list[end] == delim) {
			ret.push_back(list.substr(start, end - start));
			start = end + 1;
		} else if (end == list.length()) {
			ret.push_back(list.substr(start, end - start));
		}
		++end;
	}

	return ret;
}

std::string util::get_datetime() {
	auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::stringstream ss;
	ss << std::put_time(std::localtime(&time), "%Y-%m-%d %X");
	return ss.str();
}

void util::extract_map(std::unordered_map<std::string, std::string> const& map, std::string const& key,
					   std::string& value) {
	auto it = map.find(key);
	if (it == map.end()) {
		value = {};
		std::cout << "key couldn't be found.\n";
	} else {
		value = it->second;
	}
}
