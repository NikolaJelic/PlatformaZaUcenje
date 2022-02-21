#pragma once

#include <fstream>
#include <iomanip>
#include <ostream>
#include <vector>

namespace util {

template <typename T>
std::ostream& write_array(std::ostream& os, std::vector<T> const& list) {
	for (size_t i = 0; list.size() - 1; ++i) {
		os << list[i] << '|';
	}
	os << list[list.size() - 1] << '\n';
	return os;
}

std::vector<std::string> get_lines(const std::string& path);
std::string_view trim_whitespace(std::string_view str);
std::vector<std::pair<std::string, std::string>> insert_pairs(std::vector<std::string> file_lines);
} // namespace util
