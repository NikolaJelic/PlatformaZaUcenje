#include <fstream>
#include <iostream>
#include "course.hpp"
#include "message.hpp"
#include "util.hpp"

int main() {
	std::cout << "HelloWorld!\n";
	std::cout << util::generate_chat_name("alma", "nikola") << std::endl;
	std::cout << util::get_datetime() << std::endl;
}
