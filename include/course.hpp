#pragma once

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include "util.hpp"

struct Rules {
	size_t num_of_courses_passed{};
	std::set<std::string> required_courses{};
	double average_grade{};
	void read_rules(std::string const& str);
	void input_rule();
	friend std::ostream& operator<<(std::ostream& os, Rules const& rules);
};

enum class Department {
	automatika,
	elektroenergetika,
	elektronika,
	komunikacije,
	elektrotehnika,
	informatika,
};

enum class Comparison {
	intersection,
	group_union,
	difference,
};

class Course {
	std::string name{};
	std::string code{};
	std::size_t credits{};
	Department department{};
	Rules rules{};
	std::vector<std::string> teachers{};  // list of teachers
	std::vector<std::string> students{};  // list of enrolled students
	std::vector<std::string> pending{};	  // list of students that haven't been accepted yet
	std::vector<std::string> graduates{}; // list of graduated students

	static bool course_exists(std::string const& code);
	static void write_courses(std::vector<Course> const& courses, std::string const& path);

  public:
	Course() = default;
	Course(std::string name, std::string code, std::size_t credits, Department department, Rules rules,
		   std::vector<std::string> teachers, std::vector<std::string> students,
		   std::vector<std::string> pending_students, std::vector<std::string> graduates);

	[[nodiscard]] std::string get_name() const { return name; }
	[[nodiscard]] std::string get_code() const { return code; }
	[[nodiscard]] std::vector<std::string> get_teachers() const { return teachers; }
	[[nodiscard]] std::vector<std::string> get_students() const { return students; }
	[[nodiscard]] std::vector<std::string> get_pending() const { return pending; }
	[[nodiscard]] std::vector<std::string> get_graduates() const { return graduates; }
	[[nodiscard]] Rules get_rules() const { return rules; }

	void set_teachers(std::vector<std::string> other) { teachers = std::move(other); }
	void set_students(std::vector<std::string> other) { students = std::move(other); }
	void set_pending(std::vector<std::string> other) { pending = std::move(other); }
	void set_graduates(std::vector<std::string> other) { graduates = std::move(other); }

	void append_course(std::string const& path) const;
	static void create_course(bool is_admin);
	static void delete_course(std::string const& code);
	void update_course(std::string const& path);
	void add_teacher(std::string const& teacher, bool is_admin);
	static std::vector<Course> read_courses(std::string const& path);

	void list_courses() const;
	// prints out courses the given user is enrolled into
	static void list_enrolled(std::string const& username);
	std::pair<std::string, size_t> pass_student(std::string const& username);

	static std::vector<std::string> compare(std::string const& code_first, std::string const& code_second,
											Comparison cmp, bool graduated);
	// add student to the list of pending students
	void enroll_student(std::string const& student);
	// teacher approves student enrollment
	void accept_user(std::string const& requested_user, std::string const& username, bool is_admin);
	friend std::ostream& operator<<(std::ostream& os, Course const& course);
};
std::ostream& operator<<(std::ostream& os, Course const& course);
