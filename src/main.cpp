
#include "course.hpp"
#include "interface.hpp"
#include "message.hpp"
#include "user.hpp"
#include "util.hpp"

int main() {

	Screen s{};
	while (s.running) {
		s.run();
	}
}
