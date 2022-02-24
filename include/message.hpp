#pragma once

#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "user.hpp"

// files should be a combination of the usernames of the participants in alphabetical order
// messages will look like a chat, so all of them will be in one file

class Message {
	std::string sender{}; // uses the name and not the username
	std::string time{};
	std::string message{};

  public:
	Message(std::string sender, std::string time, std::string message);
	void send_message(User const& sender, User const& receiver);
	static Message write_message(std::string const& sender);
	friend std::ostream& operator<<(std::ostream& os, Message const& message);
};

std::ostream& operator<<(std::ostream& os, Message const& message);
