#include "message.hpp"
#include <filesystem>
#include "util.hpp"

Message::Message(std::string sender, std::string time, std::string message)
	: sender(std::move(sender)), time(std::move(time)), message(std::move(message)) {}

void Message::send_message(std::string const& sender, std::string const& receiver) {
	std::string chat_name = util::generate_chat_name(sender, receiver);
	Message msg{};
	msg.write_message(sender);
	if (auto file = std::fstream("data/inbox" + chat_name, std::ios::app)) {
		file << msg << "\n-----------\n";
	} else {
		std::cout << "File can't be opened.\n";
	}
}

void Message::write_message(std::string const& sender) {
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
		std::cout << "File can't be opened.\n";
	}
	std::cout << "File can't be opened.\n";
}

void Message::search(std::string const& key) const{
	
}
