#include "message.hpp"
#include <filesystem>
#include <fstream>
#include "util.hpp"

Message::Message(std::string sender, std::string time, std::string message)
	: sender(std::move(sender)), time(std::move(time)), message(std::move(message)) {}

void Message::send_message(std::string const& sender, std::string const& receiver) {
	std::string chat_name = util::generate_chat_name(sender, receiver);
	Message msg{};
	std::cout << "Write message:\n";
	msg.write_message(sender);
	if (auto file = std::fstream("data/inbox/" + chat_name, std::ios::app)) {
		file << msg << "-----------\n";
	} else {
		std::cout << "File can't be opened.\n";
	}
}

std::vector<Message> Message::read_messages(std::string const& path) {
	auto lines = util::get_lines(path);
	std::vector<Message> ret{};
	std::vector<std::string> msg{};
	for (auto const& line : lines) {
		if (line != "-----------") {
			msg.push_back(line);
		} else {
			if (msg.size() >= 2) {
				Message message{};
				auto header = util::parse_list(msg[0], '|');
				message.sender = header[0];
				message.time = header[1];
				message.message = msg[1];
				ret.push_back(message);
			}
			msg = {};
		}
	}

	return ret;
}

void Message::write_message(std::string const& sender) {
	std::cin.ignore();
	std::getline(std::cin, message);
	time = util::get_datetime();
	this->sender = sender;
}

std::ostream& operator<<(std::ostream& os, Message const& message) {
	os << message.sender << " | " << message.time << ":\n";
	os << message.message << "\n";
	return os;
}

void Message::print_chat(std::string const& chat) {
	if (std::filesystem::exists(chat)) {
		if (auto file = std::ifstream(chat)) {
			std::cout << '\n' << file.rdbuf() << '\n';
		}
	}
	std::cout << "File can't be opened.\n";
}

void Message::search(std::string const& key, std::string const& username) {
	auto messages = filter(key, username);
	for (auto const& msg : messages) {
		std::cout << msg << "===========================\n";
	}
}

std::vector<Message> Message::filter(std::string const& key, std::string const& username) {
	auto chats = read_chats(username);
	std::vector<Message> messages{};
	std::vector<Message> ret{};
	for (auto const& chat : chats) {
		auto messages = read_messages("data/inbox/" + chat);

		for (auto const& msg : messages) {
			if (msg.message.find(key) != std::string::npos) {
				ret.push_back(msg);
			}
		}
		messages = {};
	}
	return ret;
}

void Message::display_inbox(std::string const& username) const {
	auto inbox = Message::read_chats(username);
	for (size_t i = 0; i < inbox.size(); ++i) {
		std::cout << i + 1 << ". " << get_recipient(inbox[i], username) << std::endl;
	}
}

std::vector<std::string> Message::read_chats(std::string const& username) {
	std::vector<std::string> ret{};
	for (auto const& file : std::filesystem::directory_iterator("data/inbox")) {
		if (file.path().filename().generic_string().find(username)) {
			ret.push_back(file.path().filename().generic_string());
		}
	}
	return ret;
}

std::string Message::get_recipient(std::string chat, std::string const& username) const {
	auto pos = chat.find(username);
	if (pos != std::string::npos) {
		chat.erase(pos, username.length());
	}
	pos = chat.find("_");
	if (pos != std::string::npos) {
		chat.erase(pos, 1);
	}
	pos = chat.find(".txt");
	if (pos != std::string::npos) {
		chat.erase(pos, 4);
	}
	return chat;
}

void Message::show_chat(std::string const& path) {
	if (auto file = std::ifstream(path)) {
		std::cout << "\n" << file.rdbuf() << std::endl;
	} else {
		std::cout << "Message not found.\n";
	}
}
