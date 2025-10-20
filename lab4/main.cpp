#include <iostream>
#include <string> // for std::getline
#include <iomanip> // for std::setprecision

struct Student { // we define our student "object", to have a name, a dynamic array of grades, and the average of grades
	// this struct groups together all these variables, allowing us to refer to them as a whole, rather than treating them individually
	std::string name;
	float* grades = nullptr; // it is always a good practice to initialize pointers
	// in this case, we don't know yet the size, so we leave it at nullptr
	// this avoids any undefined behaviour due to wrongly using an uninitialized pointer
	float avg = 0.0f; // initialize the average with 0 (initial value in a summation)
};

int main() {
	int exitCode = 0;
	int noStudents, noGrades;
	std::cout << "Enter number of students: "; std::cin >> noStudents;
	if (noStudents < 1) { // validate user input
		std::cout << "Invalid number of students! The number must be bigger than 0." << std::endl;
		return 1; // exit if the number of students is wrong
	}
	std::cout << "Enter number of grades: "; std::cin >> noGrades;
	if (noGrades < 1) {
		std::cout << "Invalid number of grade! The number must be bigger than 0." << std::endl;
		return 1; // exit if the number of grades is wrong
	}

	Student* students = new Student[noStudents]; // declare the array of students as an array of students
	// variables of struct type are declared and used just like variables of regular data types
	// we just have the addition that we can access member variables with the "." operator (or "->" operator if we have pointer to single struct variable)
	float minAvg = 11.0f, maxAvg = -1.0f; // initialize variables for tracking the minimum and maximum averages
	// we generally initialize the minimum with the first highest value outside the domain (0-10)
	// and the maximum with the first smallest value outside the domain
	int minPos = -1, maxPos = -1;
	// initialize variables for keeping track of the position of the smallest, respectively biggest average
	for (int i = 0; i < noStudents; i++) {
		std::cout << "Enter student name for student " << i + 1 << " : ";
		std::getline(std::cin >> std::ws, students[i].name); // reads the student's name with spaces included, and discard leftover '\n' from previous reads
		students[i].grades = new float[noGrades]; // initialize grades dynamic array

		for (int j = 0; j < noGrades; j++) {
			std::cout << "Enter grade " << j + 1 << " for student " << i + 1 << " : ";
			std::cin >> students[i].grades[j];
			if (students[i].grades[j] < 1.0f || students[i].grades[j] > 10.0f) {
				std::cout << "Invalid student grade! Grades must be between 1 and 10" << std::endl;
				exitCode = 1; // mark error state
				goto CLEANUP;
				// if we have reached a state where the grade is wrong, we terminate the program
				// however, since we have allocated dynamic memory, we need a way to clean it up
				// goto allows us to jump to various labels within our program, redirecting execution flow
				// when we jump to CLEANUP, all code from here, up until where the label is defined IS NOT EXECUTED
				// it is a very powerful tool and can easily mess up the logic of the program and cause horrible bugs
				// one of the appropriate use-cases of "goto" is this, cleaning up memory after error states
				// !! please be advised, only use goto in these situations, and only where it is absolutely necessary (exhaust all other options first)
			}
			students[i].avg += students[i].grades[j];
		}
		students[i].avg /= noGrades;

		// update our minimum and maximum values
		if (students[i].avg < minAvg) {
			minAvg = students[i].avg;
			minPos = i;
		}
		if (students[i].avg > maxAvg) {
			maxAvg = students[i].avg;
			maxPos = i;
		}
	}

	std::cout << std::endl;
	for (int i = 0; i < noStudents; i++) {
		std::cout << std::fixed << std::setprecision(2) << "Student " << students[i].name << " has the average grade of "
			<< students[i].avg << std::endl;
	}

	std::cout << std::endl << std::fixed << std::setprecision(2) << "Student " << students[minPos].name
		<< " has the lowest average grade: " << minAvg << std::endl;

	std::cout << std::endl << std::fixed << std::setprecision(2) << "Student " << students[maxPos].name
		<< " has the highest average grade: " << maxAvg << std::endl;

CLEANUP: // this is called a label
	// it is used to represent a specific place in the program, where we can make our code jump to, if needed
	// correctly cleaning up our dynamically allocated variables
	for (int i = 0; i < noStudents; i++) {
		if (students[i].grades) { // if the grades array was allocated <=> students[i].grades != nullptr
			// we need to check for this since we don't know if we allocated the array or note, since we may error before allocating all arrays
			delete[] students[i].grades; // we need to manually de-allocate all dynamic data allocated inside our struct
			// !!! it does not get de-allocated when we delete the dynamic array of the struct type
			students[i].grades = nullptr; // making the pointers null, also a must!
		}
	}

	delete[] students; // cleanup for students array
	students = nullptr;

	return exitCode;
}
