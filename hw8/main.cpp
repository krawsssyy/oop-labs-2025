#include <iostream>
#include <string>

class Doctor {
private:
    std::string name;
    std::string * specialties;
    int specialtyCount;
    int preferredWorkingDay;
    int workingHoursStart;
    int workingHoursEnd;

public:
    Doctor() { // default constructor
        this->name = "";
        this->specialties = nullptr;
        this->specialtyCount = 0;
        this->preferredWorkingDay = 1;
        this->workingHoursStart = 8;
        this->workingHoursEnd = 16;
    }

    Doctor(const std::string& n, const std::string* specs, int count, int day, int startHour, int endHour): 
        specialties(nullptr), workingHoursStart(0), workingHoursEnd(23) { // parametrized constructor
        // since we use setters and do validations on the parameters there with our parameters
        // we have to give them some default values (so as to avoid comparisons with uninitialized memory)
        this->setName(n);
        this->setSpecialties(specs, count); // use setters to avoid code reuse
        this->setPreferredWorkingDay(day);
        this->setWorkingHoursStart(startHour);
        this->setWorkingHoursEnd(endHour);
    }

    Doctor(const Doctor& other) { // copy constructor
        // no need to validate other values, since we have an already constructed object which passed validations
        // this one, being also a constructor, has the class's name
        // it tells the program how we can initialize an object from an already existing one, rather than a list of parameters now
        // the copy ctor is called explicitly when doing stuff like:
        // "Doctor a; Doctor b(a);"
        // or "Doctor b = Doctor(a);" (even though this one uses the assignment operator, since "b" is not already defined
        // it does something called copy-initialization, and is equivalent to "Doctor b(a);", and no assignment is done, just copy)
        // if "b" was already defined, doing "b = a" would result in using the assignment operator, which we haven't learned yet
        // or implicitly, when passing/returning objects as value rather than reference
        // by default, the copy ctor would do a bitwise copy of all of our attributes
        // and therefore would shallow-copy the dynamic arrays
        // hence, it is imperative that we define it when working with dynamic memory 
        // (more on that later, when we learn operators and learn about the rule of three)

        // it being a special function, and operating on objects of our type, the copy constructor can access private attributes
        // without any special designation (more on that later, when we'll learn about friend classes)
        this->name = other.name;
        this->specialtyCount = other.specialtyCount;

        if (this->specialtyCount > 0) {
            this->specialties = new std::string[this->specialtyCount];
            for (int i = 0; i < this->specialtyCount; i++) {
                this->specialties[i] = other.specialties[i];
            }
        }
        else {
            this->specialties = nullptr;
        }

        this->preferredWorkingDay = other.preferredWorkingDay;
        this->workingHoursStart = other.workingHoursStart;
        this->workingHoursEnd = other.workingHoursEnd;
    }

    ~Doctor() {
        delete[] this->specialties;
        this->specialties = nullptr;
    }

    std::string getName() const {
        return this->name;
    }

    std::string* getSpecialties() const {
        if (!this->specialtyCount || !this->specialties) { // ensure existence
            return nullptr;
        }

        std::string* result = new std::string[this->specialtyCount]; // deep-copy
        for (int i = 0; i < this->specialtyCount; i++) {
            result[i] = this->specialties[i];
        }

        return result; // beware: since we return a dynamic array here, the caller has the responsibility of cleaning it up
    }

    int getSpecialtyCount() const {
        return this->specialtyCount;
    }

    int getPreferredWorkingDay() const {
        return this->preferredWorkingDay;
    }

    int getWorkingHoursStart() const {
        return this->workingHoursStart;
    }

    int getWorkingHoursEnd() const {
        return this->workingHoursEnd;
    }

    void setName(const std::string& n) {
        if (n.length() >= 3) {
            this->name = n;
        }
        else {
            std::cout << "Name length cannot be less than 3!" << std::endl;
            this->name = ""; // give a default value
        }
        
    }

    void setSpecialties(const std::string* specs, int count) {
        if (this->specialties) { // clear before re-allocation
            delete[] this->specialties;
        }

        if (count > 0 && specs != nullptr) {
            this->specialtyCount = count;
            this->specialties = new std::string[count]; // since strings are a special container, we cannot use memcpy directly
            // because we cannot determine the size with simple calculations involving sizeof
            for (int i = 0; i < count; i++) {
                this->specialties[i] = specs[i];
            }
        }
        else {
            std::cout << "Count cannot be less than 1 or the array cannot be null!" << std::endl;
            this->specialtyCount = 0;
            this->specialties = nullptr;
        }
    }

    void setPreferredWorkingDay(int d) {
        if (d < 1) {
            std::cout << "Day cannot be less than 1 (Monday)!" << std::endl;
            d = 1;
        }
        if (d > 7) {
            std::cout << "Day cannot be bigger than 7 (Sunday)!" << std::endl;
            d = 7;
        }
        this->preferredWorkingDay = d;
    }

    void setWorkingHoursStart(int h) {
        if (h < 0) {
            std::cout << "Hour cannot be less than 0!" << std::endl;
            h = 0;
        }
        if (h > 23) {
            std::cout << "Hour cannot be bigger than 23!" << std::endl;
            h = 23;
        }
        if (h >= this->workingHoursEnd) {
            h = this->workingHoursEnd - 1;
            std::cout << "Start hour cannot be bigger than or equal to the end hour!" << std::endl;
        }
        this->workingHoursStart = h;
    }

    void setWorkingHoursEnd(int h) {
        if (h < 0) {
            std::cout << "Hour cannot be less than 0!" << std::endl;
            h = 0;
        }
        if (h > 23) {
            std::cout << "Hour cannot be bigger than 23!" << std::endl;
            h = 23;
        }
        if (h <= this->workingHoursStart) {
            h = this->workingHoursStart + 1;
            std::cout << "End hour cannot be smaller than or equal to the start hour!" << std::endl;
        }
        this->workingHoursEnd = h;
    }

    bool hasSpecialty(const std::string& s) const {
        for (int i = 0; i < this->specialtyCount; i++) {
            if (this->specialties[i] == s) {
                return true;
            }
        }
        return false;
    }

    bool worksAt(int day, int hour) const {
        return day == this->preferredWorkingDay && hour >= this->workingHoursStart && hour <= this->workingHoursEnd;
    }

    void print() const {
        std::cout << "Doctor: " << this->name << std::endl;
        std::cout << "Specialties: ";
        if (this->specialties) {
            for (int i = 0; i < this->specialtyCount; i++) {
                std::cout << this->specialties[i];
                if (i < this->specialtyCount - 1) {
                    std::cout << ", ";
                }
            }
        }
        else {
            std::cout << "(None)";
        }
        std::cout << std::endl << "Preferred day: " << this->preferredWorkingDay << std::endl;
        std::cout << "Working hours: " << this->workingHoursStart << "-" << this->workingHoursEnd << std::endl;
    }
};

int main() {
    std::string* cardio = new std::string[2]; // use dynamic arrays
    // since our class uses dynamic arrays, we have to also use dynamic arrays to initialize it
    // had we used static arrays, when an object would be destroyed, the program would attempt to delete static memory
    // resulting in a crash
    cardio[0] = "Cardiology";
    cardio[1] = "Intensivist";

    std::string* derm = new std::string[1];
    derm[0] = "Dermatology";

    std::string* neuro = new std::string[3];
    neuro[0] = "Neurology";
    neuro[1] = "Psychiatry";
    neuro[2] = "Neurosurgery";

    Doctor doctors[10] = { // declare our doctors array
        Doctor("John Smith", cardio, 2, 2, 8, 16),
        Doctor("Donna Noble", derm, 1, 3, 10, 18),
        Doctor("Rose Taylor", neuro, 2, 4, 9, 17),
        Doctor("Sarah Jane Smith", derm, 1, 1, 7, 15),
        Doctor("Kate Stewart", cardio, 2, 5, 6, 14),
        // use copy constructor to initialize new objects
        Doctor(doctors[0]),
        Doctor(doctors[1]),
        Doctor(doctors[2]),
        Doctor(doctors[3]),
        Doctor(doctors[4]),
    };
    // using doctors[5] = Doctor(doctors[0]); here instead of defining it in the array, 
    // would result in using the assignment operator (since doctors[5] would be already initialized with the default constructor)
    // which would create a shallow-copy of our object, which we do not want
    doctors[5].setName("John Smith Jr.");
    doctors[6].setName("Donna Noble Jr.");
    doctors[7].setName("Rose Taylor Jr.");
    doctors[8].setName("Sarah Jane Smith Jr.");
    doctors[9].setName("Kate Stewart Jr.");

    // cleanup for our dynamic arrays
    // safe since copies have been already made
    delete[] cardio;
    cardio = nullptr;
    delete[] derm;
    derm = nullptr;
    delete[] neuro;
    neuro = nullptr;

    while (true) {
        std::cout << "1. List all doctors" << std::endl;
        std::cout << "2. Schedule appointment" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Choose option: ";

        int option;
        std::cin >> option;

        if (option == 1) {
            for (int i = 0; i < 10; i++) {
                doctors[i].print();
            }
        }
        else if (option == 2) {
            std::string spec;
            int day;
            int hour;
            std::cout << "Specialty: ";
            std::cin >> spec; // since specialities are 1 word, it is safe to use std::cin rather than getline
            std::cout << "Day (1-7): ";
            std::cin >> day;
            std::cout << "Hour (0-23): ";
            std::cin >> hour;

            bool found = false;

            for (int i = 0; i < 10; i++) {
                if (doctors[i].hasSpecialty(spec) && doctors[i].worksAt(day, hour)) {
                    std::cout << "Appointment scheduled with: " << std::endl;
                    doctors[i].print();
                    found = true;
                    break;
                }
            }

            if (!found) {
                std::cout << "No doctor available!" << std::endl;
            }
        }
        else if (option == 3) {
            std::cout << "Goodbye!" << std::endl;
            break;
        }
        else {
            std::cout << "Invalid option!" << std::endl;
        }
    }

    return 0;
}
