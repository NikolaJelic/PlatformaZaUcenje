#pragma once

#include <cstddef>
#include <string>
#include <vector>

// files should be a combination of the usernames of the participants in alphabetical order
// messages will look like a chat, so all of them will be in one file

class Message {
	std::string sender{}; // uses the name and not the username
	std::string time{};
	std::string message{};
};
