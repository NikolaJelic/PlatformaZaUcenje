#include <iostream>
#include "course.hpp"
#include "message.hpp"
#include "user.hpp"

enum class ScreenState {
	login,
	home,
	courses,
	users,
	messages,

};

class Screen {
	ScreenState screen_state = ScreenState::login;
	User current_user{};

	void login();
	void home();
	void courses();
	void users();
	void messages();

  public:
	void run();
};
