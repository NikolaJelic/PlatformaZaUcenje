#pragma once

#include <fstream>
#include <ostream>
#include <set>
#include <string>
#include <vector>
#include "message.hpp"
#include "util.hpp"

class User {
	std::string name{};		// can be repeatable
	std::string username{}; // must be unique
	std::string password{};
	bool is_admin = false;						  // can only be flagged by the root acount
	std::vector<std::string> friends{};			  // list of friends
	std::vector<std::string> received_requests{}; // list of friend requests that the user has to answer
	std::vector<std::string> sent_requests{};	  // list of friend requests that the user has sent

	std::vector<std::string> list_teachers() const;
	std::string get_recipient(std::string chat) const;

  public:
	User(std::string name, std::string username, std::string password, bool is_admin, std::vector<std::string> friends,
		 std::vector<std::string> received_requests, std::vector<std::string> sent_requests);

	void append_user(std::string const& path);

	std::string get_username() const { return username; }

	std::vector<User> read_users(std::string const& path);
	void send_friend_request(std::string const& username);
	void accept_user(std::string const& username);
	bool enroll_course(std::string const& code);
	void chat(std::string const& receiver);
	bool is_contained(std::string const& user, std::vector<std::string> const& list);
	void update_user(std::string const& path);

	// Returns filenames of all chats belonging to the current user
	std::vector<std::string> read_chats() const;
	void display_inbox() const;

	friend std::ostream& operator<<(std::ostream& os, User const& user);
	friend std::ifstream& operator>>(std::ifstream& in, User const& user);
};
std::ostream& operator<<(std::ostream& os, User const& user);
std::ifstream& operator>>(std::ifstream& in, User const& user);
