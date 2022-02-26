#include "user.hpp"
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include "util.hpp"

User::User(std::string name, std::string username, std::string password, bool is_admin,
		   std::vector<std::string> friends, std::vector<std::string> received_requests,
		   std::vector<std::string> sent_requests)
	: name(std::move(name)), username(std::move(username)), password(std::move(password)), is_admin(is_admin),
	  friends(std::move(friends)), received_requests(std::move(received_requests)),
	  sent_requests(std::move(sent_requests)) {}

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
	os << "friends=";
	util::write_array(os, user.friends);
	os << "received_requests=";
	util::write_array(os, user.received_requests);
	os << "sent_requests=";
	util::write_array(os, user.sent_requests);

	return os;
}

std::vector<User> User::read_users(std::string const& path) {
	std::vector<std::string> lines = util::get_lines(path);
	std::vector<User> users{};
	std::vector<std::string> user{};
	for (auto const& line : lines) {
		if (line != "=====") {
			user.push_back(line);
		} else {
			// could use refactoring
			auto temp = util::insert_pairs(user);
			user = {};
			std::string str{};
			std::string name{}, username{}, password{};
			std::vector<std::string> friends{}, received_requests{}, sent_requests{};
			bool is_admin{};
			util::extract_map(temp, "name", name);
			util::extract_map(temp, "username", username);
			util::extract_map(temp, "password", password);

			util::extract_map(temp, "friends", str);
			friends = util::parse_list(str, '|');
			util::extract_map(temp, "received_requests", str);
			friends = util::parse_list(str, '|');
			util::extract_map(temp, "sent_requests", str);
			friends = util::parse_list(str, '|');
			util::extract_map(temp, "is_admin", str);
			is_admin = stoi(str);
			users.push_back({name, username, password, is_admin, friends, received_requests, sent_requests});
		}
	}
	return users;
}

bool User::is_contained(std::string const& user, std::vector<std::string> const& list) {
	for (auto const& f : list) {
		if (f == user) {
			return true;
		}
	}
	return false;
}

// check if it is possible to send a request and then add to the received and pending requests of both users
void User::send_friend_request(std::string const& username) {
	auto users = read_users("data/users.txt");
	for (auto& usr : users) {
		if (usr.get_username() == username) {
			if (!is_contained(usr.get_username(), usr.friends) &&
				!is_contained(usr.get_username(), usr.received_requests)) {
				usr.received_requests.push_back(this->username);
				sent_requests.erase(std::find(sent_requests.begin(), sent_requests.end(), username));
				usr.update_user("data/users.txt");
				update_user("data/users.txt");
				std::cout << "Friend request has been sent to " << username << std::endl;
			}
		}
	}
	std::cout << "Failed to send a friend request to " << username << std::endl;
}

void User::update_user(std::string const& path) {
	std::vector<User> temp_users = read_users(path);
	for (auto& user : temp_users) {
		if (user.get_username() == username) {
			user = *this;
		}
	}
	if (auto file = std::ofstream(path)) {
		for (auto const& user : temp_users) {
			file << user << "=====\n";
		}
	} else {
		std::cout << "File couldn't be opened.\n";
	}
}

void User::accept_user(std::string const& username) {
	if (is_contained(username, received_requests)) {
		friends.push_back(username);
		received_requests.erase(std::find(received_requests.begin(), received_requests.end(), username));
		update_user("data/users.txt");
		auto users = read_users("data/users.txt");
		for (auto& usr : users) {
			if (usr.get_username() == username) {
				usr.sent_requests.erase(std::find(usr.sent_requests.begin(), usr.sent_requests.end(), this->username));
				usr.update_user("data/users.txt");
			}
		}
	}
	std::cout << "User couldn't be accepted.\n";
}
