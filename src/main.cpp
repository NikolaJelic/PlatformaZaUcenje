#include <iostream>
#include "config.hpp"

int main(int argc, char** argv) {
	std::cout << "HelloWorld!\n";
	Config paths(Config::get_data_path(argv[0]));
}
