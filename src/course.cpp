#include "course.hpp"
#include "util.hpp"

std::ostream& operator<<(std::ostream& os, Course const& course) {
	os << "name=" << course.name << '\n';
	os << "code=" << course.code << '\n';
	os << "credits=" << course.credits << '\n';
	os << "department=" << util::enum_to_int(course.department) << '\n';
	for (size_t i = 0; i < course.rules.size() - 1; ++i) {
		os << util::enum_to_int(course.rules[i]) << '|';
	}
	os << util::enum_to_int(course.rules[course.rules.size() - 1]) << '\n';
	util::write_array(os, course.conditions);
	util::write_array(os, course.teachers);
	util::write_array(os, course.students);
	util::write_array(os, course.pending_students);
	util::write_array(os, course.graduates);

	return os;
}
