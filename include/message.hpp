#pragma once

#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// files should be a combination of the usernames of the participants in alphabetical order
// messages will look like a chat, so all of them will be in one file

class Message {
	std::string sender{}; // uses the name and not the username
	std::string time{};
	std::string message{};

  public:
	Message() = default;
	Message(std::string sender, std::string time, std::string message);
	static void send_message(std::string const& sender, std::string const& receiver);
	void write_message(std::string const& sender);
	// takes the path of the chat log
	static void print_chat(std::string const& chat);
	void search(std::string const& key) const;
	friend std::ostream& operator<<(std::ostream& os, Message const& message);
};

std::ostream& operator<<(std::ostream& os, Message const& message);
