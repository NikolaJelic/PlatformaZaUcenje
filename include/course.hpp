#pragma once

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <string>
#include <vector>
#include "user.hpp"
#include "util.hpp"

// Needs to be moved to Utils
enum class Rules {
	course_passed,				 // student passed a specific course
	amount_passed_with_atribute, // student passed a specific amount of courses with a certain
								 // atribute
	passed_with_grade,			 // passed specific courses with a specific grade
	each_department,			 // passed a course from each listed department
};

enum class Department {
	automatika,
	elektroenergetika,
	elektronika,
	komunikacije,
	elektrotehnika,
	informatika,
};



// all courses are stored in data/courses.txt
class Course {
	std::string name{};
	std::string code{};
	std::size_t credits{};
	Department department{};
	std::vector<Rules> rules{};
	std::vector<std::string> conditions{};
	std::vector<std::string> teachers{};		 // list of teachers
	std::vector<std::string> students{};		 // list of enrolled students
	std::vector<std::string> pending_students{}; // list of students that haven't been accepted yet
	std::vector<std::string> graduates{};		 // list of graduated students

  public:
	Course(std::string name, std::string code, std::size_t credits, Department department, std::vector<Rules> rules,
		   std::vector<std::string> conditions, std::vector<std::string> teachers, std::vector<std::string> students,
		   std::vector<std::string> pending_students, std::vector<std::string> graduates);
	std::string get_code() const { return code; }
	void enroll_student(std::string const& student);
	void update_course(std::string const& path);
	bool can_enroll(User const& user) const;
	// only admins can use this
	void add_teacher(std::string const& teacher, bool is_admin);
	static std::vector<Course> read_courses(std::string const& path);

	void accept_user(std::string const& requested_user, std::string const& username, bool is_admin);
	friend std::ostream& operator<<(std::ostream& os, Course const& course);
};
std::ostream& operator<<(std::ostream& os, Course const& course);
