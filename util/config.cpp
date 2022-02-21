#include "config.hpp"

std::filesystem::path Config::get_data_path(std::filesystem::path start) {
	start = std::filesystem::absolute(start);
	static constexpr std::string_view target = "data";
	while (!start.empty() && (start.parent_path() != start.root_directory())) {
		auto ret = start / target;
		if (std::filesystem::is_directory(ret)) {
			return ret;
		}
		start = start.parent_path();
	}
	return {};
}

std::string Config::get_path(std::string key) const {
	for (size_t i = 0; i < paths.size(); i++) {
		if (paths[i].first == key) {
			return (prefix / paths[i].second).generic_string();
		}
	}
	return {};
}

std::filesystem::path Config::get_prefix() const { return prefix; }

void Config::load_paths(const std::string& path) {
	try {
		paths = util::insert_pairs(util::get_lines((prefix / path).string()));
	} catch (std::invalid_argument e) {
		std::cout << e.what();
	}
}
