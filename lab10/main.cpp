#include <iostream>
#include <string>

class Animal { // sample class for testing
	private:
		std::string name;
		int age;
	public:
		Animal() { 
			this->name = ""; 
			this->age = 0; 
		}
		Animal(std::string name, int age) { 
			this->name = name; 
			this->age = age; 
		}
		Animal(const Animal& other) { 
			this->name = other.name; 
			this->age = other.age; 
		}
		~Animal() { } // empty destructor

		Animal& operator=(const Animal& other) { // rule of three
			if (this != &other) {
				this->name = other.name;
				this->age = other.age;
			}
			return *this;
		}

		std::string getName() const { 
			return this->name; 
		}

		int getAge() const {
			return this->age;
		}
		// until now, we have done the following operators
		// =, +=, -=, *=, /=, ^=, &=, |=, +, -, *, /, ^, &, |, ==, !=, <, <=, >, >=, !


		// keep in mind, the binary operators (+, -, *, /, ^, &, |) and comparison ones, work only for cases such as
		// ObjectOfOurClass operator AnotherObjectOrSimpleDataType
		// doing AnotherObjectOrSimpleDataType operator ObjectOfOurClass does not work with the definitions we provided before
		// because operators are not commutative

		Animal operator+(int val) { // this works only for Animal + int, and not for int + Animal
			Animal tmp = Animal(*this);
			tmp.age += val;
			return tmp;
		}

		// for AnotherObjectOrSimpleDataType operator ObjectOfOurClass, we need to define an operator outside of the class (a global operator)
		// for that, when we have private fields (that cannot be accessed outside the class's definition), we need to declare that function as a friend
		// using the "friend" keyword, we can tell C++ that the function with the given signature, is a "friend" of this class
		// and can access its private fields
		
		// for declaring a function as a friend, we use the "friend" keyword, followed by the function's signature, as such:
		friend Animal operator+(int val, Animal animal);
		
		// NOTE: we can define friend global operators inside of our class as well
		// that also breaks several clean coding principles, and we'd like to keep things separated where possible
		// for small examples this might be ok, but in production code, there are subtle difference which favor the declaring of friend functions
		// outside of the class
		// so please, abide by that rule and keep the friend implementations outside of the class

		// additionally, we can also declare operators for Animal + int in a similar way using global operators
		// but that breaks several clean coding principles and we'd like to keep everything related to our class inside of it, when possible
		// friend Animal operator+(Animal animal, int val);

		// new operators

		// NOTE: the implementations of these operators are just an example that was relatively fit for this class
		// and not how all operators should always be implemented
		// the operators' function signatures are to be taken as is, as these are the correct definitions
		// but the implementations vary from example to example, and should do what makes sense for the class you're working on

		// indexing operator ([])
		// used for adding custom behavior when indexing an object of your class

		// SYNTAX: ReturnType operator[](int index) -> read only OR const ReturnType& operator[](int index) - additionally you can add const after (if you need it working on const objects)
		// the return type specified is the type of the elements of the dynamic array we want to index
		
		// to have both variants in our program (and we ideally should), the read-only variant should ideally have both consts (if not both then at least just one
		// such as the operators differ by a const - that is required, as they can't only differ by return type (value vs reference))
		// for a C++ principle called const-correctness
		// as const objects should be indexable, but not modifiable
		// so with this, we make const objects be read only, and the non-const objects actually be modifiable
		const char operator[](int index) const { // read only version, we can only do a[10], and not write any values to it
		 // since the return value in this case is a copy, it doesn't reference the original array and is not modifiable
			if (index < 0 || index >= this->name.length()) {
				throw "...";
			}
			return this->name[index];
		}

		// SYNTAX: ReturnType& operator[](int index) -> read + write
		char& operator[](int index) { // also allows for write access, such as a[10] = 'm';
			// we index into our string
			if (index < 0 || index >= this->name.length()) { // validate index to not go out of bounds
				throw "...";
			}
			return this->name[index];
		}

		// explicit cast operators
		// used for defining custom behaviour when (explicitly) casting an object of your class to another data type
		// explicit casts e.g. - (int)a, static_cast<int>(a), dynamic_cast<int>(a), reinterpret_cast<int>(a),...
		// the first one is the one we use, the others are more advanced and can be researched for a future read

		// SYNTAX: explicit operator DataType() const
		// here, DataType can be anything you want, either a trivial data type (int, float, bool, double, char etc...)
		// or more complex objects (std::string, std::vector) or other classes

		// just gave some sample impelemntations
		// you can have as many explicit cast operators as you want
		explicit operator float() const {
			return (float)this->age * 2.f;
		}

		explicit operator double() const {
			return (double)this->age * 3.;
		}

		// function call operators
		// used to define custom behaviour when calling an object of our class -> a();

		// SYNTAX: ReturnType operator()(Param1, Param2, ..., ParamN)
		// you have as much liberty as you want with these ones, and have as many as you want
		// the return type and list of parameters differs from use case to use case
		// you can have any return type and any list of parameters

		// just some sample examples of these operators
		int operator()() {
			return this->age * 7;
		}

		int operator()(int val) {
			return (this->age + val) * 7;
		}

		int operator()(float val) {
			return (this->age + val) * 70;
		}

		float operator()(float f1, float f2, float f3) {
			return (float)*this + f1 * f2 * f3;
		}

		// pre-fixed and post-fixed increment and decrement operators (++a, a++, --a, a--)
		// used too provide custom behaviour for the aforementioned operations, when used on an object of a class

		// SYNTAX: ClassType& operator++() / ClassType& operator--() // - pre-fixed variant
		// SYNTAX: ClassType operator++(int) / ClassType operator--(int) // post-fixed variant

		// NOTE: the post-fixed variant has a dummy "int" parameter, mostly because of historical reasons
		// C++ had required a way to distinguish between ++a and a++, and since both operators modified the class, they couldn't be made const
		// so, since they had to differ by something more than just the return type (value vs reference), they added a dummy variable for the post-fixed variant
		// that dummy variable is not used anywhere, and should just be taken as is
		// a lot more code would be needed to be modified now if that were to change, so it will just stay as it, and take it as is

		// ++ pre-fixed => ++Animal
		// increments and then returns the (same) value
		// we increment and then return the same object, since that's the behaviour of ++i
		// it first increments the value, and only after that it uses is in whatever expression it was in, using the incremented value
		// so we replicate that here as well
		Animal& operator++() {
			++this->age;
			return *this;
		}

		// ++ post-fixed => Animal++
		// first returns the value, then increments
		// we make a copy of the non-increment object and return that one, such that it is the one being used, and not the incremented one
		// we then increment our object
		// matching the behaviour of i++, which uses the non-incremented value first, and only after that it increments it
		// which is exactly what we do by returning the non-increment copy and then incrementing the current object
		Animal operator++(int) {
			Animal tmp = Animal(*this);
			++(*this);
			return tmp;
		}

		// -- pre-fixed => --Animal
		// decrements and then returns the value
		Animal& operator--() {
			--this->age;
			return *this;
		}

		// -- post-fixed => Animal--
		// first returns the value, then decrements
		Animal operator--(int) {
			Animal tmp = Animal(*this);
			--(*this);
			return tmp;
		}

		// lastly, streaming operators (<< and >>)
		// used for defining custom behaviour for when inserting an object of our type inside an output stream for processing there
		// or when extracting elements from an input stream for creating an object of our type
		// most common streams we use is std::cout, which is an output stream, and std::cin, which is an input stream
		
		// therefore, we decide how an object should be sent for display/saving to an output stream
		// and we decide how an object should be created from an input stream

		// SYNTAX: std::ostream& operator<<(std::ostream& os, const Animal& animal)
		// SYNTAX: std::istream& operator>>(std::istream& is, Animal& animal)

		// since the first parameter in these operators is not an object of the type of our class, we have to declare them globally
		// for that, we use the friend operator, together with the functions' signature
		friend std::ostream& operator<<(std::ostream& os, const Animal& animal);
		friend std::istream& operator>>(std::istream& is, Animal& animal);

		// NOTE: we can also define these operators as returning void, and not std::ostream& / std::istream& (similar tot the assigment operator's case)
		// as in the assignment operator's case, it breaks chaining, such as std::cout << a << b << c, since C++ translates that to
		// (std::cout << a) << b << c
		// if (std::cout << a) doesn't return the actual stream, it won't be able to recursively print and chain such things
		// additionally, we won't be able to check the return value (useful for checking if std::cin >> a succeeded or not)
		// since we don't return the stream to check its status
		
		// so please, try to use these correct variants where we actually return the streams

};

// then, we define the friend function outside of the class
// and as parameters, we provide the AnotherObjectOrSimpleDataType as the first one, and then ObjectOfOurClass as the second one, as such:
Animal operator+(int val, Animal animal) { // int + Animal
	// then, we just implement the operator 
	Animal tmp = Animal(animal);
	tmp.age += val;
	return tmp;
}

// example of ObjectOfOurClass + AnotherObjectOrSimpleDataType implemented as a global operator
// nothing changes, just that we had to declare it as friend inside of our class and then declare it outside
// Animal operator+(Animal animal, int val) { // Animal + int 
//	Animal tmp = Animal(animal);
//	tmp.age += val;
//	return tmp;
// }

std::ostream& operator<<(std::ostream& os, const Animal& animal) {
	// sample example providing how we want the class to be displayed in an output stream
	os << "[ " << animal.name << "," << animal.age << " ]";
	return os;
}

std::istream& operator>>(std::istream& is, Animal& animal) {
	// sample example providing how we want the class to be instantiated from elements inside an input stream
	// we simply just read element by element (for this simple case)
	// had we have a dynamic array, we would need to make a local dynamic variable and then copy it into our object's dynamic field
	is >> animal.name >> animal.age;
	return is;
}

// if we were to use void, the definitions would be the same, just that we wouldn't return anything
// void operator>>(std::istream& is, Animal&) { .. }

int main() {
	Animal a("gica", 3);
	Animal b = a + 3;
	std::cout << "a name: " << a.getName() << std::endl;
	a[1] = 'I'; // indexing operator with write access
	const Animal c = a;
	std::cout << "c[0]: " << c[0] << std::endl; // indexing operator read-only for const objects
	std::cout << "a name after a[1] = 'I': " << a.getName() << std::endl;
	std::cout << "(float)a: " << (float)a << std::endl; // explicit cast to float operator
	std::cout << "(double)b: " << (double)b << std::endl; // explicit cast to double operator
	std::cout << "a(): " << a() << std::endl; // examples of function call operators
	// which operator to call is defined by the same criteria used in function overloading
	// i.e., the number of parameters and their types
	std::cout << "a(10): " << a(10) << std::endl;
	std::cout << "a(1.f): " << a(1.f) << std::endl;
	std::cout << "a(1.f, 2.f, 3.f): " << a(1.f, 2.f, 3.f) << std::endl;
	std::cout << "a++.age: " << a++.getAge() << std::endl;
	std::cout << "a.age after a++: " << a.getAge() << std::endl;
	std::cout << "++a.age: " << (++a).getAge() << std::endl;
	std::cout << "actual obj: " << a << std::endl; // operator<<(std::cout, a);
	std::cout << "give new values for the object (name and then age): ";
	std::cin >> a; // operator>>(std::cin, a);
	std::cout << "new a: " << a << std::endl;
	return 0;
}