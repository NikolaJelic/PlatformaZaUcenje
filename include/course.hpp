#pragma once

#include <cstddef>
#include <string>
#include <vector>

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

class Course {
	std::string name{};
	std::size_t code{};
	std::size_t credits{};
	Department department{};
	std::vector<Rules> rules{};
	std::vector<std::string> conditions{};
	std::vector<std::string> teachers{};		 // list of teachers
	std::vector<std::string> students{};		 // list of enrolled students
	std::vector<std::string> pending_students{}; // list of students that haven't been accepted yet
	std::vector<std::string> graduates{};		 // list of graduated students

  public:
	friend std::ostream& operator<<(std::ostream& os, Course const& course);
};
std::ostream& operator<<(std::ostream& os, Course const& course);
