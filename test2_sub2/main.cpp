#include <iostream>
#include <string>

enum class Specialty { SURGEON, CARDIOLOGIST, NEUROLOGIST, PNEUMOLOGIST, INTERNIST, GENERAL };

class Doctor {
	private:
		std::string name;
		int noPatients;
		std::string* patientsNames;
		Specialty specialty;
		const int maxPatients;

		bool isArrayValid(std::string* arr, int size) { // helper for validating the restriction
			if (size < 1 || !arr) {
				return true;
			}
			else {
				for (int i = 0; i < size; i++) {
					if (arr[i].length() <= 3) {
						return false;
					}
				}
				return true;
			}
		}
	public:
		// default constructor
		Doctor(): maxPatients(20) { // initialize const attribute in initializer list
			this->name = "(empty)";
			this->noPatients = 0;
			this->patientsNames = nullptr;
			this->specialty = Specialty::GENERAL;
		}

		// parametrized constructor with all parameters
		Doctor(std::string name, int noPatients, std::string* patientsNames, Specialty specialty) : maxPatients(20) {
			if (name.length() < 3) {
				std::cout << "Name length cannot be less than 3!" << std::endl;
				this->name = "";
			}
			else {
				this->name = name;
			}

			bool isArrValid = true; // use a flag to check the array, so we don't duplicate code for making the array null
			if (noPatients < 1) {
				std::cout << "Number of patients cannot be less than 1!" << std::endl;
				isArrValid = false;
			}
			else {
				if (!patientsNames) {
					std::cout << "Patient names array cannot be empty!" << std::endl;
					isArrValid = false;
				}
				else {
					if (!this->isArrayValid(patientsNames, noPatients)) {
						std::cout << "Patients array does not respect restriction!" << std::endl;
						isArrValid = false;
					}
					else {
						this->noPatients = noPatients;
						this->patientsNames = new std::string[this->noPatients];
						for (int i = 0; i < this->noPatients; i++) {
							this->patientsNames[i] = patientsNames[i];
						}
					}
				}
			}

			if (!isArrValid) {
				this->noPatients = 0;
				this->patientsNames = nullptr;
			}

			this->specialty = specialty;
		}

		// copy constructor (no need to validate anything, just ensure it exists)
		Doctor(const Doctor& other) : maxPatients(20) {
			this->name = other.name;
			this->noPatients = other.noPatients;
			if (other.patientsNames) {
				this->patientsNames = new std::string[this->noPatients];
				for (int i = 0; i < this->noPatients; i++) {
					this->patientsNames[i] = other.patientsNames[i];
				}
			}
			else {
				this->patientsNames = nullptr;
			}
			this->specialty = other.specialty;
		}

		// destructor
		~Doctor() {
			if (this->patientsNames) {
				delete[] this->patientsNames;
				this->patientsNames = nullptr;
			}
		}

		std::string* getPatientsNames() const {
			if (!this->patientsNames) {
				return nullptr;
			}
			else {
				std::string* copy = new std::string[this->noPatients];
				for (int i = 0; i < this->noPatients; i++) {
					copy[i] = this->patientsNames[i];
				}
				return copy;
			}
		}

		void setPatientsNames(int noPatients, std::string* patientsNames) {
			if (this->patientsNames) {
				delete[] this->patientsNames;
				this->patientsNames = nullptr;
			}

			if (noPatients < 1) {
				std::cout << "Number of patients cannot be less than 1!" << std::endl;
				this->noPatients = 0;
			}
			else {
				if (!patientsNames) {
					std::cout << "Patient names array cannot be empty!" << std::endl;
					this->noPatients = 0;
				}
				else {
					if (!this->isArrayValid(patientsNames, noPatients)) {
						std::cout << "Patients array does not respect restriction!" << std::endl;
						this->noPatients = 0;
					}
					else {
						this->noPatients = noPatients;
						this->patientsNames = new std::string[this->noPatients];
						for (int i = 0; i < this->noPatients; i++) {
							this->patientsNames[i] = patientsNames[i];
						}
					}
				}
			}
		}

		Doctor& operator/=(std::string value) { // Doctor /= std::string
			// for compound assignment operators, we do modify the current object, and we return exactly it (therefore the reference in the return type)
			// small guard case
			if (!this->patientsNames || value.empty()) {
				return *this;
			}
			// another easy check for a fast return
			// since we check if the value exists, might as well save its position (so we don't duplicate this after)
			int idx = -1;
			for (int i = 0; i < this->noPatients; i++) {
				if (this->patientsNames[i] == value) {
					idx = i;
					break;
				}
			}
			if (idx == -1) { // if not found, return current object
				return *this;
			}

			// if we reached here, it means that the value was found
			// small check whether we only have 1 value, so we ensure we don't allocate an array of size 0
			if (this->noPatients == 1) {
				delete[] this->patientsNames;
				this->patientsNames = nullptr;
				this->noPatients--;
				return *this;
			}

			std::string* newArray = new std::string[this->noPatients - 1];
			for (int i = 0, j = 0; i < this->noPatients; i++) { // use i as the index in our initial array, and j as the index for the new array
				// when we reach the index to remove, we just move on without doing anything
				if (i != idx) {
					newArray[j++] = this->patientsNames[i];
				}
			}
			this->noPatients--;
			delete[] this->patientsNames;
			this->patientsNames = newArray;
			return *this;
		}

		explicit operator std::string() const { // explicit cast to std::string
			if (!this->patientsNames) {
				return "(None)";
			}
			std::string result = "";
			for (int i = 0; i < this->noPatients; i++) {
				result += (i != this->noPatients - 1 ? (this->patientsNames[i] + "-") : this->patientsNames[i]);
			}
			return result;
		}

		void prettyPrint() const {
			std::cout << std::endl << "Name: " << this->name << std::endl;
			std::cout << "Number of patients: " << this->noPatients << std::endl;
			std::cout << "Patients' names: " << (std::string)(*this) << std::endl; // use the cast to string operator to simplify printing
			std::string specialtyString;
			switch (this->specialty) {
				case Specialty::CARDIOLOGIST:
					specialtyString = "CARDIOLOGIST";
					break;
				case Specialty::SURGEON:
					specialtyString = "SURGEON";
					break;
				case Specialty::NEUROLOGIST:
					specialtyString = "NEUROLOGIST";
					break;
				case Specialty::PNEUMOLOGIST:
					specialtyString = "PNEUMOLOGIST";
					break;
				case Specialty::INTERNIST:
					specialtyString = "INTERNIST";
					break;
				case Specialty::GENERAL:
					specialtyString = "GENERAL";
					break;
			}
			std::cout << "Specialty: " << specialtyString << std::endl;
			
		}
};

int main() {
	Doctor a; // default constructor
	std::cout << "Doctor a:";
	a.prettyPrint(); // also testing pretty print and explicit cast to string

	std::string names[] = {"John", "Joanne", "Victor"};
	Doctor b("William", 3, names, Specialty::NEUROLOGIST); // parametrized constructor
	std::cout << "Doctor b:";
	b.prettyPrint();

	Doctor c(a); // copy constructor
	std::cout << "Doctor c:";
	c.prettyPrint();

	Doctor* d = new Doctor;
	std::cout << "Doctor d:";
	d->prettyPrint();
	delete d; // destructor
	d = nullptr;

	c.setPatientsNames(3, names); // setter
	std::string* namesArr = c.getPatientsNames(); // getter
	std::cout << "c patients' names after setter (via getter):" << std::endl;
	for (int i = 0; i < 3; i++) {
		std::cout << namesArr[i] << " ";
	}
	std::cout << std::endl;
	delete[] namesArr; // clean it up after we're done
	namesArr = nullptr;

	b /= "John"; // operator/=
	std::cout << "b after removal of John:";
	b.prettyPrint();

	std::cout << "c on explicit cast to string: " << (std::string)c << std::endl; // explicit cast operator

	return 0;
}