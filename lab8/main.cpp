#include <iostream>

class Patient {
	private:
		std::string name;
		int age;
		float* testValues;
		int noValues;
		std::string diagnosis;
	public:
		Patient(): name(""), age(0), testValues(nullptr), noValues(0), diagnosis("") {} // default constructor
		Patient(const std::string& name, const int age, const float* testValues, const int noValues, const std::string& diagnosis) { // parametrized constructor
			if (name.empty()) {
				std::cout << "Name cannot be null!" << std::endl;
			}
			else {
				this->name = name;
			}
			// parametrized constructor
			if (age < 0) {
				this->age = 0;
				std::cout << "Age cannot be less than 0!" << std::endl;
			}
			else {
				this->age = age;
			}

			if (noValues < 1) {
				this->noValues = 0;
				std::cout << "Number of values for the tests cannot be less than 1!" << std::endl;
			}
			else {
				this->noValues = noValues;
			}

			if (!testValues) {
				this->testValues = nullptr;
				std::cout << "Test values array cannot be null!" << std::endl;
			}
			else if (!this->noValues) {
				this->testValues = nullptr;
				std::cout << "Test values array is not null but the number of elements is wrong!" << std::endl;
			}
			else {
				this->testValues = new float[this->noValues];
				memcpy(this->testValues, testValues, this->noValues * sizeof(float)); // BEWARE: memcpy requires number of bytes, not number of elements
				// to get the number of bytes required, we multiply the number of elements in the array with the number of bytes per element (obtained via sizeof for the specific type of the array)
				// memcpy performs deep copy, it copies byte by byte from the destination to the source buffer
			}

			this->diagnosis = diagnosis; // it doesn't make sense to perform any checks, we can later provide functionalities for adding a diagnosis
		}

		// COPY CONSTRUCTOR
		// if the default constructor allows us to create an object with default values
		// and the parametrized constructor allows us to define how we create an object from various combinations of parameters
		// a copy constructor allows us to define how we initialize an object from an already existent one
		// it is called as such:
		// Patient A;
		// Patient B(A); // B will have the same values as A (default ones)
		// float* arr = new float[2]; arr[0] = 3.5f; arr[1] = 7.9f;
		// Patient C("Andrei", 28, arr, 2, "");
		// Patient D(C); // D will have the same values as C (the ones from the list of parameters given)
		// delete[] arr; arr = nullptr; // cleanup for dynamic array
		// small note on the example: beware of not using static arrays for mocking up dynamic arrays, since in the destructor
		// you will attempt to delete static memory, which will result in a crash of your application!
		// SYNTAX: <CLASS_NAME>(const <CLASS_NAME>& obj) { ... }
		// it being a constructor, we still use the class's name as the function
		// we use const + & to avoid copies of the object (and that is a requirement for the parameter)
		// (only the "&" is required, to avoid copies, since we are defining the copy constructor, and not using the reference will make it make a copy of the parameter, entering an endless loop)
		// const is optional, but is a good practice to avoid accidental changes
		// NOTE: we can also now do "Patient E = Patient(D);" or "Patient F = D;"
		// this is what's called copy-initialization, and even though it uses the assignment operator, it is not used, since the given object
		// is not already defined (had it been, then the assignment operator would've been used and that is a no-no just yet)

		Patient(const Patient& other): name(other.name), age(other.age), diagnosis(other.diagnosis), noValues(other.noValues) {
			// we can still use the initialization list for the copy constructor
			// since we are taking values from an already initialized object, no need for additional validations
			if (!this->noValues) { // we check if we have any values for proper initialization of the values array
				this->testValues = nullptr;
			}
			else {
				this->testValues = new float[this->noValues];
				memcpy(this->testValues, other.testValues, this->noValues * sizeof(float)); // correctly perform deep copy such that we avoid any issues
			}

			// NOTE:
			// in this case, we can access the class's private parameters, since we are still inside our own class and we manipulate an object of our own type
			// had we created a constructor for any other object type, we wouldn't have been able to do that
		}

		~Patient() { // memory cleanup
			if (this->testValues) {
				delete[] this->testValues;
				this->testValues = nullptr;
			}
		}

		void printDetails() const { // helper function for printing out the details, used for ensuring we correctly copied everything
			std::cout << std::endl << "Name: " << (this->name.empty() ? "John Doe" : this->name) << std::endl;
			std::cout << "Age: " << this->age << std::endl;
			std::cout << "Test values: ";
			if (this->testValues) {
				for (int i = 0; i < this->noValues; i++) {
					std::cout << this->testValues[i] << " ";
				}
				std::cout << std::endl;
			}
			else {
				std::cout << "None" << std::endl;
			}
			std::cout << "Diagnosis: " << (this->diagnosis.empty() ? "None" : this->diagnosis) << std::endl;
		}
};

int main() {
	Patient A; // default object
	std::cout << "Patient A" << std::endl;
	A.printDetails();
	Patient B(A); // copy of A - still a default object
	std::cout << std::endl << "Patient B" << std::endl;
	B.printDetails();
	float* arr = new float[3]; arr[0] = 1.2f; arr[1] = 2.4f; arr[2] = 4.7f;
	Patient C("Maria", 32, arr, 3, "Pneumonia"); // parametrized object
	delete[] arr; // safely delete allocated memory, since it is no longer needed
	// each constructor makes its copy, so we don't have any use for the original anymore
	arr = nullptr;
	std::cout << std::endl << "Patient C" << std::endl;
	C.printDetails();
	Patient D(C); // copy of C - the parametrized object
	std::cout << std::endl << "Patient D" << std::endl;
	D.printDetails();

	return 0;
}
