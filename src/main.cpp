#include <fstream>
#include <iostream>
#include "course.hpp"
#include "message.hpp"
#include "user.hpp"
#include "util.hpp"

int main() {

	Course::create_course(true);

	/* 	User nikola{"nikola", "nikola", false, {"tiho"}, {"igor"}, {"Miroslav"}};
		User tiho{"tiho", "tiho", false, {"nikola"}, {"igor"}, {"miroslav"}};
		User igor{"igor", "tiho", false, {"nikola"}, {"igor"}, {"miroslav"}};
		User miroslav{"miroslav", "tiho", false, {"nikola"}, {"igor"}, {"tiho"}}; */

	/* nikola.append_user("data/users.txt");
	tiho.append_user("data/users.txt");
	igor.append_user("data/users.txt");
	miroslav.append_user("data/users.txt"); */

	// Nikola.append_user("data/users.txt");
	// User::create_user();
	// auto users = User::read_users("data/users.txt");
	/* 	auto test = util::insert_pairs({"friends_pending=Nikola"});
		for (auto const& [key, value] : test) {
			std::cout << key << " | " << value << std::endl;
		} */
	// auto ret = User::read_users("data/usr.txt");
	// nikola.chat("tiho");
	// Message msg{};
	// msg.display_inbox("nikola");
	//  msg.show_chat("data/inbox/" + util::generate_chat_name("nikola", "tiho"));
	//	msg.read_chats("nikola");
	/* 	msg.search("Helou", "nikola");
		//	nikola.send_friend_request("miroslav");
		auto users = User::read_users("data/users.txt");
		for (auto& usr : users) {

			if (usr.get_username() == "nikola") {
				usr.enroll_course("101");
			}
		}
	 */
	//! Read message potencijalan problem
}
