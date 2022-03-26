#pragma once

#include <fstream>
#include <ostream>
#include <set>
#include <string>
#include <vector>
#include "message.hpp"
#include "util.hpp"

// data for super user who can add new admins and create new courses and users
struct root {
	std::string username = "root";
	std::string password = "root";
};

// DONE User creation
// DONE Set admin
// DONE	User login
// DONE Course creation
// DONE Set course filter
// TODO Make Graph
// TODO Create screens

class User {
	std::string username{}; // must be unique
	std::string password{};
	bool admin = false;
	double grade{};								// can only be flagged by the root acount
	std::vector<std::string> friends{};			// list of friends
	std::vector<std::string> friends_pending{}; // list of friend requests that the user has to answer
	std::vector<std::string> sent_requests{};	// list of friend requests that the user has sent

	std::string get_recipient(std::string chat) const;

  public:
	User() = default;
	User(std::string username, std::string password, bool is_admin, double grade, std::vector<std::string> friends,
		 std::vector<std::string> friends_pending, std::vector<std::string> sent_requests);

	static bool user_exists(std::vector<User> const& users, std::string const& username);
	std::vector<std::string> list_teachers() const;
	void append_user(std::string const& path);
	std::string get_username() const { return username; }
	bool is_admin() const { return admin; }
	std::vector<std::string> get_friends() const { return friends; }
	std::vector<std::string> get_sent_requests() const { return sent_requests; }
	std::vector<std::string> get_friends_pending() const { return friends_pending; }
	double get_grade() const { return grade; }

	static std::vector<User> read_users(std::string const& path);
	static void create_user();
	void list_users() const;

	void login();

	static void delete_user(std::string const& username);
	static void write_users(std::vector<User> const& users, std::string const& path);

	void send_friend_request(std::string const& username);
	void accept_user(std::string const& username);
	bool enroll_course(std::string const& code);
	void chat(std::string const& receiver);
	bool is_contained(std::string const& user, std::vector<std::string> const& list);
	void update_user(std::string const& path);

	bool set_admin(std::string const& username, bool new_state);
	void update_password(std::string const& username);

	friend std::ostream& operator<<(std::ostream& os, User const& user);
	friend std::ifstream& operator>>(std::ifstream& in, User const& user);
};
std::ostream& operator<<(std::ostream& os, User const& user);
std::ifstream& operator>>(std::ifstream& in, User const& user);
