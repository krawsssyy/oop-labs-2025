#define _CRT_SECURE_NO_WARNINGS // for regular strcpy and not _s variants
#include <iostream>
#include <cstring> // for the implementation of strcpy, strlen etc..
#include <string> // for std::string

class Book {
	private:
		char* title;
		char* author;
		int year;
		int ID;
		static int idCounter; // this is a static variable
		// static variables are class members, but they are not specific to it
		// they get initialized only once, and are shared by all objects of that class (no re-initialization upon a new object)
		// we initialize them outside of the class with a specific value, and then inside we can use that value for whatever we may need
		// in our case, we use it to keep track of the IDs
		// we will initialize the ID with 1, and then use it such that each book gets an incremental ID value
		// note: generally, we don't use this when referring to static variables
		// outside of the class, we refer to them by the class (for e.g., in this it will be Book::idCounter).
		const int maxLen = 256; // this is a const variable
		// const variables are class members, specific to every object (each object gets its own copy) and they do get initialized for each object
		// however, they can only be initialized (not assigned), and therefore will have the same value across all objects
		// being a const variable, its value cannot change
		// they are exactly the same as any other const variable that's being used outside the class
		// in our case, we use it to provide a maximum length for the name of the author and the title of the book

		// side-note:
		// const before the function's signature just modifies the return type to return a const variable (whose value cannot be changed)
		// ideally, getters should return const values, since there's no purpose in modifying them (only where it makes sense)
	public:
		Book() : title(nullptr), author(nullptr), year(0) {
			// we could've also done ID(idCounter++) in the initialization list, but I kept it separate to highlight it
			this->ID = Book::idCounter++; // set id to current idCounter value and then increment it (
			// note: we can't use 'this' to refer to static variables in static functions
			// in non-static member functions, we can use idCounter, this->idCounter, or Book::idCounter
			// using Book:: makes it clear that this is a class-level (static) variable
			// post-fixed ++ first uses the current value of the variable in whatever operation we have it (in this case the assignment), and only after that succeeded increments the value by 1
			// pre-fixed ++ does the opposite, first it increments the value by 1, and only after uses the (new) value in the operation
		}

		Book(const char* title, const char* author, const int year): title(nullptr), author(nullptr), year(0) { // we initialize the pointers to nullptr and year to 0 to have correct values for error cases
			// name before the parentheses is the member attribute, and name inside is the value/parameter we want to initialize it with
			// due to that, in the initialization list there's no need to use "this->" to avoid naming conflicts
			// whereas inside the body, we do need to use it to differentiate
			// apart from that, it is a good practice to always use "this->", such that an external reader would know that you are referring to a class attribute and not some odd variable
			// note: it is also a good practice to use const for the parameters whose values we won't modify
			// it is a further safe-guard against accidental mistakes, such that we don't tamper the values before initializing our attributes
			// further side-note: if we want to avoid making copies of variables, we could also use "&" as the type of the parameters
			// in our case, the char arrays are already passed by pointer which means no copy, and int is trivial to copy and it is unnecessary as there's no performance impact from it
			// for more complex types, usage of "&" may save a bit of time and memory
			this->ID = Book::idCounter++;
			// use setters to avoid code repetition
			this->setTitle(title);
			this->setAuthor(author);
			this->setYear(year);
		}

		~Book() { // cleanup for our dynamic attributes
			if (this->title) {
				delete[] this->title;
				this->title = nullptr;
			}
			if (this->author) {
				delete[] this->author;
				this->author = nullptr;
			}
		}

		const char* getTitle() const { // const after the function's signature means that the function will not modify any class attributes
			// it is a good practice to use it on functions which don't modify attributes, as you'll get an error if you accidentally do so
			// that allows you to fix it before building your solution
			// note: this const also will allow us to use this function on a const object of this class, but more on that later
			// note: in this case, since the return type is a pointer, that const means that the pointer points to data of the type const char, not that the pointer is const
			if (!this->title) { // if we have no title, return nullptr
				return nullptr; // caller will have to check the return value
			}
			// for dynamic attributes, we return a deep copy (new allocation with copied data)
			// if we returned this->title directly, both pointers would share the same memory address
			// this would allow the caller to cast away const, modify our data, or even delete it
			// by creating a new copy, we protect our internal data from any external interferences
			char* titleCpy = new char[strlen(this->title) + 1]; // since we allocate here and then return this value
			// it is up to the caller to clean-up this memory
			strcpy(titleCpy, this->title);
			return titleCpy;
		}

		const char* getAuthor() const {
			// same here, caller will clean up the memory
			if (!this->author) {
				return nullptr;
			}
			// we don't need to check for the length here since the only ways of setting this value are safe-guarded
			// if we don't have the author as nullptr, we know we have a valid string inside
			char* authorCpy = new char[strlen(this->author) + 1];
			strcpy(authorCpy, this->author);
			return authorCpy;
		}

		int getYear() const { // no sense for const in front here, since the return is by-value and a copy is made anyway
			// trailing const is still important - as it marks the function as not modifying member variables
			return this->year;
		}

		int getID() const {
			return this->ID;
		}

		void setTitle(const char* title) {
			if (!title) {
				std::cout << "Title cannot be null!" << std::endl;
				return; // early exit if we detect any issues
				// since this is a void function and doesn't return anything, that return statement is empty
				// it still works and exits this function, but won't return anything
				// we don't delete this->title here - we keep the old value if the new one is invalid
			}
			if (strlen(title) > 0 && strlen(title) < this->maxLen) {
				if (this->title) { // check if our title is already allocated
					// if yes, then de-allocate before we re-allocate it
					delete[] this->title;
					this->title = nullptr;
				}
				this->title = new char[strlen(title) + 1];
				strcpy(this->title, title);
			}
			else {
				std::cout << "Title cannot be empty or exceeding than the max length!" << std::endl;
			}
		}

		void setAuthor(const char* author) {
			if (!author) {
				std::cout << "Author cannot be null!" << std::endl;
				return;
			}
			if (strlen(author) > 0 && strlen(author) < this->maxLen) {
				if (this->author) {
					delete[] this->author;
					this->author = nullptr;
				}
				this->author = new char[strlen(author) + 1];
				strcpy(this->author, author);
			}
			else {
				std::cout << "Author cannot be empty or exceeding than the max length!" << std::endl;
			}
		}

		void setYear(const int year) {
			if (year > -1 && year < 2026) {
				this->year = year;
			}
			else {
				std::cout << "Year cannot be negative or bigger than the current year!" << std::endl;
			}
			
		}

		// no setter for ID, since we allocate it via the constructors

		static void greetFromBook() {
			std::cout << "Hello dear reader, welcome to the book management program!" << std::endl << std::endl;
			// we can also have the concept of static functions
			// static functions are functions which do not use any class attributes, and can be used as various helpers
			// such as converting from various number bases for a calculator, printing various messages and so on..
			// note: since static functions cannot use non-static member variables, we cannot use "this->" inside them
			// static functions CAN use static member variables (like idCounter), but not instance-specific data
			// attempting to use non-static members will result in a compiler error
		}
};

int Book::idCounter = 1; // 1-index for our IDs
// this is how we initialize a static variable
// it needs to be initialized in the global scope (outside of any function), such that the compiler will know how to create the variable
// we don't need to specify the "static" keyword again, but we do need to specify the data type, and we need to use the class identifier
// in our case, "Book::"

int main() {
	Book::greetFromBook(); // for calling static functions, we also need to use the class identifier
	// and we don't need to instantiate any object for usage of this
	// the static function is related to the class, but is not specific to a single object of it, just like static variables
	int noBooks;
	std::cout << "Enter number of books: "; std::cin >> noBooks;
	if (noBooks < 1) {
		std::cout << "Number of books cannot be less than 1!" << std::endl;
		return 1;
	}

	Book* books = new Book[noBooks]; // calls default constructor for each
	for (int i = 0; i < noBooks; i++) {
		std::string title, author;
		std::cout << "Enter title for book " << i + 1 << " : "; // 1-index
		std::getline(std::cin >> std::ws, title); // reads the book's title with spaces included, and discard leftover '\n' from previous reads
		std::cout << "Enter author for book " << i + 1 << " : ";
		std::getline(std::cin >> std::ws, author);
		int year;
		std::cout << "Enter publishing year for book " << i + 1 << " : "; std::cin >> year;
		// note: we use the setters here to avoid doing books[i] = Book(title.c_str(), author.c_str(), year)
		// that one invokes 2 more advanced concepts we haven't discussed yet and will wrongly create our attributes and lead to memory leaks, since shallow-copy of pointers will be involved
		// the concepts are - assignment operator and copy constructor
		// we will get back to this after discussing them, but for now keep using setters for setting the attributes
		books[i].setTitle(title.c_str()); // we use .c_str() to convert our std::string to a const char*, such that the parameter to our function is correct in type
		books[i].setAuthor(author.c_str());
		books[i].setYear(year);
	}

	std::cout << std::endl << "Listing all books..." << std::endl;
	for (int i = 0; i < noBooks; i++) {
		std::cout << "Book ID: " << books[i].getID() << std::endl; // with this we verify that our ID assignment worked
		// it increments the IDs correctly
		const char* currentTitle = books[i].getTitle();
		// we need to save these as values for destruction later since we allocate memory in the getter
		// if we would just call them in the std::cout, then we would have memory leaks since we didn't clean up
		// when the callee said we should
		const char* currentAuthor = books[i].getAuthor();
		std::cout << "Title: " << (currentTitle ? currentTitle : "(empty)") << std::endl; // ensure we don't try to print nullptr
		std::cout << "Author: " << (currentAuthor ? currentAuthor : "(empty)") << std::endl;
		std::cout << "Year: " << books[i].getYear() << std::endl << std::endl;

		// clean-up for our values from the getters
		if (currentTitle) {
			delete[] currentTitle;
			currentTitle = nullptr;
		}
		if (currentAuthor) {
			delete[] currentAuthor;
			currentAuthor = nullptr;
		}
	}

	delete[] books; // clean-up for our dynamic array
	// will call the destructor for each element inside, and therefore all memory will be cleaned up
	books = nullptr;
	return 0;
}