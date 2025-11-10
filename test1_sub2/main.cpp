#include <iostream>
#include <string>
#include <cstring>
// 0.5pt.Implement the Sensor class, with the following attributes(all class attributes are private)
enum class State { OK, WARNING, ERROR, UNKNOWN };
class Sensor {
private:
	// 0.2pt. All class attributes have default values
	int id = -1;
	int numberOfMeasurements = 0;
	float* measurements = nullptr;
	State state = State::UNKNOWN;
	static int idCounter;
public:
	// 0.2pt.Define the default class constructor.state must be “UNKNOWN”.
	Sensor() {
		// 0.1pt.Correctly handle the static attribute and the initialization of id.
		this->id = Sensor::idCounter++; // assign current id counter to id and increase the static attribute
	}
	// 0.5pt. Define a public interface for the measurements array. The setter will validate the restriction previously imposed. Otherwise, it will print an error message.
	void setMeasurements(float* measurements, int numberOfMeasurements) {
		// validations
		if (numberOfMeasurements < 1) {
			std::cout << "Number of measurements cannot be less than 1!" << std::endl;
			return;
		}
		if (!measurements) {
			std::cout << "Array of measurements cannot be null!" << std::endl;
			return;
		}
		// ensure restriction
		for (int i = 0; i < numberOfMeasurements; i++) {
			if (measurements[i] < 0 || measurements[i] > 130) {
				std::cout << "Measurement not in [0, 130]!" << std::endl;
				return;
			}
		}
		// if we reached here, the restrictions and validations passed, meaning that our array is correct
		this->numberOfMeasurements = numberOfMeasurements;
		if (this->measurements) {
			delete[] this->measurements; // cleanup for current array
		}
		// 0.3pt. The setter does not perform shallow copy.
		this->measurements = new float[this->numberOfMeasurements];
		// use memcpy to copy the exact bytes from the given array to the destionation array
		memcpy(this->measurements, measurements, sizeof(float) * this->numberOfMeasurements);
	}

	float* getMeasurements() const {
		if (!this->measurements) { // ensure we don't do bad stuff if the array isn't allocated
			return nullptr;
		}
		float* cpy = new float[this->numberOfMeasurements];
		// 0.3pt. The getter does not perform shallow copy.
		memcpy(cpy, this->measurements, sizeof(float) * this->numberOfMeasurements);
		return cpy;
	}

	// 0.4pt.Define the parametrized constructor(with all parameters) for your class.The constructor must validate the data.
	Sensor(int numberOfMeasurements, float* measurements, State state) {
		// give them default values in case the setter fails
		this->measurements = nullptr;
		this->numberOfMeasurements = 0;
		this->setMeasurements(measurements, numberOfMeasurements); // also does the validations
		this->state = state;
		// 0.1pt. Correctly handle the static attribute and the initialization of id.
		this->id = Sensor::idCounter++;
	}

	// 0.5pt. Correctly define the class destructor.
	~Sensor() {
		if (this->measurements) {
			delete[] this->measurements;
			this->measurements = nullptr;
		}
	}

	// 1pt. Implement the compactPrint() function, which will print the details of a Sensor object on a single line(id state numberOfMeasurements average_of_measurements).
	void compactPrint() const {
		// deduce genre string
		std::string stateStr;
		switch (this->state) {
		case State::OK:
			stateStr = "OK";
			break;
		case State::WARNING:
			stateStr = "WARNING";
			break;
		case State::ERROR:
			stateStr = "ERROR";
			break;
		case State::UNKNOWN:
			stateStr = "UNKNOWN";
			break;
		default:
			std::cout << "Something really bad occured!" << std::endl;
			return;
		}

		// calculate average + ensure the array is valid
		float avg = 0.f;
		if (this->measurements) {
			for (int i = 0; i < this->numberOfMeasurements; i++) {
				avg += this->measurements[i];
			}
			avg /= this->numberOfMeasurements;
		}

		// do the print
		std::cout << this->id << " " << stateStr << " " << this->numberOfMeasurements << " " << avg << std::endl;
	}

	// 2pt. Implement the addMeasurement(float newMeasurement) function, which will add a new measurement to the dynamic array, if the new measurement adheres to the given restrictions.
	void addMeasurement(float newMeasurement) {
		if (newMeasurement < 0 || newMeasurement > 130) {
			std::cout << "New measurement not in [0, 130]!" << std::endl;
			return;
		}

		if (this->measurements) {
			// resize, since we're in the case where we already allocated the array
			float* aux = new float[this->numberOfMeasurements + 1];
			memcpy(aux, this->measurements, sizeof(float) * this->numberOfMeasurements);
			aux[this->numberOfMeasurements++] = newMeasurement; // add new measurement and also increment the number of measurements
			// cleanup our initial array
			delete[] this->measurements;
			this->measurements = aux; // move the new array into our class'
		}
		else {
			this->measurements = new float[1];
			this->measurements[this->numberOfMeasurements++] = newMeasurement;
		}
	}


};

int Sensor::idCounter = 1; // initialize static attribute

int main() {
	// 0.5pt. Test the constructor in the main function, by creating a sample object with it
	Sensor s1;

	// 0.2pt.Test the previous setter in main.
	float* setTestArr = new float[3];
	setTestArr[0] = 1.f; setTestArr[1] = 2.f; setTestArr[2] = 3.f; // we cannot initialize dynamic arrays using { }, as in the case of static arrays
	// so we do this manully for the test, or use a for loop
	s1.setMeasurements(setTestArr, 3);
	// 0.2pt. Test the previous getter in main and correctly handle the memory.
	float* getTestArr = s1.getMeasurements();
	// ensure match
	bool ok = true;
	for (int i = 0; i < 3; i++) {
		if (setTestArr[i] != getTestArr[i]) {
			ok = false;
			break;
		}
	}
	if (ok) {
		std::cout << "Arrays match!" << std::endl;
	}
	else {
		std::cout << "Arrays don't match!" << std::endl;
	}
	// cleanup
	// since we copied the array in the set inside our class' attribute, we can now safely delete this one, since the class has a safe copy
	delete[] setTestArr;
	setTestArr = nullptr;
	// since the getter gives us a dynamic array, we are resposible for cleaning it up
	delete[] getTestArr;
	getTestArr = nullptr;

	// 0.5pt. Correctly test the constructor in main.
	float* ctorArr = new float[4];
	ctorArr[0] = 1.f; ctorArr[1] = 3.f; ctorArr[2] = 5.f; ctorArr[3] = 7.f;
	Sensor s2(4, ctorArr, State::OK);
	// cleanup
	// the constructor uses the deep copy for the measurements array, so we can safely clean up the array, avoiding double-frees
	delete[] ctorArr;
	ctorArr = nullptr;

	// 0.5pt. Test the previous functionality in main, by printing the details of at least 2 sensors.
	s1.compactPrint();
	s2.compactPrint();

	// 1pt. Test the previous functionality in main for at least 2 sensors (1 correct + 1 incorrect).
	s1.addMeasurement(-131.f);
	s2.addMeasurement(9.f);
	s2.compactPrint();

	return 0;
}
