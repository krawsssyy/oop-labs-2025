#include <iostream>
#include <string> // for std::getline
#include <iomanip> // for std::setprecision
#include <cstring> // for proper memcpy/memset

class Student {
	// instead of using a struct, we now use the more powerful class construct (it is merely a struct with access modifiers)
	private:
		std::string name;
		float* grades;
		float avg;
		int noGrades;
	public:
		// NOTE: in the constructors,  we do not read input for our data, we only initialize our internal attributes
		// (optionally we can validate the data to ensure that we can at least create an object successfully)
		// with either default attributes or the values from the parameters passed to the constructors
		// we read the data outside of the class and validate it before initializing it and then pass it inside of the class via the constructor
		Student() : name(""), grades(nullptr), avg(0.f), noGrades(0) { } // empty body since we didn't have anything else to add
		// this is called a constructor with an initialization list
		// we can use ":" followed by our list of members, and next to each one, in parentheses, we provide the values that we want them to take
		// it is not mandatory that we initialize all fields in the initialization list, we could do part in the list, and part in the body
		// it is equivalent to the following code:
		// Student() {
		//	this->name = "";
		//	this->grades = nullptr;
		//	this->avg = 0.0f;
		//	this->noGrades = 0;
		// }
		// --
		// inside of a class, we use the "this" pointer to reference the class's objects
		// each member variable and function we reference inside of our class's definition, we use "this" followed by a "->" (because "this" is a pointer, and for pointers "->" is the access operator for internal members)
		// the "this" pointer references the "current" object of the class
		// it is a tad abstract, but when defining a class, this that you are working on a specific object, and "this" references that one such that you can differentiate between other members and the internal members of the class
		// --
		// furthermore, this is a default constructor (constructor without parameters)
		// this is called when we want to create an "empty" object of our class, and fill it with default values, such that we make sure that it is properly initialized
		// this constructor is called automatically when we instantiate an object of this class, without providing any parameters
		// for e.g.
		// Student s1;
		// Student s2{}; // beware, using "Student s2();" is wrong - it is a case of "most vexing parse", which is an issue in C++ where it can't distinguish between a function declaration and an object's parameters, and chooses the first variant
		// Student s3 = Student(); // this not only calls the default constructor, but more on that later when we learn about copy constructors - therefore we don't prefer this
		// the previous statement is valid until C++17, where copy elision was added and this no longer calls the copy constructor
		// Student* s4 = new Student;
		// Student* s5 = new Student();
		// Student* arrStudents = new Student[10]; // each element in the list will be initialized via the default constructor
		// NOTE: if we didn't have any dynamic field or other nested complex classes, we could've done
		// Student() = default; // which basically tells the compiler that for our default constructor
		// we want to call the default constructor for each individual member that we have and initialize it as such
		// but be careful, this only works when we have as member classes which have defined their default constructor
		// in our case, only std::string is correct and will get a default value of "", because the other members have fundamental types which don't have default constructor
		// the rest (the number of grades, the average, and our grades pointer) will remain uninitialized (and have random values from memory)
		Student(std::string name, const float* grades, int noGrades): name(name), noGrades(noGrades), avg(0.f) { // this is a parametrized constructor
			// it allows us to pass values from outside, into our object, such that we can create it
			// it is equivalent to creating a struct using { } or setting its parameters member-wise with some given values
			// just that in this case, we have a special function for it
			// it is automatically called when we initialize a class with parameters
			// for e.g.
			// float* arr = new float[3];
			// arr[0] = 5.5f; arr[1] = 6.6f; arr[2] = 7.7f;
			// Student s1("Maria", arr, 3);
			// Student s2 = Student("Andrei", arr, 3); // this not only calls the parametrized constructor, but more on that later - therefore we don't prefer this
			// Student* s3 = new Student("Alex", arr, 3);
			// in this case we use the initialization list as well, to pass some parameters
			// that is the equivalent of writing "this->name = name; this->noGrades = noGrades; this->avg = 0.f;" inside this constructor, before the "this->grades = new float[noGrades];" line
			// ideally, in all types of constructors, we want to provide values for all the attributes of our class

			// setting the parameter array as consts forbids us from accidentally performing any operations on it and modifying it
			// it is a good practice to mark it as such when no modifications occur (any attempt now will lead to a compiler error)
			if (!grades || this->noGrades < 1) { // validate the data
				this->noGrades = 0;
				this->grades = nullptr;
			}
			else {
				this->grades = new float[this->noGrades];
				memcpy(this->grades, grades, this->noGrades * sizeof(float));
			}
			// memcpy is a function which copies a given amount of bytes from a source buffer to a destination buffer
			// memcpy(dst_ptr, src_ptr, no_bytes);
			// it is equivalent of doing (more or less - beware of datatypes between the source and destination buffer)
			// for (int i = 0; i < this->noGrades; i++) this->grades[i] = grades[i];
			// NOTE: as in memset's case, the third parameter represents the amount of BYTES
			// therefore, using only this->noGrades would be wrong, since that's just the number of elements
			// since each element in our list is a float, we need to multiply the number of elements by the number of bytes a float has to get the correct number of bytes that we need to copy
		}
		// we can have parametrized constructors for whatever combination of parameters we want
		// it is not mandatory that we give them all inside this constructor
		// and we can also have as many such constructors as we want, there's no limit

		Student(std::string name, int noGrades): name(name), noGrades(noGrades), avg(0.f) { // example of a different parametrized constructor
			// where we are given no grades list, and we initialize it to all 0s
			if (this->noGrades < 1) { // validate the data
				this->noGrades = 0;
				this->grades = nullptr;
			}
			else {
				this->grades = new float[this->noGrades];
				memset(this->grades, 0, this->noGrades * sizeof(float));
			}
			// beware, memset only works best for setting 0s
			// since the data type it sets is an int, it will put that value for each byte
			// when the memory is interpreted as a different data type, errors may occur
			// if we wanted to set the array to a different value, we needed to use a for loop
			// coincidentally, for numerical data types, this works fine and sets them all as 0	
		}

		~Student() { // this is another special type of function inside of a class, called a destructor
			// it is automatically called by the program when our class object goes out of scope
			// (either the block of instructions where we defined our object ends, or it is a pointer and we call delete)
			// if (...) {
			//	Student s1 = Student("Andrei", grades, 3);
			//	... // do some more stuff
			// } // destructor automatically called here, when the variable went out of scope
			// or
			// int main() {
			// ... // some stuff
			// Student s1 = Student("Vasile", grades, 5);
			// ... // some more stuff
			// return 0;
			// } // destructor called automatically here, since our variable went out of scope
			// or
			// Student* s2 = new Student("Ioan", grades, 3);
			// ...// do some more stuff
			// delete s2; // destructor automatically called here
			// or
			// Student* arrStudents1 = new Student[3];
			// ...// do some more stuff
			// delete[] arrStudents1; // destructor for each element inside of the list is automatically called here
			if (this->grades) {
				// even though deleting a nullptr is valid in C++, checking for it develops a good practice
				// it is a very good practice to always check your pointers before doing anything with them
				delete[] this->grades; 
				this->grades = nullptr; // avoid dangling pointer and other vulnerabilities that may arise from misuse of this pointer after deletion
			}
		}

		// after defining our special functions such that we can create and ensure proper deletion of objects, we can then create whatever associated functions we want for our class
		// some good helper functions are getters and setters, such that we can retrieve attributes of our class, and set new values for them
		// since our attributes are private, that means that they are accessible via the access operator only inside of our class
		// outside, we will not be able to do "Student s1(); s1.name = "Ana";"
		// instead, to facilitate that, we create getters (which retrieve the value of a field), and setters (which set a new value for a field)
		std::string getName() const { // adding const after the function signature tells the compiler that this function doesn't modify any internal variable
			// it is considered good practice for getters and allows the function to be called for const objects
			return this->name;
		}

		float* getGrades() const {
			// beware, since we return a pointer, the caller has the responsibility of cleaning this up
			// such that we avoid a memory leak
			if (!this->grades || this->noGrades < 1) {
				return nullptr;
			}
			float* gradesCopy = new float[this->noGrades];
			memcpy(gradesCopy, this->grades, this->noGrades * sizeof(float));
			return gradesCopy;
			// in getters for dynamic fields, we don't return the actual field
			// instead, we return copies of them
			// such that any modifications we perform on the outside after retrieving the dynamic field doesn't reflect inside of our class
			// this is called deep-copy, and it is preferred that we only do this when handling dynamic fields to avoid accidental modifications of our class's attributes
		}

		int getNoGrades() const {
			return this->noGrades;
		}

		float getAverage() const {
			return this->avg;
		}

		void setName(std::string name) {
			// ideally, in setters, we would want to validate that the value is actually correct before setting it
			// for constructors, the validation of the data falls mostly to the caller
			// for setters, the validation of the data falls mostly to us
			if (name != "") {
				this->name = name;
			}
			else {
				std::cout << "Name cannot be empty!" << std::endl; // print a message
				// ideally we would throw an error, but in this case it would mess up our allocations and we'd need to add more future concepts to properly handle this
				// we will see more about this on exceptions, what they are, and how we can catch them
			}
		}

		void setGrades(const float* grades, int noGrades) {
			if (!grades) { // fast-fail if our given array is null
				std::cout << "Invalid array given!" << std::endl;
				return;
			}
			if (noGrades >= 1) {
				this->noGrades = noGrades;
				if (this->grades) { // delete our array if initialized before re-allocating it
					delete[] this->grades;
					this->grades = nullptr;
				}
				this->grades = new float[this->noGrades];
				for (int i = 0; i < this->noGrades; i++) {
					if (grades[i] < 1.f || grades[i] > 10.f) {
						std::cout << "Invalid value for grade! Defaulting to 1." << std::endl;
						this->grades[i] = 1.f; // equivalent to 1.0f;
					}
					else {
						this->grades[i] = grades[i];
					}
				}
			}
			else {
				std::cout <<  "Number of grades cannot be less than 1!" << std::endl;
			}
		}
		// since we have now created useful helpers for our class, we are now done with the "sort-of mandatory" functions
		// and can now create whatever functions we may deem necessary for the good functioning of our class
		// in our case, a function for calculating the average is perfectly fitted to be used inside of our class
		void calculateAverage() {
			if (this->grades && this->noGrades > 0) { // ensure noGrades is bigger than 0 since we divide by it
				this->avg = 0.f;
				for (int i = 0; i < this->noGrades; i++) {
					this->avg += this->grades[i];
				}
				this->avg /= this->noGrades;
			}
			else {
				std::cout << "Grades array is not allocated or the number of grades is 0! We cannot compute the average!" << std::endl;
				this->avg = 0.f; // reset it
			}

		}

};
int main() {
	int noStudents;
	std::cout << "Enter number of students: "; std::cin >> noStudents;
	if (noStudents < 1) { // validate user input
		std::cout << "Invalid number of students! The number must be bigger than 0." << std::endl;
		return 1; // exit if the number of students is wrong
		// since nothing was allocated until this point, no need to de-allocate anything
	}

	Student* students = new Student[noStudents]; // declare the array of students
	// it automatically calls the default constructor for each element in the list
	float minAvg = 11.0f, maxAvg = 0.f; // initialize variables for tracking the minimum and maximum averages
	int minPos = -1, maxPos = -1;
	// initialize variables for keeping track of the position of the smallest, respectively biggest average
	for (int i = 0; i < noStudents; i++) {
		std::string name;
		std::cout << "Enter student name for student " << i + 1 << " :";
		std::getline(std::cin >> std::ws, name); // reads the student's name with spaces included, and discard leftover '\n' from previous reads
		int noGrades;
		std::cout << "Enter number of grades for student " << i + 1 << " :";
		std::cin >> noGrades;
		if (noGrades < 1) {
			std::cout << "Number of grades cannot be less than 1!" << std::endl;
			students[i].setName(name); // we set the name of the student
			// since the default constructor was called, all attributes have default values
			// meaning that our grades is nullptr, number of grades is 0 and the average is 0
			// exactly how it should be for this case
			continue; // move to the next step of the for loop without executing the code below
		}
		float* grades = new float[noGrades]; // initialize grades dynamic array

		for (int j = 0; j < noGrades; j++) {
			std::cout << "Enter grade " << j + 1 << " for student " << i + 1 << " : ";
			std::cin >> grades[j];
			if (grades[j] < 1.f || grades[j] > 10.f) {
				std::cout << "Invalid student grade! Grades must be between 1 and 10! Skipping student " << i + 1 << std::endl;
				delete[] grades; // delete the grades array
				grades = nullptr;
				break; // exit the loop and do not continue with future steps
			}
		}

		if (grades) { // check if dynamic array is not de-allocated
			// if it is de-allocated, it means we errored and we cannot use it for our student initialization
			students[i].setName(name);
			students[i].setGrades(grades, noGrades); // also sets the number of grades
			// students[i] = Student(name, grades, noGrades); // we don't do this yet, since it requires another special type of constructor, which we haven't done yet
			// our class doesn't adhere to the rule of three, which is a golden principle in C++
			// it states that if any of these special function is implemented (destructor, copy constructor or assignment operator), all should be present
			// for more reading, there's the rule of five, which adds the move constructor and operator to the aforementioned list (this is for the students looking to get more advanced knowledge of C++ mostly)
			// with that copy constructor not defined, this would shallow-copy our pointer and result in a double-free vulnerability when the object in the array was destroyed
			delete[] grades; // delete dynamic array
			// this won't lead to a double free since the array in our student object is a deep-copy of this
			grades = nullptr;
		}
		else { // if we have encountered a bad grade
			students[i].setName(name);
			// students[i] = Student(name, noGrades); // not this yet, since it would require more stuff
			// use our other parametrized constructor to construct this object
			// the program will automatically know what constructor to use based on the number of parameters and their types
			// called function overloading (having multiple functions with the same name but different signatures)
			// we don't need to delete anything here since the grades array is already de-allocated
		}
		students[i].calculateAverage(); // calculate the average internally for the current student object
		// update our minimum and maximum values
		if (students[i].getAverage() < minAvg && students[i].getAverage() > 0.f) { // ensure we only check minimum averages
			// for students where we have actual correct grades
			minAvg = students[i].getAverage();
			minPos = i;
		}
		if (students[i].getAverage() > maxAvg) {
			maxAvg = students[i].getAverage();
			maxPos = i;
		}
	}

	std::cout << std::endl;
	for (int i = 0; i < noStudents; i++) {
		std::cout << std::fixed << std::setprecision(2) << "Student " << students[i].getName() << " has the average grade of "
			<< students[i].getAverage() << std::endl;
	}

	if (minPos != -1) { // handle the case where only wrongly added students were used
		// we do this check to ensure that our indexing in the students array is correct and within bounds
		std::cout << std::endl << std::fixed << std::setprecision(2) << "Student " << students[minPos].getName()
			<< " has the lowest average grade: " << minAvg << std::endl;
	}

	if (maxPos != -1) { // handle the case where only wrongly added students were used
		std::cout << std::endl << std::fixed << std::setprecision(2) << "Student " << students[maxPos].getName()
			<< " has the highest average grade: " << maxAvg << std::endl;
	}

	delete[] students; // cleanup for students array
	// will automatically call the destructor for each element inside
	// the destructor for the class will cleanup the dynamic arrays inside
	// so, with this, we are all properly cleaned up
	students = nullptr;

	return 0;
}