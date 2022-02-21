#pragma once

#include <fstream>
#include <ostream>
#include <string>
#include <vector>

class User {
	std::string name{};		// can be repeatable
	std::string username{}; // must be unique
	std::string password{};
	bool is_admin = false;						  // can only be flagged by the root acount
	std::vector<std::string> friends{};			  // list of friends
	std::vector<std::string> received_requests{}; // list of friend requests that the user has to answer
	std::vector<std::string> sent_requests{};	  // list of friend requests that the user has sent

  public:
	void append_user(std::string const& path);
	std::vector<User> read_users(std::string const& path);
	bool send_friend_request(std::string const& username);
	bool enroll_course(std::string const& code);

	friend std::ostream& operator<<(std::ostream& os, User const& user);
};
std::ostream& operator<<(std::ostream& os, User const& user);
