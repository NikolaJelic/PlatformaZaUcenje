#include "course.hpp"

#include <utility>
#include "util.hpp"

Course::Course(std::string name, std::string code, std::size_t credits, Department department, Rules rules,
			   std::vector<std::string> teachers, std::vector<std::string> students, std::vector<std::string> pending,
			   std::vector<std::string> graduates)
	: name(std::move(name)), code(std::move(code)), credits(credits), department(department), rules(std::move(rules)),
	  teachers(std::move(teachers)), students(std::move(students)), pending(std::move(pending)),
	  graduates(std::move(graduates)) {}

void Course::enroll_student(std::string const& student) {

	pending.push_back(student);
	update_course("data/courses.txt");
	std::cout << "Request for enrollment has been sent.\n";
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
		if (std::find(teachers.begin(), teachers.end(), teacher) == teachers.end()) {
			teachers.push_back(teacher);
			update_course("data/courses.txt");
			std::cout << "Teacher has been added.\n";
		}
	}
}
std::vector<Course> Course::read_courses(std::string const& path) {
	std::vector<std::string> lines = util::get_lines(path);
	std::vector<Course> courses{};
	std::vector<std::string> course{};
	for (auto const& line : lines) {
		if (line != "=====") {
			course.push_back(line);
		} else if (course.size() == 9) { // checks if entry has enough fields
			// could use refactoring
			auto temp = util::insert_pairs(course);
			course = {};
			std::string str{};
			std::size_t credits{};
			std::string name{}, code{};
			Department department{};
			Rules rules{};
			std::vector<std::string> teachers{}, students{}, pending{}, graduates{};
			util::extract_map(temp, "name", name);
			util::extract_map(temp, "code", code);
			util::extract_map(temp, "rules", str);
			rules.read_rules(str);

			util::extract_map(temp, "credits", str);
			credits = stoi(str);

			util::extract_map(temp, "teachers", str);
			teachers = util::parse_list(str, '|');
			util::extract_map(temp, "students", str);
			students = util::parse_list(str, '|');
			util::extract_map(temp, "pending", str);
			pending = util::parse_list(str, '|');
			util::extract_map(temp, "graduates", str);
			graduates = util::parse_list(str, '|');
			util::extract_map(temp, "department", str);
			department = static_cast<Department>(stoi(str));
			courses.emplace_back(name, code, credits, department, rules, teachers, students, pending, graduates);
		} else {
			course = {};
		}
	}
	return courses;
}

void Course::accept_user(std::string const& requested_user, std::string const& username, bool is_admin) {
	auto it = std::find(teachers.begin(), teachers.end(), username);
	if (it != teachers.end() || is_admin) {
		it = std::find(pending.begin(), pending.end(), requested_user);
		if (it != pending.end()) {
			students.push_back(requested_user);
			pending.erase(it);
			update_course("data/courses.txt");
		}
	}
}

void Course::create_course(bool is_admin) {
	if (is_admin) {
		std::cout << "Enter details for the new course: \n";
		std::string name, code{};
		size_t credits{};
		Department department{};
		Rules rules{};
		std::vector<std::string> conditions{};

		std::cout << "Course name: ";
		std::cin.ignore();

		std::getline(std::cin, name);
		std::cout << "Course code (3 digit number): ";
		std::cin >> code;
		std::cout << "Credits (integer): ";
		std::cin >> credits;
		// TODO print departments
		std::cout << "Department (0-5): ";
		size_t dp{};
		std::cin >> dp;
		department = static_cast<Department>(dp);
		rules.input_rule();

		Course new_course{name, code, credits, department, rules, {}, {}, {}, {}};
		new_course.append_course("data/courses.txt");
		std::cout << "Course created.\n";

	} else {
		std::cout << "User isn't admin.\n";
	}
}

void Course::append_course(std::string const& path) const {
	if (auto file = std::fstream(path, std::ios::app)) {
		file << *this;
		file << "=====\n";
	} else {
		std::cout << "File couldn't be opened.\n";
	}
}

std::vector<std::string> Course::compare(std::string const& code_first, std::string const& code_second, Comparison cmp,
										 bool graduated) {
	auto courses = read_courses("data/courses.txt");
	std::vector<std::string> ret{};
	std::vector<std::string> first{}, second{};
	for (auto const& course : courses) {
		if (course.code == code_first) {
			first = graduated ? course.graduates : course.students;
			std::sort(first.begin(), first.end());
		}
		if (course.code == code_second) {
			second = graduated ? course.graduates : course.students;
			std::sort(second.begin(), second.end());
		}
	}
	if (cmp == Comparison::intersection) {
		std::set_intersection(first.begin(), first.end(), second.begin(), second.end(), std::back_inserter(ret));
	} else if (cmp == Comparison::difference) {
		std::set_difference(first.begin(), first.end(), second.begin(), second.end(), std::back_inserter(ret));
	} else if (cmp == Comparison::group_union) {
		std::set_union(first.begin(), first.end(), second.begin(), second.end(), std::back_inserter(ret));
	}
	return ret;
}

std::ostream& operator<<(std::ostream& os, Course const& course) {
	os << "name=" << course.name << '\n';
	os << "code=" << course.code << '\n';
	os << "credits=" << course.credits << '\n';
	os << "department=" << util::enum_to_int(course.department) << '\n';
	os << "rules=" << course.rules << '\n';
	os << "teachers=";
	util::write_array(os, course.teachers);
	os << "students=";
	util::write_array(os, course.students);
	os << "pending=";
	util::write_array(os, course.pending);
	os << "graduates=";
	util::write_array(os, course.graduates);

	return os;
}

std::ostream& operator<<(std::ostream& os, Rules const& rules) {
	os << rules.num_of_courses_passed << "|";
	if (rules.required_courses.empty()) {
		os << "0|";
	} else {
		size_t i{};
		for (const auto& r : rules.required_courses) {
			os << r << ((i++ < rules.required_courses.size() - 1) ? "," : "");
		}
		os << "|";
	}
	os << rules.average_grade;
	return os;
}

void Rules::read_rules(std::string const& str) {
	auto rules_string = util::parse_list(str, '|');
	if (rules_string.size() != 3) {
		*this = {};
		return;
	}
	num_of_courses_passed = std::stoi(rules_string[0]);
	auto temp = util::parse_list(rules_string[1], ',');
	if (temp.size() > 1 && temp[0] != "0") required_courses = {temp.begin(), temp.end()};
	average_grade = std::stoi(rules_string[2]);
}

void Rules::input_rule() {
	size_t selection = -1;

	while (selection) {

		std::cout << "Select an option:\n";
		std::cout << "0. Quit:\n";
		std::cout << "1. Minimum amount of passed courses:\n";
		std::cout << "2. Required passed courses:\n";
		std::cout << "3. Minimum average grade:\n";
		std::cin >> selection;
		switch (selection) {
		case 0: return;
		case 1: std::cin >> num_of_courses_passed; break;
		case 2: {
			auto courses = Course::read_courses("data/courses.txt");
			for (auto const& course : courses) {
				std::cout << course.get_name() << " : " << course.get_code() << std::endl;
			}
			std::string choice{};
			while (choice != "0") {
				std::cout << "Input code (0 = quit): ";
				std::cin >> choice;
				if (choice != "0") required_courses.insert(choice);
			}
		} break;
		case 3:
			std::cout << "Minimum average grade: ";
			std::cin >> average_grade;
			break;
		}
	}
}

void Course::list_courses() const {
	auto courses = read_courses("data/courses.txt");
	for (size_t i = 0; i < courses.size(); ++i) {
		std::cout << i + 1 << ". " << courses[i].code << " | " << courses[i].name << '\n';
	}
}
void Course::list_enrolled(std::string const& username) {
	auto courses = read_courses("data/courses.txt");
	size_t i = 1;

	for (auto const& course : courses) {
		if (std::find(course.students.begin(), course.students.end(), username) != course.students.end()) {
			std::cout << i++ << ". " << course.code << " | " << course.name << '\n';
		}
	}
}

void Course::delete_course(std::string const& code) {
	auto courses = read_courses("data/courses.txt");

	if (course_exists(code)) {
		auto match_code = [code](const Course& course) { return course.code == code; };
		courses.erase(std::find_if(courses.begin(), courses.end(), match_code));
	}
	write_courses(courses, "data/courses.txt");
}

bool Course::course_exists(std::string const& code) {
	auto courses = read_courses("data/courses.txt");
	bool ret = false;
	for (auto const& course : courses) {
		if (course.code == code) {
			ret = true;
		}
	}
	return ret;
}

void Course::write_courses(std::vector<Course> const& courses, std::string const& path) {
	if (auto file = std::ofstream(path)) {
		for (auto const& course : courses) {
			file << course << "=====\n";
		}
	} else {
		std::cout << "File couldn't be opened.\n";
	}
}

std::pair<std::string, size_t> Course::pass_student(std::string const& username) {
	auto it = std::find(students.begin(), students.end(), username);
	if (it != students.end()) {
		std::cout << "Input student grade[6 - 10]: ";
		size_t grade{};
		std::cin >> grade;
		if (grade >= 6 && grade <= 10) {
			students.erase(it);
			graduates.push_back(username);
			update_course("data/courses.txt");
			std::cout << "Student passed " << name << " with " << grade << ".\n";
			return {code, grade};
		} else {
			return {code, 0};
		}
	} else {
		std::cout << "Student not found.\n";
	}
	return {code, 0};
}
