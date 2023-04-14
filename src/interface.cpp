#include "interface.hpp"
#include "course.hpp"
#include "message.hpp"
#include "user.hpp"
#include "util.hpp"

void Screen::login() {
	std::cout << "\nLOGIN SCREEN\n\n";
	current_user.login();
	if (!current_user.get_username().empty()) {
		screen_state = ScreenState::home;
	} else {
		running = false;
	}
}

void Screen::home() {
	std::cout << "\nHOME SCREEN\n\n";
	std::cout << "0.Log out\n1.Users\n2.Courses\n3.Messages\n4.Graph\n";
	size_t selection{};
	std::cout << "Input option: ";
	std::cin >> selection;
	switch (selection) {
	case 0: {
		current_user = {};
		screen_state = ScreenState::login;
		break;
	}
	case 1: screen_state = ScreenState::users; break;
	case 2: screen_state = ScreenState::courses; break;
	case 3: screen_state = ScreenState::messages; break;
	case 4: screen_state = ScreenState::graph; break;
	}
}

void Screen::users() {
	std::cout << "\nUSERS\n\n";
	std::cout << "0.Back\n1.List friends\n2.Add friend\n3.Accept friend requests\n";
	if (current_user.is_admin()) {
		std::cout << "4.Create user\n5.Assign admin\n6.Delete user\n7.Edit user password\n";
	}
	size_t selection;
	std::cout << "Input option: ";
	std::cin >> selection;
	switch (selection) {
	case 0: screen_state = ScreenState::home; break;
	case 1: util::print_list(current_user.get_friends()); break;
	case 2: {
		current_user.list_users();
		std::string other{};
		std::cout << "Enter username: ";
		std::cin >> other;
		current_user.send_friend_request(other);
	} break;
	case 3: {
		util::print_list(current_user.get_friends_pending());
		std::cout << "Input username you want to accept[empty string for quit selection]:";
		std::string username{};
		std::cin >> username;
		if (!username.empty()) {
			current_user.accept_user(username);
		}
	} break;
	case 4: {
		if (current_user.is_admin()) {
			User::create_user();
			break;
		}
	}
	case 5: {
		if (current_user.is_admin()) {
			current_user.list_users();
			bool new_state{};
			std::string username{};
			size_t bool_int{};
			std::cout << "Input username you want to accept[empty string for quit selection]:";
			std::cin >> username;
			if (!username.empty()) {
				std::cout << "Input new state [0 = remove admin | else = assign admin]: ";
				std::cin >> bool_int;
				if (!bool_int) {
					new_state = false;
				} else {
					new_state = true;
				}
				current_user.set_admin(username, new_state);
			}
		}
	} break;
	case 6: {
		if (current_user.is_admin()) {
			current_user.list_users();
			std::string username{};
			std::cout << "Input username you want to delete[empty string for quit selection]:";
			std::cin >> username;
			if (!username.empty()) {
				User::delete_user(username);
			}
		}
	} break;
	case 7: {
		if (current_user.is_admin()) {
			current_user.list_users();
			std::string username{};
			std::cout << "Input username you want to edit password for[empty string for quit selection]:";
			std::cin >> username;
			if (!username.empty()) {
				current_user.update_password(username);
			}
		}
	} break;
	}
}

void Screen::courses() {
	Course viewed_course{};
	std::cout << "\nCOURSES\n\n";
	std::cout << "0.Back\n1.View all courses\n2.View enrolled courses\n3.Enroll into course\n4.Accept student\n";

	if (current_user.is_admin()) {
		std::cout << "5.Create new course\n6.Delete course\n7.Add teacher to course\n8.Compare "
					 "courses\n9.Pass student\n";
	}

	size_t selection;
	std::cout << "Input option: ";
	std::cin >> selection;
	switch (selection) {
	case 0: screen_state = ScreenState::home; break;
	case 1: viewed_course.list_courses(); break;
	case 2: Course::list_enrolled(current_user.get_username()); break;
	case 3: {
		viewed_course.list_courses();
		std::string code{};
		std::cout << "Input course code you want to attend[empty string for quit selection]:";
		std::cin >> code;
		if (!code.empty()) {
			auto courses = Course::read_courses("data/courses.txt");
			for (auto& course : courses) {
				if (course.get_code() == code && current_user.can_enroll(course)) {
					course.enroll_student(current_user.get_username());
					break;
				} else {
					std::cout << "Enrollment failed.\n";
				}
			}
		}
	} break;
	case 4: {
		auto courses = Course::read_courses("data/courses.txt");
		std::vector<Course> selected_courses{};
		std::vector<std::string> teachers{};
		for (auto const& course : courses) {
			teachers = course.get_teachers();

			if (std::find(teachers.begin(), teachers.end(), current_user.get_username()) != teachers.end()) {
				selected_courses.push_back(course);
				std::cout << "=> " << course.get_code() << " | " << course.get_name() << '\n';
			}
		}
		std::string code{};
		std::cout << "Enter the code of the course you want to approve enrollment for:";
		std::cin >> code;
		if (!code.empty()) {
			for (auto& course : selected_courses) {
				if (course.get_code() == code) {
					std::string username{};
					util::print_list(course.get_pending());
					std::cout << "Enter username of the student you wish to enroll: ";
					std::cin >> username;
					if (!username.empty()) {
						course.accept_user(username, current_user.get_username(), current_user.is_admin());
					}
				}
			}
		}
	} break;

	case 5: {
		if (current_user.is_admin()) {
			Course::create_course(current_user.is_admin());
		}
	} break;
	case 6: {
		if (current_user.is_admin()) {

			viewed_course.list_courses();
			std::string code{};
			std::cout << "Input course code you want to delete[empty string for quit selection]:";
			std::cin >> code;
			if (!code.empty()) {
				auto courses = Course::read_courses("data/courses.txt");
				for (auto& course : courses) {
					if (course.get_code() == code) {
						Course::delete_course(code);
						std::cout << "Course has been deleted.\n";
						break;
					}
				}
			}
		}
	} break;
	case 7: {
		if (current_user.is_admin()) {
			viewed_course.list_courses();
			std::cout << "Enter the code of the course you want to assign a teacher to:";
			std::string code{};
			std::cin >> code;
			if (!code.empty()) {
				auto courses = Course::read_courses("data/courses.txt");
				for (auto& course : courses) {
					if (course.get_code() == code) {
						current_user.list_users();
						std::string username{};
						std::cout << "Enter username of the teacher you wish to assign: ";
						std::cin >> username;
						auto users = User::read_users("data/users.txt");
						if (User::user_exists(users, username)) {
							course.add_teacher(username, current_user.is_admin());
						} else {
							std::cout << "User doesn't exist.\n";
						}
					}
				}
			}
		}
	} break;

	case 8: {
		viewed_course.list_courses();
		std::string code_first{}, code_second;
		std::cout << "Enter the code of the first course :";
		std::cin >> code_first;
		std::cout << "Enter the code of the second course :";
		std::cin >> code_second;
		if (!code_first.empty() && !code_second.empty()) {
			std::cout << "Comparison type:\n0.Back\n1.Union\n2.Intersection\n3.Difference\nInput: ";
			int selection{};
			std::cin >> selection;
			std::cout << "Current[0] or graduated users[else]: ";
			size_t option{};
			std::cin >> option;
			bool graduated = option;
			switch (selection) {
			case 1:
				util::print_list(Course::compare(code_first, code_second, Comparison::group_union, graduated));
				break;
			case 2:
				util::print_list(Course::compare(code_first, code_second, Comparison::intersection, graduated));
				break;
			case 3:
				util::print_list(Course::compare(code_first, code_second, Comparison::difference, graduated));
				break;
			}
		}
	} break;
	case 9: {
		auto courses = Course::read_courses("data/courses.txt");
		std::vector<Course> selected_courses{};
		std::vector<std::string> teachers{};
		for (auto const& course : courses) {
			teachers = course.get_teachers();

			if (std::find(teachers.begin(), teachers.end(), current_user.get_username()) != teachers.end()) {
				selected_courses.push_back(course);
				std::cout << "=> " << course.get_code() << " | " << course.get_name() << '\n';
			}
		}
		std::string code{};
		std::cout << "Enter the code of the course you want to pass student in:";
		std::cin >> code;
		if (!code.empty()) {
			for (auto& course : selected_courses) {
				if (course.get_code() == code) {
					std::string username{};
					util::print_list(course.get_students());
					std::cout << "Enter username of the student you wish to pass: ";
					std::cin >> username;
					if (!username.empty()) {
						auto grade = course.pass_student(username);
						if (grade.second) {
							auto users = User::read_users("data/users.txt");
							for (auto& user : users) {
								if (user.get_username() == username) {
									user.add_grade(grade);
									User::write_users(users, "data/users.txt");
								}
							}
						}
					}
				}
			}
		}
	}
	}
}

void Screen::messages() {
	std::cout << "\nMESSAGES\n\n";
	std::cout << "0.Return\n1.Inbox\n2.New message\n3.Search\n";
	size_t selection{};
	Message temp_message{};
	std::cout << "Input option: ";
	std::cin >> selection;
	switch (selection) {
	case 0: screen_state = ScreenState::home; break;
	case 1: {
		std::cout << "\nINBOX:\n";
		temp_message.display_inbox(current_user.get_username());

	} break;
	case 2: {
		auto teachers = current_user.list_teachers();
		auto friends = current_user.get_friends();
		std::sort(friends.begin(), friends.end());
		std::sort(teachers.begin(), teachers.end());
		std::vector<std::string> available_chats{};
		std::set_union(teachers.begin(), teachers.end(), friends.begin(), friends.end(),
					   std::back_inserter(available_chats));
		for (auto const& chat : available_chats) {
			if (!chat.empty() && chat != current_user.get_username()) {
				std::cout << "=> " << chat << '\n';
			}
		}
		std::string username{};
		std::cout << "Input username: ";
		std::cin >> username;
		if (!username.empty()) {
			current_user.chat(username);
		}
	} break;
	case 3: {
		std::string key{};
		std::cout << "Input keyword you want to search for: ";
		std::cin >> key;
		if (!key.empty()) {
			Message::search(key, current_user.get_username());
		}
	} break;
	}
}

void Screen::graph() {
	Graph g{};
	auto users = User::read_users("data/users.txt");
	g.create_graph(users);
	std::cout << "\nGRAPH\n\n";
	std::cout << "0.Back\n1.Find shortest path between users\n2.Find average distance between two groups\n";
	size_t selection{};
	std::cout << "Input: ";
	std::cin >> selection;
	switch (selection) {
	case 0: screen_state = ScreenState::home; break;
	case 1: {
		current_user.list_users();
		std::string first{}, second{};
		std::cout << "Input first username: ";
		std::cin >> first;
		std::cout << "Input second username: ";
		std::cin >> second;

		if (g.contains(first) && g.contains(second)) {
			auto path = g.shortest_path(first, second);
			for (auto const& u : path) {
				std::cout << u << "  ";
			}
			std::cout << std::endl;
		}
	} break;
	case 2: {
		Course temp{};
		temp.list_courses();
		std::string first{}, second{};
		std::cout << "Input first course code: ";
		std::cin >> first;
		std::cout << "Input second course code: ";
		std::cin >> second;

		std::cout << "Average distance is: " << g.average_distance(first, second) << std::endl;

	} break;
	}
}

void Screen::run() {
	switch (screen_state) {
	case ScreenState::login: login(); break;
	case ScreenState::home: home(); break;
	case ScreenState::courses: courses(); break;
	case ScreenState::messages: messages(); break;
	case ScreenState::users: users(); break;
	case ScreenState::graph: graph(); break;
	}
}
