#pragma once

#include <cstddef>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include "util.hpp"

class Config {
	std::vector<std::pair<std::string, std::string>> paths{};
	std::filesystem::path prefix{};

  public:
	static std::filesystem::path get_data_path(std::filesystem::path start);
	Config(std::filesystem::path prefix) : prefix(std::move(prefix)) {}
	std::string get_path(std::string key) const;
	std::filesystem::path get_prefix() const;
	void load_paths(const std::string& path);
};
