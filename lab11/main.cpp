#define _CRT_SECURE_NO_WARNINGS
#include <iostream> // for std::cin, std::cout, std::istream, std::ostream
#include <fstream> // for std::ofstream, std::ifstream
#include <cstring> // for strcpy, strlen

class Student {
	private:
		char name[20];
		int age;
		int noClasses;
		float* grades;
		bool isPassing;
	public:
		// default constructor
		Student() : age(0), noClasses(0), grades(nullptr), isPassing(false) { this->name[0] = '\0'; } // raw arrays cannot be initialized within the initialization list
		// param constructor
		Student(const char name[], int age, int noClasses, float* grades, bool isPassing) :isPassing(isPassing) {
			if (age < 6) {
				std::cout << "No students under 6 allowed!" << std::endl;
				this->age = 6;
			}
			else {
				this->age = age;
			}
			if (noClasses < 1) {
				std::cout << "Cannot have negative or null number of classes!" << std::endl;
				this->noClasses = 0;
				this->grades = nullptr;
			}
			else {
				if (!grades) {
					std::cout << "Invalid array of grades!" << std::endl;
					this->noClasses = 0;
					this->grades = nullptr;
				}
				else {
					this->noClasses = noClasses;
					this->grades = new float[this->noClasses];
					memcpy(this->grades, grades, this->noClasses * sizeof(float));
				}
			}
			strcpy(this->name, name);
		}
		// copy constructor
		Student(const Student& other): age(other.age), noClasses(other.noClasses), isPassing(other.isPassing) {
			strcpy(this->name, other.name);
			if (this->noClasses) {
				this->grades = new float[this->noClasses];
				memcpy(this->grades, other.grades, this->noClasses * sizeof(float));
			}
			else {
				this->grades = nullptr;
			}
		}
		// destructor
		~Student() {
			if (this->grades) {
				delete[] this->grades;
				this->grades = nullptr;
			}
		}

		// mark our streaming operators as friend, such that they can access the private attributes
		// think of streams as special buffers or files, which act as both the source and destination for various operations
		// we can send data to them, or extract data from them
		// when we send data to a stream, that data is stored in its internal buffers, until we consume it
		// (for e.g., when we send data to the console output stream (cout), it gets consumed instantly and printed to the console)
		// (when we send it to the file output stream, it gets consumed and written into the file)
		// when we extract data from the stream, that data is moved from its internal buffers into our defined buffers
		// (for e.g., when we extract data from the console input stream (cin) (data gets saved there as soon as we enter it), it gets moved from the stream (whatever was saved when we entered our data into the console) into our variables)
		// (when we extract data from the file input stream (data gets there as soon as we open the file), the data read from the file is moved into our variables)

		// the streaming operators work on general input and output streams, and can therefore be used on regular console streams or file streams

		// for reading/writing files as text, we use the streaming operators, since they only work with text-based inputs (or easily convertible to text)
		friend std::ostream& operator<<(std::ostream& os, const Student& student);
		friend std::istream& operator>>(std::istream& is, Student& student);
		// beware of the reference return type, we need to return the actual modified object for chaining and return value checks to work


		// function for writing binary data to a file (output file stream)
		// when we save in binary, we save the actual bytes of each attribute
		// such that we can sometimes compress the data, and save it in a better way, without having to account for separators or manipulations when reading
		void serialize(std::ofstream& ofs) const {
			// for writing/reading binary to files, we don't use the streaming operators
			// instead, we use the write() and read() function, respectively
			// SYNTAX: write(src_as_char*, size_in_bytes);
			// it requires the source to be as a character array, since it works with bytes, and it expects to write some bytes
			// since char is 1 byte, a char array is the simplest form of a byte array, regardless of endianness and data types
			// so, when writing, convert your data to (char*), and use as reference to your variable (meaning its address)
			// so the program interprets the bytes at that address as a character array (which is what we want)
			// that way, the variable isn't changed at all, and just its actual representation is memory is treated as individual bytes, rather than a group that forms a different data type
			// if we were to not use the reference, the actual value would be casted to another type which could potentially change the variable's value, and we don't want that
			
			// since we write bytes, we need to provide the actual length in bytes, so beware of that
			// as with the streaming operators for saving to file, we first write the length of the array
			// and then the array
			// such that when reading, we know how many values to read
			// this time, we need to do this even for character arrays, as the read function doesn't know to stop at \0 when reading a char[]
			int length = strlen(this->name) + 1;// to include the null-terminator, so we don't have to handle it ourselves separately
			ofs.write((char*)&length, sizeof(int));
			ofs.write(this->name, length); 
			ofs.write((char*)&this->age, sizeof(int));
			ofs.write((char*)&this->isPassing, sizeof(bool));
			ofs.write((char*)&this->noClasses, sizeof(int));
			if (this->noClasses) {
				// since grades is already a pointer, we just pass it as is, without reference
				// since that would corrupt the pointer, and take the address of the pointer variable as the byte array
				// and not its elements
				ofs.write((char*)this->grades, this->noClasses * sizeof(float));
			}
		}

		void deserialize(std::ifstream& ifs) {
			// SYNTAX: read(dest_as_char*, size_in_bytes)
			// when reading, we repeat the same process as before, just that this time we read into the variables
			// the logic for using conversion to char* and using reference to write the actual bytes as we saved them is the same as in the serialization's case
			// we want to write the bytes exactly as we saved them, such that the value is interpreted back to its original type in exactly the same way
			int length;
			ifs.read((char*)&length, sizeof(int));
			ifs.read(this->name, length);
			ifs.read((char*)&this->age, sizeof(int));
			ifs.read((char*)&this->isPassing, sizeof(bool));
			ifs.read((char*)&this->noClasses, sizeof(int));
			if (this->noClasses) {
				if (this->grades) {
					delete[] this->grades;
				}
				this->grades = new float[this->noClasses];
				ifs.read((char*)this->grades, this->noClasses * sizeof(float));
			}
			else {
				if (this->grades) {
					delete[] this->grades;
				}
				this->grades = nullptr;
			}
		}

		// NOTE: whenever we save streams as parameters, we need to use references to them, such that the actual streams get modified
		// either by extracing data from them or sending data to them
		// streams have a deleted copy constructor, meaning that it is not allowed to call the copy constructor for them
		// therefore, we need to send them as references

		// small helper to check equality between 2 objects to ensure writing and reading worked correctly
		bool operator==(const Student& other) const {
			return strcmp(this->name, other.name) == 0 && 
				this->age == other.age && 
				this->isPassing == other.isPassing && 
				this->noClasses == other.noClasses &&
				(this->grades != nullptr && other.grades != nullptr ? memcmp(this->grades, other.grades, this->noClasses * sizeof(float)) == 0 :
					(this->grades == nullptr && other.grades == nullptr ? true : false));
		}
};

std::ostream& operator<<(std::ostream& os, const Student& student) {
	// in here, we define how we want our class to be displayed, either to the console output or the file output
	// for simplicity when reading, input streams separate by space, so we'll print everything separated by space

	// if we want to use this for printing to the screen as well as saving to a file, we have to be aware of how we save the data
	// using specific prints will only make life harder when trying to read, as extensive string manipulation will be necessary

	// additionally, since we have a dynamic array, we save the length first, and then the array
	// this is done such that when we read it back, we know the length we need to read in order to avoid misreads
	os << student.name << " " << student.age << " " << student.isPassing << " " << student.noClasses;
	if (student.noClasses) {
		// be mindful of spaces, as they can mess your input stream
		os << " ";
		for (int i = 0; i < student.noClasses; i++) {
			os << student.grades[i];
			if (i != student.noClasses - 1) {
				os << " ";
			}
		}
	}
	os << std::endl; // insert an endl to separate records
	return os;
}

std::istream& operator>>(std::istream& is, Student& student) {
	// in here, we define how we want our class to be instantiated from data available in a stream (be it a file or std::cin) (i.e. data read from a file or from the console)
	// since we used our separator nicely, we can now just simply chain-read everything
	// NOTE: Please avoid asking for the variables here via std::cout, as if we don't use std::cin to read them
	// we will have just empty prints in here
	is >> student.name >> student.age >> student.isPassing >> student.noClasses;
	if (student.noClasses) {
		if (student.grades) {
			delete[] student.grades;
		}
		student.grades = new float[student.noClasses];
		for (int i = 0; i < student.noClasses; i++) {
			is >> student.grades[i];
		}
	}
	else {
		if (student.grades) {
			delete[] student.grades;
		}
		student.grades = nullptr;
	}
	return is;
}

int main() {
	float arr[] = { 5.f, 6.f, 7.f };
	float arr2[] = { 2.f, 3.f, 4.f };
	Student A("Gigi", 21, 3, arr, true);
	Student B("Vasile", 20, 3, arr2, false);
	Student C;

	// testing streaming operators with std::cin and std::cout

	std::cout << "Student A:" << std::endl << A << std::endl;
	std::cout << "Student B:" << std::endl << B << std::endl;
	std::cout << "Student C:" << std::endl << C << std::endl;

	std::cout << "Please give details of student C in the console: ";
	std::cin >> C;
	std::cout << std::endl << "Student C after reading it from std::cin:" << std::endl << C;

	// testing streaming operators for file

	// by default and unless otherwise specified, all streams start at the beginning of the file (except std::ios::ate is used)
	std::ofstream ofs("textFileOutput.txt", std::ios::out | std::ios::trunc); // since the file may not be yet created, we specify out to create it if it doesn't exist
	// we also specify std::ios::trunc, to overwrite, so we always start from a clean sheet
	// no need to use std::ios::ate (at the end), to move the file pointer at the end at the start, since the file gets re-created every time (and end = start)
	// (std::ios::ate just sets the file pointer at the end at the opening, and then allows us to move freely)
	// if we wanted to append to an existing file, we would use std::ios::app, which moves the file pointer at the end upon each write
	ofs << A;
	ofs.close(); // we close our file streams when we're done with them

	std::ifstream ifs("textFileOutput.txt"); 
	if (ifs.is_open()) {
		ifs >> C;
		std::cout << "Student C after reading it from a file (values from student A): " << std::endl << C << std::endl;
		std::cout << "After writing A and reading into C => A == C is " << (A == C ? "true" : "false") << std::endl;
	}
	else {
		throw "Failed to open text file for reading!";
	}
	ifs.close();

	// testing serialization and deserialization for binary files
 
	// we use same flags as before, just that we add std::ios::binary, to signify that this is a binary file stream
	std::ofstream ofsBin("binaryFileOutput.bin", std::ios::binary | std::ios::out | std::ios::trunc);
	B.serialize(ofsBin);
	ofsBin.close();

	std::ifstream ifsBin("binaryFileOutput.bin", std::ios::binary);
	if (ifsBin.is_open()) {
		C.deserialize(ifsBin);
		std::cout << "Student C after reading it from a file (values from student B): " << std::endl << C << std::endl;
		std::cout << "After writing B and reading into C => B == C is " << (B == C ? "true" : "false") << std::endl;
	}
	else {
		throw "Failed to open binary file for reading!";
	}
	ifsBin.close();
	
	return 0;
}