#include <iostream> // for std::cout, std::cin, std::endl
#include <string> // for std::string
#include <cstring> // for memcpy, memcmp

class SensorBuffer {
    private:
        std::string name;
        int scale;
        int* data;
        int size;

    public:
        SensorBuffer() { // default constructor
            this->name = "(unnamed)";
            this->scale = 0;
            this->size = 0;
            this->data = nullptr;
        }

        SensorBuffer(const std::string& name, int scale, int size) { // parametrized constructor
            this->name = name;
            this->scale = scale;
            this->size = size;
            if (this->size) {
                this->data = new int[this->size]; // just allocate it, no data
                // we use the setter for setting values
            }
            else {
                this->data = nullptr;
            }
        }

        SensorBuffer(const SensorBuffer& other) { // copy constructor
            this->name = other.name;
            this->scale = other.scale;
            this->size = other.size;
            if (this->size && other.data) { // ensure size is correct and other array is allocated correctly
                this->data = new int[this->size];
                memcpy(this->data, other.data, this->size * sizeof(int));
                // memcpy works in this case since the dynamic array is a simple type (int)
                // any trivial data types (float, char, double, int, long) are ok for memcpy
                // classes and structs with no special containers (std::string) or dynamic fields are also ok
                // but if we have an array of std::string or other STL containers or classes/structs with dynamic/special attributes
                // we cannot use memcpy reliably because we cannot compute the correct size using sizeof and it might mess up alignments inside special containers' attributes
            }
            else {
                this->data = nullptr;
            }
        }

        ~SensorBuffer() { // destructor
            if (this->data != nullptr) {
                delete[] this->data;
                this->data = nullptr;
            }
        }

        // operators


        // assignment operator (=)
        // used for assigning a new value to an ALREADY EXISTENT object

        // small recap, copy-assignment (uses operator=) vs copy-initialization (uses copy constructor)

        // copy-initialization - used only when initializaing an object in the same statement where it is defined (the object DOES NOT EXIST before that)
        // even though we use "=", there's no assigment being used, and only the copy constructor is used

        // SensorBuffer A;
        // SensorBuffer B = SensorBuffer();
        // SensorBuffer C = SensorBuffer(...); // copy-init
        // SensorBuffer D = SensorBuffer(C); // copy-init
        // SensorBuffer E = D; // copy init

        // copy-assigment - used only when attributing a different object to an already existing object
        // it uses both the copy constructor (for constructing a copy of the element in the right hand side), and the assignment operator
        // for giving the object on the left hand side the required value
        // in the context of the operator, the object receiving the new value is "this", and the object from which we take the value is the parameter of the operator

        // SensorBuffer A, B;
        // A = B; // copy-assign
        // A = SensorBuffer(); // copy-assign
        // A = SensorBuffer(...); // copy-assign
        // A = SensorBuffer(B); // copy-assign
    

        // SYNTAX: ClassName& operator=(const ClassName& other) { ... }
        // assignment operators (and compound assignment operators (for e.g. +=, -=, ...)) return a reference to the class
        // and modify this (and returns it)
        SensorBuffer& operator=(const SensorBuffer& other) {
            // it is basically a copy constructor, without the guards that we have not initialized attributes, and therefore we need to check them
            // before assigning them new values
            if (this != &other) { // we guard against self-assignment (A = A)
                this->name = other.name;
                this->scale = other.scale;
                if (this->data != nullptr) { // ensure pointer is valid before deletion
                    delete[] this->data;
                }
                this->size = other.size;
                if (this->size && other.data) { // ensure size is valid before allocation and the data array is correctly allocated
                    this->data = new int[this->size];
                    memcpy(this->data, other.data, this->size * sizeof(int));
                }
                else {
                    this->data = nullptr;
                }
            }
            return *this;
        }
        // NOTE: assignment operator can also return void, but that breaks several things, such as:
        // 1. Chaining operators - A = B = C => this is evaluated as A = (B = C), but since B = C doesn't return anything, it breaks the chain
        // 2. Checking the result of an assignment - for e.g., consider reading objects in a loop until you reach an end object
        // while((A = read_object()) != END_OBJECT) { ... } => since the assignment doesn't return anything, we can't use that for comparison
        // 3. If we have a simple class and want to mark the default implementation of the operator as safe to use, we can do
        // SensorBuffer& operator=(const SensorBuffer& other) = default;
        // but with the void return type, we cannot do that since that is not recognized as the official return type of the operator
        // 4. You cannot use it as the underlying type in STL containers (since they require a valid operator= implementation)

        // compound assignment operators (+=, -=, *=, /=, &=, |=, ^=)
        // SYNTAX: ClassName& operatorXX(const AnyType& other) { ... }
        // for the compound assignment operators, we can either define them for objects of the same type, other objects or simple data types
        // since compound assignment expands to (for e.g., for +=) A = A + B
        // we can have that other element be any type
        // however, beware, since we haven't talked about "friend", we can only define OurObject + OtherObject, which is not commutative
        // therefore, we cannot yet do OtherObject + OurObject

        // for those, we don't need to guard against self-assignment, since A += A is valid and produces a different result
        // example with += AnotherObjectOfTheSameType;
        SensorBuffer& operator+=(const SensorBuffer& other) {
            if (!this->size || !this->data || !other.size || !other.data) { // guard against nullptr accesses
                return *this;
            }
            int minSize = this->size < other.size ? this->size : other.size; // determine min size from both arrays
            // such that we don't overflow when reading/writing into any array
            // but that may leave some values unchanged (it is ok for now), but for other cases
            // we can truncate both arrays to the same size
            for (int i = 0; i < minSize; i++) {
                this->data[i] += other.data[i];
            }
            return *this;
        }

        // example with += AnIntValue;
        SensorBuffer& operator+=(int value) {
            if (!this->size || !this->data) {
                return *this;
            }
            for (int i = 0; i < this->size; i++) {
                this->data[i] += value;
            }
            return *this;
        }

        SensorBuffer& operator-=(const SensorBuffer& other) {
            if (!this->size || !this->data || !other.size || !other.data) {
                return *this;
            }
            int minSize = this->size < other.size ? this->size : other.size;
            for (int i = 0; i < minSize; i++) {
                this->data[i] -= other.data[i];
            }
            return *this;
        }

        SensorBuffer& operator-=(int value) {
            if (!this->size || !this->data) {
                return *this;
            }
            for (int i = 0; i < this->size; i++) {
                this->data[i] -= value;
            }
            return *this;
        }

        SensorBuffer& operator*=(const SensorBuffer& other) {
            if (!this->size || !this->data || !other.size || !other.data) {
                return *this;
            }
            int minSize = this->size < other.size ? this->size : other.size;
            for (int i = 0; i < minSize; i++) {
                this->data[i] *= other.data[i];
            }
            return *this;
        }

        SensorBuffer& operator*=(int value) {
            if (!this->size || !this->data) {
                return *this;
            }
            for (int i = 0; i < this->size; i++) {
                this->data[i] *= value;
            }
            return *this;
        }

        SensorBuffer& operator/=(const SensorBuffer& other) {
            if (!this->size || !this->data || !other.size || !other.data) {
                return *this;
            }
            int minSize = this->size < other.size ? this->size : other.size;
            for (int i = 0; i < minSize; i++) {
                if (other.data[i]) { // guard against division by 0
                    this->data[i] /= other.data[i];
                }
            }
            return *this;
        }

        SensorBuffer& operator/=(int value) {
            if (!this->size || !this->data) {
                return *this;
            }
            if (!value) { // guard against division by 0
                return *this;
            }
            for (int i = 0; i < this->size; i++) {
                this->data[i] /= value;
            }
            return *this;
        }

        // binary operators (+, -, *, /, & (binary and), | (binary or), ^ (xor - exclusive or))
        // as mentioned for the compound assignment operators, we can only do now OurObject + OtherVariable, and not OtherVariable + OurObject
        // since that requires overloading a global operator, and we require the use of the "friend" keyword which we haven't learned (we'll do it next seminary)

        // SYNTAX: ClassName operatorX(const VariableType& other) const { ... }
        // binary operators don't modify this (hence the const keyword after the parameter list)
        // and return a new object, with the modified value

        // in this case, when doing A operator B, "this" is A, and the parameter is "B"

        // generally, we can either define first the binary operators, and then use them for defining the compound assignment ones
        // (using the required operator + operator=)
        // or we first define the compound assignment operators and then use them for defining the binary operators, as such:

        // define A + AnotherObjectOfTheSameType
        SensorBuffer operator+(const SensorBuffer& other) const {
            SensorBuffer temp(*this); // make a copy from this (current object (first one in the operation - A))
            temp += other; // use +=, which returns a new value and that is saved in temp, and we return the modified object
            return temp;
        }

        // define A + AnIntValue
        SensorBuffer operator+(int value) const {
            SensorBuffer temp(*this);
            temp += value;
            return temp;
        }

        SensorBuffer operator-(const SensorBuffer& other) const {
            SensorBuffer temp(*this);
            temp -= other;
            return temp;
        }

        SensorBuffer operator-(int value) const {
            SensorBuffer temp(*this);
            temp -= value;
            return temp;
        }

        SensorBuffer operator*(const SensorBuffer& other) const {
            SensorBuffer temp(*this);
            temp *= other;
            return temp;
        }

        SensorBuffer operator*(int value) const {
            SensorBuffer temp(*this);
            temp *= value;
            return temp;
        }

        SensorBuffer operator/(const SensorBuffer& other) const {
            SensorBuffer temp(*this);
            temp /= other;
            return temp;
        }

        SensorBuffer operator/(int value) const {
            SensorBuffer temp(*this);
            temp /= value;
            return temp;
        }

        SensorBuffer operator&(const SensorBuffer& other) const {
            if (!this->size || !this->data || !other.size || !other.data) {
                return *this;
            }
            SensorBuffer temp(*this);
            int minSize = this->size < other.size ? this->size : other.size;
            for (int i = 0; i < minSize; i++) {
                temp.data[i] = this->data[i] & other.data[i];
            }
            return temp;
        }

        SensorBuffer operator&(int value) const {
            if (!this->size || !this->data) {
                return *this;
            }
            SensorBuffer temp(*this);
            for (int i = 0; i < this->size; i++) {
                temp.data[i] = this->data[i] & value;
            }
            return temp;
        }

        SensorBuffer operator|(const SensorBuffer& other) const {
            if (!this->size || !this->data || !other.size || !other.data) {
                return *this;
            }
            SensorBuffer temp(*this);
            int minSize = this->size < other.size ? this->size : other.size;
            for (int i = 0; i < minSize; i++) {
                temp.data[i] = this->data[i] | other.data[i];
            }
            return temp;
        }

        SensorBuffer operator|(int value) const {
            if (!this->size || !this->data) {
                return *this;
            }
            SensorBuffer temp(*this);
            for (int i = 0; i < this->size; i++) {
                temp.data[i] = this->data[i] | value;
            }
            return temp;
        }

        SensorBuffer operator^(const SensorBuffer& other) const {
            if (!this->size || !this->data || !other.size || !other.data) {
                return *this;
            }
            SensorBuffer temp(*this);
            int minSize = this->size < other.size ? this->size : other.size;
            for (int i = 0; i < minSize; i++) {
                temp.data[i] = this->data[i] ^ other.data[i];
            }
            return temp;
        }

        SensorBuffer operator^(int value) const {
            if (!this->size || !this->data) {
                return *this;
            }
            SensorBuffer temp(*this);
            for (int i = 0; i < this->size; i++) {
                temp.data[i] = this->data[i] ^ value;
            }
            return temp;
        }

        // comparison operators (==, !=, <, <=, >, >=)
        // they are used for comparing objects to one another (again, the parameter is not necessary to be the type of the object, and can be anything)

        // SYNTAX: bool operatorXX(const AnotherVariableType& other) const { ... }
        // this doesn't modify the current object (this) at all, and therefore the "const" after the paramter list
        // as in the other cases, if we have A operator B, "this" is A, and the parameter is "B"

        // generally, we can define ==, < and >, and then define the rest using these ones (if the desired behaviour matches that)
    
        // example with comparing against an object of the same type (A == B)
        bool operator==(const SensorBuffer& other) const {
            // we need to do some prior checks before validating the memory
            if (this->size == 0 && other.size == 0) { // comparing 0-size buffers, since memcmp on nullptr is undefined behaviour
                return this->name == other.name && this->scale == other.scale;
            }
            if (this->size != other.size) { // fast-fail if sizes differ
                return false;
            }
            return this->name == other.name &&
                this->scale == other.scale &&
                memcmp(this->data, other.data, this->size * sizeof(int)) == 0;
            // memcmp is a special function which compares 2 memory buffers, byte-by-byte, until one differs
            // beware, as memcmp can also be used in the same simple cases as memcpy, because of the same size issues
            // SYNTAX: memcmp (buffer_1, buffer_2, size_in_bytes)
            // RETURN values
            // >0 if the first byte that does not match has a greater value in buffer_1 than in buffer_2
            // 0 if both buffers match (up to the provided size_in_bytes)
            // <0 if the first byte that does not match has a lower value in buffer_1 than in buffer_2
        }

        // example with comparing against an integer ( A == 10 )
        // returns true if all elements are the given int value
        bool operator==(const int& other) const {
            if (!this->size || !this->data) {
                return false;
            }
            for (int i = 0; i < this->size; i++) {
                if (this->data[i] != other) {
                    return false;
                }
            }
            return true;
        }

        bool operator!=(const SensorBuffer& other) const {
            return !(*this == other);
        }

        bool operator<(const SensorBuffer& other) const {
            return this->average() < other.average();
        }

        bool operator<=(const SensorBuffer& other) const {
            return this->average() <= other.average();
        }

        bool operator>(const SensorBuffer& other) const {
            return this->average() > other.average();
        }

        bool operator>=(const SensorBuffer& other) const {
            return this->average() >= other.average();
        }

        bool operator!() const {
            if (!this->size || !this->data) {
                return true; // empty/uninitialized buffer evaluates to false, so !buffer is true
            }
            for (int i = 0; i < this->size; i++) {
                if (this->data[i]) { // if any element is non-zero, buffer is "truthy"
                    return false; // so !buffer is false
                }
            }
            return true; // all elements are zero, buffer is "falsy", so !buffer is true
        }

        double average() const { // helper function
            if (!this->size || !this->data) {
                return 0.0; // guard against final division by 0
            }
            double sum = 0.0;
            for (int i = 0; i < this->size; i++) {
                sum += this->data[i];
            }
            return sum / this->size;
        }

        void print() const { // helper function
            std::cout << "Name: " << this->name << std::endl;
            std::cout << "Scale: " << this->scale << std::endl;
            std::cout << "Size: " << this->size << std::endl;
            std::cout << "Data: ";
            if (!this->size || !this->data) {
                std::cout << "(None)";
            }
            else {
                for (int i = 0; i < this->size; i++) {
                    std::cout << this->data[i] << " ";
                }
            }
            std::cout << std::endl;
        }

        void setValue(int index, int value) // helper for setting values in the dynamic array, so we don't have to define any dynamic arrays in main when testing
        {
            if (index >= 0 && index < this->size)
            {
                this->data[index] = value;
            }
        }
};

int main()
{
    // setup
    std::string nameA = "TestA";
    std::string nameB = "TestB";
    SensorBuffer a(nameA, 1, 5);
    SensorBuffer b(nameB, 1, 5);
    SensorBuffer c, d;

    a.setValue(0, 10);
    a.setValue(1, 20);
    a.setValue(2, 30);
    a.setValue(3, 40);
    a.setValue(4, 50);

    b.setValue(0, 1);
    b.setValue(1, 2);
    b.setValue(2, 3);
    b.setValue(3, 4);
    b.setValue(4, 5);

    std::cout << "Sensor A:" << std::endl;
    a.print();
    std::cout << std::endl;

    std::cout << "Sensor B:" << std::endl;
    b.print();
    std::cout << std::endl;

    std::cout << "Sensor C:" << std::endl;
    c.print();
    std::cout << std::endl;

    std::cout << "Sensor D:" << std::endl;
    d.print();
    std::cout << std::endl;

    // assignment and compound assignment operators

    c = b;
    std::cout << "C = B: " << std::endl;
    c.print();
    std::cout << std::endl;

    d = b; // added so D is not empty
    d += b;
    std::cout << "D += B: " << std::endl;
    d.print();
    std::cout << std::endl;

    c *= b;
    std::cout << "C *= B: " << std::endl;
    c.print();
    std::cout << std::endl;

    d /= b;
    std::cout << "D /= B: " << std::endl;
    d.print();
    std::cout << std::endl;

    d *= 5;
    std::cout << "D *= 5: " << std::endl;
    d.print();
    std::cout << std::endl;

    // binary operators

    SensorBuffer e = a + b;
    std::cout << "A + B:" << std::endl;
    e.print();
    std::cout << std::endl;

    SensorBuffer f = a - 10;
    std::cout << "A - 10:" << std::endl;
    f.print();
    std::cout << std::endl;

    SensorBuffer g = a * b;
    std::cout << "A * B:" << std::endl;
    g.print();
    std::cout << std::endl;

    SensorBuffer h = a / 10;
    std::cout << "A / 10:" << std::endl;
    h.print();
    std::cout << std::endl;

    SensorBuffer i = a & b;
    std::cout << "A & B:" << std::endl;
    i.print();
    std::cout << std::endl;

    SensorBuffer j = a | 3;
    std::cout << "A | 3:" << std::endl;
    j.print();
    std::cout << std::endl;

    SensorBuffer k = a ^ b;
    std::cout << "A ^ B:" << std::endl;
    k.print();
    std::cout << std::endl;


    // comparison operators
    std::cout << "A == B: " << (a == b) << std::endl;
    std::cout << "A != B: " << (a != b) << std::endl;
    std::cout << "A < B: " << (a < b) << std::endl;
    std::cout << "A > B: " << (a > b) << std::endl;
    std::cout << "!A: " << (!a) << std::endl;

    return 0;
}
