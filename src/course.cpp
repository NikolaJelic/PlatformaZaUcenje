#include "course.hpp"


Course::Course(std::string name, std::string code, std::size_t credits, Department department, std::vector<Rules> rules,
			   std::vector<std::string> conditions, std::vector<std::string> teachers,
			   std::vector<std::string> students, std::vector<std::string> pending_students,
			   std::vector<std::string> graduates)
	: name(name), code(code), credits(credits), department(department), rules(std::move(rules)),
	  conditions(std::move(conditions)), teachers(std::move(teachers)), students(std::move(students)),
	  pending_students(std::move(pending_students)), graduates(std::move(graduates)) {}

std::ostream& operator<<(std::ostream& os, Course const& course) {
	os << "name=" << course.name << '\n';
	os << "code=" << course.code << '\n';
	os << "credits=" << course.credits << '\n';
	os << "department=" << util::enum_to_int(course.department) << '\n';
	for (size_t i = 0; i < course.rules.size() - 1; ++i) {
		os << util::enum_to_int(course.rules[i]) << '|';
	}
	os << util::enum_to_int(course.rules[course.rules.size() - 1]) << '\n';
	os << "conditions=";
	util::write_array(os, course.conditions);
	os << "teachers=";
	util::write_array(os, course.teachers);
	os << "students=";
	util::write_array(os, course.students);
	os << "pending_students=";
	util::write_array(os, course.pending_students);
	os << "graduates=";
	util::write_array(os, course.graduates);

	return os;
}

void Course::enroll_student(std::string const& student) {
	pending_students.push_back(student);
	update_course("data/courses.txt");
}

void Course::update_course(std::string const& path) {
	std::vector<Course> temp_courses = read_courses(path);
	for (auto& course : temp_courses) {
		if (course.get_code() == code) {
			course = *this;
		}
	}
	if (auto file = std::ofstream(path)) {
		for (auto const& course : temp_courses) {
			file << course << "=====\n";
		}
	} else {
		std::cout << "File couldn't be opened.\n";
	}
}

// only admins can use this
void Course::add_teacher(std::string const& teacher, bool is_admin) {
	if (is_admin) {
		teachers.push_back(teacher);
		update_course("data/courses.txt");
	}
}
std::vector<Course> Course::read_courses(std::string const& path) {
	std::vector<std::string> lines = util::get_lines(path);
	std::vector<Course> courses{};
	std::vector<std::string> course{};
	for (auto const& line : lines) {
		if (line != "=====") {
			course.push_back(line);
		} else {
			// could use refactoring
			auto temp = util::insert_pairs(course);
			course = {};
			std::string str{};
			std::size_t credits{};
			std::string name{}, code{};
			Department department{};
			std::vector<Rules> rules{};
			std::vector<std::string> conditions{}, teachers{}, students{}, pending_students{}, graduates{};
			util::extract_map(temp, "name", name);
			util::extract_map(temp, "code", code);
			util::extract_map(temp, "rules", str);
			auto temp_list = util::parse_list(str, '|');
			for (auto rule : temp_list) {
				rules.push_back(static_cast<Rules>(stoi(rule)));
			}

			util::extract_map(temp, "credits", str);
			credits = stoi(str);
			util::extract_map(temp, "conditions", str);
			conditions = util::parse_list(str, '|');
			util::extract_map(temp, "teachers", str);
			teachers = util::parse_list(str, '|');
			util::extract_map(temp, "students", str);
			students = util::parse_list(str, '|');
			util::extract_map(temp, "pending_students", str);
			pending_students = util::parse_list(str, '|');
			util::extract_map(temp, "graduates", str);
			graduates = util::parse_list(str, '|');
			util::extract_map(temp, "department", str);
			department = static_cast<Department>(stoi(str));
			courses.push_back(
				{name, code, credits, department, rules, conditions, teachers, students, pending_students, graduates});
		}
	}
	return courses;
}

void Course::accept_user(std::string const& requested_user, std::string const& username, bool is_admin) {
	auto it = std::find(teachers.begin(), teachers.end(), username);
	if (it != teachers.end() || is_admin) {
		it = std::find(pending_students.begin(), pending_students.end(), requested_user);
		if (it != pending_students.end()) {
			students.push_back(requested_user);
		}
		pending_students.erase(it);
		update_course("data/courses.txt");
	}
}
