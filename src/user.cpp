#include "user.hpp"
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <ostream>
#include "util.hpp"

void User::append_user(std::string const& path) {
	if (auto file = std::fstream(path, std::ios::app)) {
		file << *this;
		file << "=====\n";
	} else {
		std::cout << "File couldn't be opened.\n";
	}
}

std::ostream& operator<<(std::ostream& os, User const& user) {
	os << "username=" << user.username << '\n';
	os << "name=" << user.name << '\n';
	os << "password=" << user.password << '\n';
	os << "admin=" << user.is_admin << '\n';
	util::write_array(os, user.friends);
	util::write_array(os, user.received_requests);
	util::write_array(os, user.sent_requests);

	return os;
}
