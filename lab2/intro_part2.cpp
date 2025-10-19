#define _CRT_SECURE_NO_WARNINGS // for usage of strcpy, strcat
// they are deprecated and visual studio doesn't let us compile without this
// they are not to be used, but they have been shown here as examples
// this has to be the first statement, such that no subsequent include directives somehow already import strcpy and overwrite this define directive
#include <iostream> // std::cout, std::cin
#include <string> // std::string
#include <limits> // std::numeric_limits for robust ignore
#include <cstring> // for C-string functions
#include <cctype> // for character classification functions


// --------------------------------------
// 10.1 - structs

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
    // 8.1 - working with char* arrays (C-style strings)
    // C-strings are null-terminated character arrays (ASCIIZ)
    // '\0' marks the end of the string - this is important, as functions won't otherwise know where the string ends

    // string literal (stored in read-only memory - it represents a pointer, but is not in the heap and nor is it dynamically allocated)
    // no de-allocation necessary
    // it is const, therefore it cannot be modified after initialization
    const char* literalStr = "Hello"; // points to "Hello\0" in memory
    std::cout << "String literal: \"" << literalStr << "\"" << std::endl;

    // character array (modifiable)
    char greetingCharArr[10] = "Hi there"; // automatically adds '\0', size must be >= actual length + 1
    std::cout << "Character array: \"" << greetingCharArr << "\"" << std::endl;

    // explicitly showing the null terminator
    char manual[6] = { 'H', 'e', 'l', 'l', 'o', '\0' }; // must include '\0'!
    std::cout << "Manual initialization: \"" << manual << "\"" << std::endl;

    // uninitialized array - always initialize before use!
    char buffer[50]; // contains garbage values
    buffer[0] = '\0'; // make it an empty string (good practice)
    std::cout << "Empty buffer: \"" << buffer << "\" (length: " << strlen(buffer) << ")" << std::endl;

    // --------------------------------------
    // 8.2 - essential C-string functions

    // strlen() - returns length (excludes '\0')
    const char* sampleCharArr = "Programming";
    std::size_t len = strlen(sampleCharArr);
    std::cout << "strlen(\"" << sampleCharArr << "\"): " << len << std::endl;

    // strcpy() - copy string (overwrites destination buffer with new data)
    // NOTE: be careful with this variant, as it can lead to security vulnerabilities called a buffer overrun if the destionation is too small
    // it doesn't check any bounds and can copy past the allocated buffer
    char dest[20];
    strcpy(dest, "Hello"); // copies "Hello\0" to dest
    std::cout << "After strcpy: \"" << dest << "\"" << std::endl;

    // strncpy() - safer copy with size limit (but has some quirks - may not add '\0', depending on how much you copy)
    char safeDest[20];
    strncpy(safeDest, "Hello World", 5); // copies only first 5 chars
    safeDest[5] = '\0'; // manually add null terminator for safety
    std::cout << "After strncpy (5 chars): \"" << safeDest << "\"" << std::endl;

    // strcat() - concatenate strings
    // NOTE: same as before, doesn't do any bound checks and can easily overrun the allocated buffer
    char combined[30] = "Hello";
    strcat(combined, " World"); // appends " World" to combined
    std::cout << "After strcat: \"" << combined << "\"" << std::endl;

    // strncat() - safer concatenate with size limit
    char safeCombined[30] = "Hello";
    strncat(safeCombined, " Beautiful World", 10); // appends at most 10 chars
    std::cout << "After strncat (10 chars max): \"" << safeCombined << "\"" << std::endl;

    // strcmp() - compare strings (returns 0 if equal, <0 if str1 < str2, >0 if str1 > str2)
    // parses the strings and compares them lexicographically until '\0'
    const char* strA = "apple";
    const char* strB = "banana";
    const char* strC = "apple";

    std::cout << "strcmp(\"apple\", \"banana\"): " << strcmp(strA, strB) << " (negative, 'apple' < 'banana')" << std::endl;
    std::cout << "strcmp(\"apple\", \"apple\"): " << strcmp(strA, strC) << " (zero, they're equal)" << std::endl;

    // strncmp() - compare first n characters (same return values)
    std::cout << "strncmp(\"apple\", \"apply\", 4): " << strncmp("apple", "apply", 4) << " (zero, first 4 chars match)" << std::endl;
    // even though they differt at position 5, we only compare the first 4, therefore they are equal until that point

    // --------------------------------------
    // 8.3 - searching in C-strings

    // strchr() - find first occurrence of character (returns pointer to the string from the first occurence onward or nullptr if not found)
    const char* textToSearch = "Hello World";
    const char* found = strchr(textToSearch, 'o');
    if (found != nullptr) {
        std::cout << "strchr found 'o' at position: " << (found - textToSearch) << std::endl;
        // pointer arithmetic to get the offset
        // since found is a pointer to a string, it holds a memory address
        // since it is a pointer to somewhere in our "textToSearch" string, it is a bigger memory address
        // their difference represents the offset of that character in our string (its position)
        std::cout << "Remaining string from 'o': \"" << found << "\"" << std::endl;
    }

    // strrchr() - find last occurrence of character
    const char* lastO = strrchr(textToSearch, 'o');
    if (lastO != nullptr) {
        std::cout << "strrchr found last 'o' at position: " << (lastO - textToSearch) << std::endl;
    }

    // strstr() - find first occurrence of substring (same return values as strchr)
    const char* sentenceToSearch = "The quick brown fox jumps";
    const char* substring = strstr(sentenceToSearch, "brown");
    if (substring != nullptr) {
        std::cout << "strstr found \"brown\" at position: " << (substring - sentenceToSearch) << std::endl;
        std::cout << "Remaining string: \"" << substring << "\"" << std::endl;
    }

    // --------------------------------------
    // 8.4 - tokenizing strings

    // strtok() - split string by delimiters (MODIFIES original string!)
    // NOTE: strtok is not thread-safe and has internal state
    char data[] = "apple,banana,cherry"; // must only be a modifiable array, not const char*
    const char* delimiters = ",";

    std::cout << "Tokenizing \"" << data << "\" by comma:" << std::endl;
    char* token = strtok(data, delimiters); // first call with string
    while (token != nullptr) {
        std::cout << "Token: \"" << token << "\"" << std::endl;
        token = strtok(nullptr, delimiters); // subsequent calls use nullptr, as the rest was stored in the internal states of strtok
    }
    std::cout << "New data char* array: \"" << data << "\"" << std::endl;
    // NOTE: original 'data' array is now modified! (commas replaced with '\0')

    // example with multiple delimiters
    char path[] = "usr/local/bin:include";
    std::cout << "Tokenizing path with '/' and ':' delimiters:" << std::endl;
    token = strtok(path, "/:");
    while (token != nullptr) {
        std::cout << "Token: \"" << token << "\"" << std::endl;
        token = strtok(nullptr, "/:");
    }

    // --------------------------------------
    // 8.5 - character classification

    char ch = 'A';
    // these work for singular characters
    std::cout << "Character classification for '" << ch << "':" << std::endl;
    std::cout << "isalnum: " << (isalnum(ch) ? "yes" : "no") << std::endl; // is alphanumeric (i.e. A-Z, a-z or 0-9)
    std::cout << "isalpha: " << (isalpha(ch) ? "yes" : "no") << std::endl; // is letter (i.e. A-Z or a-z)
    std::cout << "isdigit: " << (isdigit(ch) ? "yes" : "no") << std::endl; // is digit (0-9)
    std::cout << "isupper: " << (isupper(ch) ? "yes" : "no") << std::endl; // is uppercase (A-Z)
    std::cout << "islower: " << (islower(ch) ? "yes" : "no") << std::endl; // is lowercase (a-z)
    std::cout << "tolower: '" << (char)tolower(ch) << "'" << std::endl; // converts to lowercase

    // --------------------------------------
    // 9.1 - basics of std::string

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
    // 9.2 - std::string operations

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
    // 9.3 - std::string common methods

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
    // 9.5 - reading strings from input and final notes

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
    // 10.2 - struct example

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
        << (alice.isEnrolled ? " is enrolled" : " is not enrolled.") << std::endl; // ternary operator needs to be wrapped in parentheses in here
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
