#include <iostream> // std::cout, std::cin
#include <string> // std::string
#include <limits> // std::numeric_limits for robust ignore

// --------------------------------------
// 9.1 - structs

// struct: groups related data together
// think of it as a "bundle" of variables that belong together, forming an object
struct Point { // 2d point, has an x and y coordinate
    int x; // member variable
    int y; // member variable
    // note for later: by default, all members are public (accessible from outside)
};

// struct with various datatypes
struct Student {
    std::string name;
    int age;
    double gpa;
    bool isEnrolled;
};

// structs can contain also other structs
struct Rectangle {
    Point topLeft; // nested struct
    Point bottomRight; // nested struct
};


int main() {
    // --------------------------------------
    // 8.1 - basics of std::string

    // empty string
    std::string empty;
    std::cout << "empty string: \"" << empty << "\" (length: " << empty.length() << ")" << std::endl;

    // initialize it with a string literal
    std::string greeting = "Hello";
    std::cout << "greeting: \"" << greeting << "\"" << std::endl;

    // initialize it with another string
    std::string copy = greeting;
    std::cout << "copy: \"" << copy << "\"" << std::endl;

    // initialize it with repetition (5 copies of 'A')
    std::string repeated(5, 'A');
    std::cout << "repeated: \"" << repeated << "\"" << std::endl;

    // initialize it from a substring
    std::string world = "World";
    std::string partial(world, 0, 3); // from index 0, take 3 characters
    std::cout << "partial (first 3 chars of 'World'): \"" << partial << "\"" << std::endl;

    // --------------------------------------
    // 8.2 - std::string operations

    // concatenation with + operator
    std::string firstName = "Ada";
    std::string lastName = "Lovelace";
    std::string fullName = firstName + " " + lastName; // combines strings
    std::cout << "fullName: \"" << fullName << "\"" << std::endl;

    // += operator (append to existing string) (similar to strcat but for std::string_
    std::string message = "Hello";
    message += " there"; // equivalent to: message = message + " there";
    message += '!'; // can append single characters too
    std::cout << "message after +=: \"" << message << "\"" << std::endl;

    // comparison operators: ==, !=, <, >, <=, >=
    // these operators compare strings lexicographically by their ASCII code (underlying int value), character by character
    std::string str1 = "apple";
    std::string str2 = "banana";
    std::string str3 = "apple";

    std::cout << "str1 == str3: " << (str1 == str3) << " (should be 1/true)" << std::endl;
    std::cout << "str1 != str2: " << (str1 != str2) << " (should be 1/true)" << std::endl;
    std::cout << "str1 < str2: " << (str1 < str2) << " (should be 1/true, 'a' comes before 'b')" << std::endl;

    // [] operator: access individual characters (0-indexed)
    std::string word = "Programming";
    char firstChar = word[0];  // 'P'
    char fifthChar = word[4];  // 'r'
    std::cout << "word[0]: '" << firstChar << "', word[4]: '" << fifthChar << "'" << std::endl;

    // modify characters using []
    word[0] = 'p'; // change 'P' to 'p'
    std::cout << "After word[0] = 'p': \"" << word << "\"" << std::endl;

    // note: accessing out-of-bounds with [] is undefined behavior!
    // use .at() for bounds-checked access (throws exception if out of bounds)
    // word.at(100) = 'p'; // would throw std::out_of_range exception

    // modify characters using .at()
    word.at(0) = 'P'; // change 'p' back to 'P'
    std::cout << "After word.at(0) = 'P': \"" << word << "\"" << std::endl;

    // --------------------------------------
    // 8.3 - std::string common methods

    std::string sample = "  Hello World  ";

    // .length() or .size() - both return the number of characters
    // .size() is present just for compatibility with other containers that hopefully we'll see later
    std::cout << "sample: \"" << sample << "\"" << std::endl;
    std::cout << "sample.length(): " << sample.length() << std::endl;
    std::cout << "sample.size(): " << sample.size() << " (same as length)" << std::endl;

    // .empty() - checks if string has no characters
    std::cout << "sample.empty(): " << sample.empty() << " (0 means false)" << std::endl;
    std::cout << "empty.empty(): " << empty.empty() << " (1 means true)" << std::endl;

    // .clear() - removes all characters
    std::string temp = "temporary";
    temp.clear();
    std::cout << "After temp.clear(): \"" << temp << "\" (length: " << temp.length() << ")" << std::endl;

    // .substr(start, length) - extract substring
    std::string phrase = "C++ Programming";
    std::string sub1 = phrase.substr(0, 3); // from index 0, take 3 chars -> "C++"
    std::string sub2 = phrase.substr(4); // from index 4 to end -> "Programming"
    std::cout << "phrase.substr(0, 3): \"" << sub1 << "\"" << std::endl;
    std::cout << "phrase.substr(4): \"" << sub2 << "\"" << std::endl;

    // .find(substring) - returns index of first occurrence, or std::string::npos if not found
    // we also have .rfind(substring) - returns index of last occurence, or std::string::npos if not found
    std::string text = "The quick brown fox";
    std::size_t pos = text.find("quick");
    if (pos != std::string::npos) {
        std::cout << "Found 'quick' at index: " << pos << std::endl;
    }

    pos = text.find("slow");
    if (pos == std::string::npos) {
        std::cout << "'slow' not found (npos means not found)" << std::endl;
    }

    // .replace(start, length, newString) - replace part of string
    std::string sentence = "I love Java";
    sentence.replace(7, 4, "C++"); // from index 7, replace 4 chars with "C++"
    std::cout << "After replace: \"" << sentence << "\"" << std::endl;

    // .insert(position, string) - insert string at position
    std::string base = "Hello World";
    base.insert(5, " Beautiful"); // insert at index 5
    std::cout << "After insert: \"" << base << "\"" << std::endl;

    // .erase(start, length) - remove characters
    std::string longText = "Hello Beautiful World";
    longText.erase(5, 10); // from index 5, remove 10 characters
    std::cout << "After erase: \"" << longText << "\"" << std::endl;

    // .append(string) - add to end (similar to +=)
    std::string start = "Hello";
    start.append(" World");
    std::cout << "After append: \"" << start << "\"" << std::endl;

    // .push_back(char) - add single character to end
    std::string letters = "ABC";
    letters.push_back('D');
    std::cout << "After push_back('D'): \"" << letters << "\"" << std::endl;

    // .pop_back() - remove last character
    letters.pop_back(); // removes 'D'
    std::cout << "After pop_back(): \"" << letters << "\"" << std::endl;

    // .c_str() - get C-style string (const char*) - useful for legacy functions
    std::string modern = "Modern C++";
    const char* cStyle = modern.c_str();
    std::cout << "modern.c_str(): \"" << cStyle << "\" (type: const char*)" << std::endl;

    // --------------------------------------
    // 8.5 - reading strings from input and final notes

    // using >> operator will reads until the first whitespace (space, tab, newline)
    // std::string name;
    // std::cout << "Enter your first name: ";
    // std::cin >> name; // stops at first space
    // std::cout << "Hello, " << name << "!" << std::endl;

    // using std::getline will read the entire line including spaces (and will exclude the ending newline)
    // std::string fullLine;
    // std::cout << "Enter a full sentence: ";
    // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore leftover newline from previous input (>>), if any
    // std::getline(std::cin, fullLine); // reads until newline
    // or, alternatively, you can do "std::getline(std::cin >> std::ws, fullLine);" // has same effect as ignore + regular getline
    // std::cout << "You entered: \"" << fullLine << "\"" << std::endl;

    // note: avoid this mistake when handling char*
    // C-style string comparison
    // const char* cs1 = "test";
    // const char* cs2 = "test";
    // if (cs1 == cs2) // this is wrong, as this compares addresses, not contents

    // key differences between std::string and char*
    // 1. Memory management: char* has to have manual memory management, whereas std::string handles that automatically, so no need for us
    // to track size or reallocate memory
    // 2. Safety: char* allows for buffer overruns and doesn't have automatic bound checks, whereas std::string grow 
    // automatically and we can use .at() to ensure bound checks
    // 3. Accessibility: char* uses functions such as strcmp, strcat and strcpy for comparing, appending and copying, whereas
    // std::string uses the natural operator ==, + and =, with the added safety from automatic growing

    // Best practices: Whenever possible, use std::string.

    // --------------------------------------
    // 9.2 - struct example

    // structs are just like the regular data types we've seen (int, char, float etc..), just that they have a format which is defined by us
    // we can create arrays/matrices or whatever of structs, dynamic arrays, matrices or whatever of structs
    // we can pass them by value or by reference to functions, everything still applies

    // however, they differ in the fact that they don't have, by default, many operators by default
    // specifically, they only have the "=" operator defined, which copies elements field by field, exactly as they are (shallow copy)
    // note: they have more stuff, but we'll see what later in the course
    // beware when having pointers inside structs, as that default "=" operator means that they will copy the pointer (memory address)
    // and you will have a pointer referenced in 2 places
    // static arrays are deep copied

    // if you want to define custom operators for your structs, you have to do them manually
    // e.g. compare them element for element, add elements together and create a new one and so on
    // we'll see how we can define our own operators further down the road in this course (that can also be done for structs, but for now, you don't know that)

    Point p1; // declare variable of type Point
    p1.x = 10; // access members with dot (.) operator
    // if "p1" was of the type Point*, we would access its members by using the arrow (->) operator
    // for e.g.: "p1->x = 10;"
    p1.y = 20;
    std::cout << "Point p1(" << p1.x << ", " << p1.y << ")" << std::endl;

    // initialize struct members at declaration
    Point p2 = { 5, 15 }; // legacy C-style initialization
    Point p3{ 30, 40 }; // modern C++ uniform initialization (preferred) (doesn't use assignment operator)
    // members are initialized in the order they appear in the struct
    std::cout << "Point p2(" << p2.x << ", " << p2.y << ")" << std::endl;
    std::cout << "Point p3(" << p3.x << ", " << p3.y << ")" << std::endl;

    // create a Student struct
    // it is created just as the other structs
    Student alice{ "Alice Johnson", 20, 3.8, true };
    // equivalent to
    // alice.name = "Alice Johnson";
    // alice.age = 20;
    // alice.gpa = 3.8;
    // alice.isEnrolled = true;
    std::cout << "Student " << alice.name << ", age " << alice.age << ", GPA " << alice.gpa 
        << (alice.isEnrolled ? " is enrolled" : " is not enrolled.") << std::endl; // ternary operator needs to be wrapped in parantheses in here
    // for the compiler to recognize it as a single statement and not panic

    Rectangle rect{ {0, 100}, {50, 0} };
    // equivalent to
    // rect.topLeft = { 0, 100 }; // top-left corner
    // <=> rect.topLeft.x = 0; rect.topLeft.y = 100;
    // rect.bottomRight = { 50, 0 }; // bottom-right corner
    // <=> rect.bottomRight.x = 50; rect.bottomRight.y = 0;
    std::cout << "Rectangle top left at (" << rect.topLeft.x << ", " << rect.topLeft.y << ")";
    std::cout << ", rectangle bottom right at (" << rect.bottomRight.x << ", " << rect.bottomRight.y << ")" << std::endl;

    return 0;
}