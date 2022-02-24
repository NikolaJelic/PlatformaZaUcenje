#pragma once

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>
namespace util {

template <typename T>
std::ostream& write_array(std::ostream& os, std::vector<T> const& list) {
	for (size_t i = 0; i < list.size() - 1; ++i) {
		os << list[i] << '|';
	}
	os << list[list.size() - 1] << '\n';
	return os;
}

template <typename T>
int enum_to_int(T const& t) {
	return static_cast<int>(t);
}

std::vector<std::string> get_lines(const std::string& path);
std::string_view trim_whitespace(std::string_view str);
std::unordered_map<std::string, std::string> insert_pairs(std::vector<std::string> file_lines);
std::string generate_chat_name(std::string const& sender, std::string const& receiver);
std::vector<std::string> parse_list(std::string list, char delim);
std::string get_datetime();
void create_file(std::string const& path);
void extract_map(std::unordered_map<std::string, std::string> const& map, std::string const& key, std::string& value);
} // namespace util
