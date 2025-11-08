#include <iostream> // for std::cout, std::cin
#include <string> // for std::string, std::getline
#include <limits>
#include <cctype> // for toupper

enum class VehicleType { SEDAN, COUPE, HATCHBACK, MINIVAN, CONVERTIBLE, SUV, PICKUP, UNKNOWN }; // use the safer enum-class
// it doesn't automatically convert to int and requires explicit usage via the enum identifier, avoiding accidents

struct Pair { // helper pair for converting VehicleType to string and vice-versa
    // we will create a list of such objects and use them to map strings to vehicle types
    std::string first;
    VehicleType second;
};

class Vehicle {
    private:
        std::string make;
        std::string model;
        VehicleType vehicleType;
        int ID;
        int year;
        static int idCounter;
        static Pair changeMap[];
        static int mapItems; // keeps the length of the enum
        // not ideal solution, but until we learn some more advanced methods (such as reading from file and deducing the length from there), we go with hardcoding
    public:
        Vehicle() : make(""), model(""), vehicleType(VehicleType::UNKNOWN), ID(0), year(0) { }
        // increment the ID only for properly constructed elements
        // use enum identifier to refer to elements of the enum ("VehicleType::")

        Vehicle(const std::string& make, const std::string& model, const VehicleType vehicleType, const int year) :
            make(""), model(""), vehicleType(VehicleType::UNKNOWN), year(0), ID(idCounter++) { // we give default values here and use setters to validate
            // if the validation fails, we get to keep the default values and have a correct object
            // use const + "&" for complex params such that no copy for them is made and we are not able to modify them inside of our function
            // for small/trivial params, there's no need to use that combo (or any of the elements of it) - since by not using "&" a copy is made, and we can modify it however we want
            // however, it is good practice to leave those at least as const, such that any accidental modifications prior to using them in our attributes result in compilation errors
            this->setMake(make);
            this->setModel(model);
            this->setVehicleType(vehicleType);
            this->setYear(year);
        }

        // no destructor since no dynamic fields

        // for these simple values, ideally we'd return them as const, but it doesn't make any sense, so we leave them just as they are
        std::string getMake() const { // we wouldn't want to use const + "&" here, since somebody could then cast away the const and modify our internal buffers
            // so we just return a copy of the string
            return this->make;
        }

        std::string getModel() const {
            return this->model;
        }

        VehicleType getVehicleType() const {
            return this->vehicleType;
        }

        int getYear() const {
            return this->year;
        }

        int getID() const {
            return this->ID;
        }

        void setMake(const std::string& make) {
            if (!make.empty()) {
                this->make = make;
            }
            else {
                std::cout << std::endl << "Make cannot be empty!" << std::endl;
                // leave it unchanged if fail
            }
        }

        void setModel(const std::string& model) {
            if (!model.empty()) {
                this->model = model; // this actually copies the string, rather than directly moving the string from the outside into our class
                // so it is safe to make const + "&" for containers which implement copy constructor + assignment op (which copies), such as std::string
            }
            else {
                std::cout << std::endl << "Model cannot be empty!" << std::endl;
            }
        }

        void setVehicleType(const VehicleType vehicleType) {
            this->vehicleType = vehicleType;
        }

        void setYear(int year) {
            if (year > -1 && year < 2026) {
                this->year = year;
            }
            else {
                std::cout << std::endl << "Year cannot be negative or bigger than the current year!" << std::endl;
            }
        }

        static VehicleType stringToVehicleType(std::string vehicleTypeStr) {
            for (auto& c : vehicleTypeStr) { // for each - only works for containers, such as std::string and STL ones (which we'll see in the future)
                // SYNTAX: for (dataType variableToIterateWith : container)
                // auto keyword determines automatically the type of variable
                // beware, the auto keyword is to be used only in such constructs (i.e. when working with iterators over various containers)
                // we use & to make it use references to actual characters from the string, such that modifications will be reflected in the string itself
                c = (char)std::toupper((unsigned char)c); // capitalize each letter from our string, normalizing it to uppercase for match
                // casts are a bit odd, but toupper requires and returns an int (either EOF or unsigned char value), and then we iterate over the string with the "char" type
                // so all we do is ensure the correct types are used
                // allows users to type in whatever case they want for the type and for it to still be correct
            }
            for (int i = 0; i < Vehicle::mapItems; i++) {
                if (Vehicle::changeMap[i].first == vehicleTypeStr) {
                    return Vehicle::changeMap[i].second;
                }
            }
            return VehicleType::UNKNOWN; // return unknown if nothing was found
        }

        static std::string vehicleTypeToString(const VehicleType vehicleType) {
            for (int i = 0; i < Vehicle::mapItems; i++) {
                if (Vehicle::changeMap[i].second == vehicleType) {
                    return Vehicle::changeMap[i].first;
                }
            }
            return "UNKNOWN"; // return unknown if nothing was found
        }
};

int Vehicle::idCounter = 1; // 1-index
Pair Vehicle::changeMap[] = { // create a list of key-value pairs (a map - analogous to dictionaries in Python), useful for mapping strings to vehicle types and vice-versa
            { "SEDAN", VehicleType::SEDAN },
            { "COUPE", VehicleType::COUPE },
            { "HATCHBACK", VehicleType::HATCHBACK },
            { "MINIVAN", VehicleType::MINIVAN },
            { "CONVERTIBLE", VehicleType::CONVERTIBLE },
            { "SUV", VehicleType::SUV },
            { "PICKUP", VehicleType::PICKUP },
            { "UNKNOWN", VehicleType::UNKNOWN },
};
int Vehicle::mapItems = sizeof(Vehicle::changeMap) / sizeof(Vehicle::changeMap[0]); // works since changeMap is a static array (it won't work for dynamic ones)

class Garage {
    private:
        Vehicle* vehicles = nullptr;
        int size = 0; // current size of the list (i.e. index + 1 of last element)
        int capacity = 0; // capacity (max amount of elements it can hold) of the list

        void resize() { // keep the resizing function private, as we don't want it accessible from the outside
            int newCapacity = this->capacity + 100;
            Vehicle* newVehicles = new Vehicle[newCapacity]; // calls default constructor, but no ID changes
            // default constructor uses placeholder ID, and we then copy-assign to preserve original IDs

            // since we use more complex types in our class such as std::string, which has variable length and is a complex class, we can't use memcpy safely
            for (int i = 0; i < this->size; i++) {
                newVehicles[i] = vehicles[i]; // this works now since we have only simple types it needs to assign/copy (and std::string implements the required functions - take this for granted for now)
                // these are compiler generated and in cases where trivial data types are used and/or types which already implement and assignment operator & copy constructor they work just fine
                // if we had more custom data types/dynamic arrays, we would need to implement our own assignment operator and copy constructor, but in this case we are fine with the default ones
                // this way, it will also copy the IDs
            }

            delete[] this->vehicles; // cleanup our original array
            // safe, since we already moved all elements to another array
            this->vehicles = newVehicles; // assign our member array to the newly created array with a bigger capacity
            this->capacity = newCapacity; // reflect the change in capacity
        }

        int indexOfID(const int ID) const { // helper for getting the correct index for the ID
            for (int i = 0; i < this->size; ++i) {
                if (this->vehicles[i].getID() == ID)  {
                    return i;
                }
            }
            return -1;
        }

    public:
        // since we initialized the variables where we defined them in the "private" field, we don't need a default constructor anymore
        // those initializations act as the default constructor
        // this works since we don't have any complex initializations to make
        // had we the need for any such initializations, we would've needed a default constructor
        
        // note: since this is a class where only one instances is expected to be created, and we don't save the state
        // we don't need a parametrized constructor

        ~Garage() {
            if (this->vehicles) {
                delete[] this->vehicles;
                this->vehicles = nullptr;
            }
        }

        void addVehicle(const std::string& make, const std::string& model, const std::string& vehicleTypeStr, const int year) {
            if (this->size == this->capacity) { // if we are at max capacity
                this->resize();
            }
            this->vehicles[this->size++] = Vehicle(make, model, Vehicle::stringToVehicleType(vehicleTypeStr), year);
        }

        bool readVehicle(const int ID) const {
            int idx = indexOfID(ID);
            if (idx == -1) {
                std::cout << std::endl << "Invalid ID given for read!" << std::endl;
                return false;
            }
            Garage::printVehicle(this->vehicles[idx]);
            return true; // return whether the command succeeded or not
        }

        void readAllVehicles() const {
            if (this->size == 0) {
                std::cout << std::endl << "No vehicles yet!" << std::endl;
                return;
            }
            for (int i = 0; i < this->size; i++) {
                Garage::printVehicle(this->vehicles[i]);
            }
        }

        bool updateVehicle(const int ID, const std::string& make, const std::string& model, const std::string& vehicleTypeStr, const int year) {
            int idx = indexOfID(ID);
            if (idx == -1) {
                std::cout << std::endl << "Invalid ID given for update!" << std::endl;
                return false;
            }
            this->vehicles[idx].setMake(make);
            this->vehicles[idx].setModel(model);
            this->vehicles[idx].setYear(year);
            this->vehicles[idx].setVehicleType(Vehicle::stringToVehicleType(vehicleTypeStr));
            return true;
        }

        bool deleteVehicle(const int ID) {
            int idx = indexOfID(ID);
            if (idx == -1) {
                std::cout << std::endl << "Invalid ID given for delete!" << std::endl;
                return false;
            } // reset it to default values, but still keep it in the list for future usage
            for (int i = idx + 1; i < this->size; i++) {
                this->vehicles[i - 1] = this->vehicles[i]; // essentially remove the element in the list
                // by shifting everything to the right of it 1 position to the left
            }
            this->vehicles[--this->size] = Vehicle(); // reflect change in size and clear the last slot
            return true;
        }

        static void printVehicle(const Vehicle& vehicle) { // helper for printing a vehicle
            std::cout << std::endl << "Vehicle ID: " << vehicle.getID() << std::endl;
            std::cout << "Make: " << vehicle.getMake() << std::endl;
            std::cout << "Model: " << vehicle.getModel() << std::endl;
            std::cout << "Year: " << vehicle.getYear() << std::endl;
            std::cout << "Vehicle type: " << Vehicle::vehicleTypeToString(vehicle.getVehicleType()) << std::endl;
        }
};

void printMenu() { // helper for printing the menu
    std::cout << std::endl << "1. Add a vehicle" << std::endl;
    std::cout << "2. Show vehicle by ID" << std::endl;
    std::cout << "3. Show all vehicles" << std::endl;
    std::cout << "4. Update vehicle" << std::endl;
    std::cout << "5. Delete vehicle" << std::endl;
    std::cout << "6. Exit" << std::endl;
    std::cout << "Enter a choice: ";
}

int main()
{   
    Garage* garage = new Garage; // default construct our garage for use
    // make it a ptr to spice things up a little
    int choice;
    while (true) {
        printMenu();
        if (!(std::cin >> choice)) { // ensure read succeeds (i.e. we read a number and not a character or a string of characters)
            std::cin.clear(); // clear input buffer
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // remove lingering "\n" that might interfere with next inputs
            std::cout << "Invalid input! Please enter a number." << std::endl;
            continue; // retry
        }
        if (choice == 1) { // add a vehicle
            std::string make, model, type;
            int year;
            std::cout << std::endl << "Enter vehicle's make: ";
            std::getline(std::cin >> std::ws, make);
            std::cout << "Enter vehicle's model: ";
            std::getline(std::cin >> std::ws, model);
            std::cout << "Enter vehicle's type (SEDAN/COUPE/HATCHBACK/MINIVAN/CONVERTIBLE/SUV/PICKUP/UNKNOWN): ";
            std::getline(std::cin >> std::ws, type);
            std::cout << "Enter vehicle's year: ";
            if (!(std::cin >> year)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << std::endl << "Invalid year!" << std::endl;
                continue;
            }
            garage->addVehicle(make, model, type, year);
            std::cout << std::endl << "Vehicle added successfully!" << std::endl;
        }
        else if (choice == 2) { // read a vehicle by id
            int ID;
            std::cout << std::endl << "Enter ID for vehicle to show: ";
            if (!(std::cin >> ID)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << std::endl << "Invalid ID!" << std::endl;
                continue;
            }
            bool res = garage->readVehicle(ID);
            if (res) {
                std::cout << std::endl << "Vehicle read successfully!" << std::endl;
            }
        }
        else if (choice == 3) { // list all vehicles
            std::cout << std::endl << "Listing all vehicles..." << std::endl;
            garage->readAllVehicles();
        }
        else if (choice == 4) {  // update a vehicle
            int ID;
            std::cout << std::endl << "Enter ID for vehicle to update: ";
            if (!(std::cin >> ID)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << std::endl << "Invalid ID!" << std::endl;
                continue;
            }
            std::string make, model, type;
            int year;
            std::cout << "Enter vehicle's new make: ";
            std::getline(std::cin >> std::ws, make);
            std::cout << "Enter vehicle's new model: ";
            std::getline(std::cin >> std::ws, model);
            std::cout << "Enter vehicle's new type (SEDAN/COUPE/HATCHBACK/MINIVAN/CONVERTIBLE/SUV/PICKUP/UNKNOWN): ";
            std::getline(std::cin >> std::ws, type);
            std::cout << "Enter vehicle's new year: ";
            if (!(std::cin >> year)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << std::endl << "Invalid year!" << std::endl;
                continue;
            }
            bool res = garage->updateVehicle(ID, make, model, type, year);
            if (res) {
                std::cout << std::endl << "Vehicle updated successfully!" << std::endl;
            }
        }
        else if (choice == 5) {  // delete a vehicle
            int ID;
            std::cout << std::endl << "Enter ID for vehicle to delete: ";
            if (!(std::cin >> ID)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << std::endl << "Invalid ID!" << std::endl;
                continue;
            }
            bool res = garage->deleteVehicle(ID);
            if (res) {
                std::cout << "Vehicle deleted successfully!" << std::endl;
            }
        }
        else if (choice == 6) {
            std::cout << std::endl << "Goodbye.";
            break;
        }
        else {
            std::cout << std::endl << "Invalid choice! Please try again." << std::endl;
        }
    }

    delete garage; // cleanup
    garage = nullptr;
    return 0;
}