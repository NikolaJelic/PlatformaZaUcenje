#pragma once

#include <cstddef>
#include <fstream>
#include <ostream>
#include <set>
#include <string>
#include <vector>
#include "course.hpp"
#include "message.hpp"
#include "util.hpp"

class User {
	std::string username{}; // must be unique
	std::string password{};
	bool admin = false;
	std::vector<std::pair<std::string, size_t>> grades{}; // pair [code,grade]
	std::vector<std::string> friends{};					  // list of friends
	std::vector<std::string> friends_pending{};			  // list of friend requests that the user has to answer
	std::vector<std::string> sent_requests{};			  // list of friend requests that the user has sent

  public:
	User() = default;
	User(std::string username, std::string password, bool is_admin, std::vector<std::pair<std::string, size_t>> grades,
		 std::vector<std::string> friends, std::vector<std::string> friends_pending,
		 std::vector<std::string> sent_requests);

	static bool user_exists(std::vector<User> const& users, std::string const& username);
	[[nodiscard]] std::vector<std::string> list_teachers() const;
	void append_user(std::string const& path) const;
	[[nodiscard]] std::string get_username() const { return username; }
	[[nodiscard]] bool is_admin() const { return admin; }
	[[nodiscard]] std::vector<std::string> get_friends() const { return friends; }
	[[nodiscard]] std::vector<std::string> get_sent_requests() const { return sent_requests; }
	[[nodiscard]] std::vector<std::string> get_friends_pending() const { return friends_pending; }
	void add_grade(const std::pair<std::string, size_t>& grade) { grades.push_back(grade); }

	static std::vector<User> read_users(std::string const& path);
	static void create_user();
	void list_users() const;

	void login();

	static void delete_user(std::string const& username);
	static void write_users(std::vector<User> const& users, std::string const& path);

	void send_friend_request(std::string const& username);
	void accept_user(std::string const& username);
	[[nodiscard]] bool can_enroll(Course const& course) const;

	void chat(std::string const& receiver);
	bool is_contained(std::string const& user, std::vector<std::string> const& list);
	void update_user(std::string const& path);

	bool set_admin(std::string const& username, bool new_state);
	void update_password(std::string const& username);
	[[nodiscard]] double calculate_grade() const;

	friend std::ostream& operator<<(std::ostream& os, User const& user);
	friend std::ifstream& operator>>(std::ifstream& in, User const& user);
};
std::ostream& operator<<(std::ostream& os, User const& user);
std::ifstream& operator>>(std::ifstream& in, User const& user);
