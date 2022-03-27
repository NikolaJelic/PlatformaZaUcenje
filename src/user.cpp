#include "user.hpp"
#include <cstddef>
#include <cstdio>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include "course.hpp"
#include "util.hpp"

User::User(std::string username, std::string password, bool is_admin,
		   std::vector<std::pair<std::string, size_t>> grades, std::vector<std::string> friends,
		   std::vector<std::string> friends_pending, std::vector<std::string> sent_requests)
	: username(std::move(username)), password(std::move(password)), admin(is_admin), grades(grades),
	  friends(std::move(friends)), friends_pending(std::move(friends_pending)),
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

	os << "password=" << user.password << '\n';
	os << "admin=" << user.admin << '\n';
	os << "grades=";
	for (size_t i = 0; i < user.grades.size() - 1; ++i) {
		os << user.grades[i].first << ',' << user.grades[i].second << '|';
	}
	os << user.grades[user.grades.size() - 1].first << ',' << user.grades[user.grades.size() - 1].second << std::endl;
	os << "friends=";
	util::write_array(os, user.friends);
	os << "friends_pending=";
	util::write_array(os, user.friends_pending);
	os << "sent_requests=";
	util::write_array(os, user.sent_requests);

	return os;
}

bool User::user_exists(std::vector<User> const& users, std::string const& username) {
	bool ret = false;
	for (auto const& user : users) {
		if (user.get_username() == username) {
			ret = true;
		}
	}
	return ret;
}

void User::create_user() {
	std::cout << "Enter user info:\n";
	std::string username{}, password{};

	bool unique = false;

	// load username until a unieuqe one has been entered
	auto users = read_users("data/users.txt");
	while (unique == false) {
		std::cout << "Username(must be unique): ";
		std::cin >> username;
		unique = !user_exists(users, username);
		if (!unique) {
			std::cout << "\n Username must be unique!\n";
		}
	}
	std::cout << "Password: ";
	std::cin >> password;

	User new_user{username, password, {}, {}, {}, {}, {}};
	new_user.append_user("data/users.txt");
}

void User::delete_user(std::string const& username) {
	if (username != "root") {
		auto users = read_users("data/users.txt");

		if (user_exists(users, username)) {
			auto match_username = [username](User usr) { return usr.username == username; };
			users.erase(std::find_if(users.begin(), users.end(), match_username));
		}
		for (auto& user : users) {
			util::remove_from_list(user.friends, username);
			util::remove_from_list(user.friends_pending, username);
			util::remove_from_list(user.sent_requests, username);
		}
		auto courses = Course::read_courses("data/courses.txt");
		for (auto& course : courses) {
			auto students = course.get_students();
			auto teachers = course.get_teachers();
			auto pending = course.get_pending();
			auto graduates = course.get_graduates();
			util::remove_from_list(students, username);
			util::remove_from_list(teachers, username);
			util::remove_from_list(pending, username);
			util::remove_from_list(graduates, username);
			course.set_graduates(graduates);
			course.set_students(students);
			course.set_pending(pending);
			course.set_teachers(teachers);
		}
		write_users(users, "data/users.txt");
	}
}

std::vector<User> User::read_users(std::string const& path) {
	std::vector<std::string> lines = util::get_lines(path);

	std::vector<User> users{};
	std::vector<std::string> user{};
	for (auto const& line : lines) {
		if (line != "=====") {
			user.push_back(line);
		} else if (user.size() == 7) { // checks for missing fields

			// could use refactoring
			auto temp = util::insert_pairs(user);
			user = {};
			std::string str{};
			std::string username{}, password{};
			std::vector<std::string> friends{}, friends_pending{}, sent_requests{};
			bool admin{};
			std::vector<std::pair<std::string, size_t>> grades{};
			util::extract_map(temp, "username", username);
			util::extract_map(temp, "password", password);

			util::extract_map(temp, "friends", str);
			friends = util::parse_list(str, '|');
			util::extract_map(temp, "friends_pending", str);
			friends_pending = util::parse_list(str, '|');
			util::extract_map(temp, "sent_requests", str);
			sent_requests = util::parse_list(str, '|');
			util::extract_map(temp, "admin", str);
			admin = stoi(str);
			util::extract_map(temp, "grades", str);
			auto grade_list = util::parse_list(str, '|');
			for (auto const& g : grade_list) {
				if (!g.empty()) {
					auto temp = util::parse_list(g, ',');
					grades.push_back({temp[0], std::stoi(temp[1])});
				}
			}
			users.push_back({username, password, admin, grades, friends, friends_pending, sent_requests});
		} else {
			user = {};
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
			if (!is_contained(this->username, usr.friends) && !is_contained(this->username, usr.friends_pending) &&
				!is_contained(usr.username, this->sent_requests)) {
				usr.friends_pending.push_back(this->username);
				sent_requests.push_back(username);
				usr.update_user("data/users.txt");
				update_user("data/users.txt");
				std::cout << "Friend request has been sent to " << username << std::endl;
				return;
			} else {
				std::cout << "Failed to send a friend request to " << username << std::endl;
				return;
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
	write_users(temp_users, path);
}

void User::write_users(std::vector<User> const& users, std::string const& path) {
	if (auto file = std::ofstream(path)) {
		for (auto const& user : users) {
			file << user << "=====\n";
		}
	} else {
		std::cout << "File couldn't be opened.\n";
	}
}

void User::accept_user(std::string const& username) {
	if (is_contained(username, friends_pending)) {
		friends.push_back(username);
		friends_pending.erase(std::find(friends_pending.begin(), friends_pending.end(), username));
		update_user("data/users.txt");
		auto users = read_users("data/users.txt");
		for (auto& usr : users) {
			if (usr.get_username() == username) {
				usr.friends.push_back(this->username);
				usr.sent_requests.erase(std::find(usr.sent_requests.begin(), usr.sent_requests.end(), this->username));
				usr.update_user("data/users.txt");
			}
			std::cout << "Friend request accepted.\n";
		}
	} else {
		std::cout << "User couldn't be accepted.\n";
	}
}

std::vector<std::string> User::list_teachers() const {
	std::vector<Course> courses = Course::read_courses("data/courses.txt");
	std::set<std::string> unq_teachers{};

	for (auto course : courses) {
		if (std::find(course.get_students().begin(), course.get_students().end(), username) !=
			course.get_students().end()) {
			for (auto teacher : course.get_teachers()) {
				unq_teachers.insert(teacher);
			}
		}
	}
	return {unq_teachers.begin(), unq_teachers.end()};
}

void User::chat(std::string const& receiver) {
	if (is_contained(receiver, friends) || is_contained(receiver, list_teachers())) {
		Message::send_message(username, receiver);
		std::cout << "Message to " << receiver << " has been sent." << std::endl;
	}
}

bool User::set_admin(std::string const& username, bool new_state) {
	auto users = read_users("data/users.txt");
	if (user_exists(users, username)) {
		for (auto& user : users) {
			if (user.get_username() == username) {
				user.admin = new_state;
				user.update_user("data/users.txt");
				return true;
			}
		}
	}
	return false;
}

void User::update_password(std::string const& username) {
	auto users = read_users("data/users.txt");
	if (user_exists(users, username)) {
		std::cout << "Enter new password: ";
		std::string temp{};
		std::cin >> temp;
		for (auto& user : users) {
			if (user.get_username() == username) {
				user.password = temp;
				user.update_user("data/users.txt");
			}
		}
	} else {
		std::cout << "user hasn't been found.\n";
	}
}

void User::login() {
	std::cout << "Enter username: ";
	std::string username{}, password{};
	auto users = read_users("data/users.txt");
	bool is_valid = false;
	while (!is_valid) {
		std::cin >> username;
		if (username == "0") {
			std::cout << "Login failed.\n";
			return;
		} else if (user_exists(users, username)) {
			for (auto const& user : users) {
				if (user.get_username() == username) {
					std::cout << "Enter password: ";
					std::cin >> password;
					if (user.password == password) {
						*this = user;
						std::cout << "Login sucessfull.\n";
						return;
					} else {
						std::cout << "Incorrect password.\n";
						return;
					}
				}
			}
		}
	}
	std::cout << "Login failed.\n";
}

void User::list_users() const {
	auto users = read_users("data/users.txt");
	for (size_t i = 0; i < users.size(); ++i) {
		std::cout << i + 1 << ". " << users[i].username << '\n';
	}
}

double User::calculate_grade() const {
	double sum{};
	size_t number_of_courses{};
	for (auto const& g : grades) {
		sum += g.second;
		++number_of_courses;
	}
	return sum / number_of_courses;
}

bool User::can_enroll(Course const& course) const {
	bool num_courses = true;
	bool min_grade = true;
	bool req_courses = true; // sets to false if it isn't found
	auto rules = course.get_rules();
	if (rules.num_of_courses_passed > 0) {

		if (grades.size() < rules.num_of_courses_passed) {
			num_courses = false;
		}
	}
	if (rules.average_grade > 0) {
		if (calculate_grade() < rules.average_grade) {
			min_grade = false;
		}
	}

	if (!rules.required_courses.empty()) {
		std::vector<std::string> passed_courses{};
		for (auto const& [c, g] : grades) {
			passed_courses.push_back(c);
		}
		for (auto const& c : rules.required_courses) {
			if (std::find(passed_courses.begin(), passed_courses.end(), c) == passed_courses.end()) {
				req_courses = false;
			}
		}
	}

	return (num_courses && min_grade && req_courses);
}
