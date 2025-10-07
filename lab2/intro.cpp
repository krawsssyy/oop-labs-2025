#include <iostream>   // std::cout, std::cin
#include <cstring>    // std::strlen for char arrays

// General note on syntax:
// - A *statement* ends with a semicolon (;). Example: int x = 5;
// - Curly braces { } create a *block* that groups statements.
// - Whitespace (spaces/newlines) usually doesn't matter, but readability does.
// - C++ is case-sensitive.

// --------------------------------------
// 1.1 - global types

// global variables are available all throughout the program, you can use them everywhere in your code
// as opposed to (local) variables defined within {} (blocks of statements), such as when done in functions and loops
// this is called the scope of a variable (i.e., it's life-span)
// once a variable goes "out-of-scope", it no longer exists and cannot be used
// there's no difference between local and global variables, just their scope

int globalVar = 0;


// --------------------------------------
// 5.1 - functions
// function prototype (declaration) tells the compiler the function’s interface
// Syntax: <return_type> <name>(<argument_1>, <argument_2>, ..., <argument_n>) { body }
// we use prototypes when we want to use functions early, but declare them later
// the arguments for a function are called parameters
// the list of parameters, defined by their type, together with the function's name constitute the function's signature
int add(int a, int b); // returns an int (sum)
double average(const int arr[], int length); // returns a double
void printDivider(char ch, int count); // returns nothing (void)

// the actual definition of add: adds two ints and returns the result
int add(int a, int b) {
    // pass-by-value
    // when we don't use "&" next to the type of a parameter, it is passed by value
    // this means that the function won't modify the actual variable that you pass
    // the function will get a copy of it, so any modifications won't be reflected outside of the function (i.e., where you called it)
    // for later: when using big objects as parameters, it is recommended that we do "const <type>& <var_name>", such that it is still
    // passed by value, but it won't be copied, therefore it is faster and uses less memory
    // 'return' ends the function and provides a value to the caller
    return a + b;
}

// calculate average of an int array; "const" means we won’t modify arr.
// in c++, arrays are pass-by-reference by default, meaning that any modification to that variable inside the function will be reflected! be aware of this
// by saying that the array is "const", we tell the compiler that it is not modifiable - any attempts will result in an error
// for arrays, we also pass the length as a parameter
double average(const int arr[], int length) {
    if (length == 0) {
        return 0.0; // avoid division by zero
    }
    long long sum = 0;
    for (std::size_t i = 0; i < length; i++) {
        sum += arr[i];
    }
    return sum / (double) length; // when dividing 2 integers, if we need the result to be a decimal, we need to "cast" one of them to that type
    // casting means that the compiler will interpret that variable having a different type other than the one it has (only for the given statement)
}

// print a repeated character as a divider line. Returns void (no value).
void printDivider(char ch, int count) {
    for (int i = 0; i < count; i++) {
        std::cout << ch;
    }
    std::cout << std::endl;
}

// --------------------------------------
// 6.1 - pointers

// function that increments an int "by pointer" (so it can modify caller's variable)
void incrementByPointer(int* pValue) {
    if (pValue != nullptr) { // always check before dereferencing
        *pValue = *pValue + 1; // *pValue means "the int at that address" (*-dereferencing operator; or "contents-of-address")
    }
}

// swap two ints using pointers
void swapByPointer(int* a, int* b) {
    if (a != nullptr && b != nullptr) {
        int temp = *a;
        *a = *b;
        *b = temp;
    }
}

// --------------------------------------
// 7.1 - enums

// Unscoped enum: older variant. Implicitly convertible to int (can cause accidental bugs by matching values with other type from an enum).
// enum is a data type which allows us to define multiple variants for a single variable, i.e. LegacyDay is only one out of all these possible values
enum LegacyDay { MON, TUE, WED, THU, FRI, SAT, SUN };

// Scoped enum: modern style, and safer. We prefer those.
// Not implicitly convertible to int; you must cast if you need the integer.
// You can pick an underlying type to control size by inheriting (more on that later).
enum class TrafficLight { Red = 1, Yellow = 2, Green = 3 };

int main() { // the entry point of the program - the operating system calls main() to start the program
    // the definition can be left out like this, or used as "int main(int argc, char** argv)" for command line scripts
    // for example, if we run our program by: "program.exe hello there"
    // argc (arguments count) will be 3 (includes the program's name as an argument)
    // argv (arguments values) will be { { "h", "e", "l", "l", "o" }, { "t", "h", "e", "r", "e" } }
    // for strings, it is equivalent to { "hello", "there" }

    // single-line comment
    /*
    multi-line
    comment
    */

    // --------------------------------------
    // 1.2 - local variables, data types and sizes

    // boolean: true/false
    bool flag = true; // 1 byte (typically), exact size can vary across systems
    // characters: single byte integer used for characters
    char letter = 'A'; // single quotes are used to denote char literals

    // integers: signed (allow for negative values) by default; sizes vary by platform, but common on modern systems are:
    // short (often 2 bytes), int (often 4), long (4 or 8), long long (often 8).
    short s = -123;
    int i = 42;
    long l = 100000L;
    long long ll = 9000000000LL;

    // unsigned integers: only non-negative values, effectively doubling the positive range.
    unsigned int ui = 42u;

    // floating point numbers (decimals).
    float f = 3.14f; // ~6-7 decimal digits precision
    double d = 2.718281828; // ~15 decimal digits precision
    long double ld = 1.234567890123456789L; // often more than double

    // sizeof gives the size in bytes of a type or variable.
    // std::cout is used to print characters and variables to the console
    // std::cin is used to read data from the console into variables (uses >> instead of <<)
    std::cout << "flag (bool) = " << flag << ", sizeof(bool) = " << sizeof(bool) << " bytes" << std::endl;
    std::cout << "letter (char) = " << letter << ", sizeof(char) = " << sizeof(char) << " bytes" << std::endl;
    std::cout << "short = " << s << ", sizeof(short) = " << sizeof(short) << " bytes" << std::endl;
    std::cout << "int = " << i << ", sizeof(int) = " << sizeof(int) << " bytes" << std::endl;
    std::cout << "long = " << l << ", sizeof(long) = " << sizeof(long) << " bytes" << std::endl;
    std::cout << "long long = " << ll << ", sizeof(long long) = " << sizeof(long long) << " bytes" << std::endl;
    std::cout << "unsigned int = " << ui << ", sizeof(unsigned int) = " << sizeof(unsigned int) << " bytes" << std::endl;
    std::cout << "float = " << f << ", sizeof(float) = " << sizeof(float) << " bytes" << std::endl;
    std::cout << "double = " << d << ", sizeof(double) = " << sizeof(double) << " bytes" << std::endl;
    std::cout << "long double = " << ld << ", sizeof(long double) = " << sizeof(long double) << " bytes" << std::endl;

    // constants: prefer const for values that should not change.
    const int DAYS_IN_WEEK = 7; // UPPER_CASE is a common naming style for constants.
    std::cout << "DAYS_IN_WEEK (const int) = " << DAYS_IN_WEEK << std::endl;

    // --------------------------------------
    // 2 - static arrays

    // static array of length 5
    int numbers[5] = { 10, 20, 30, 40, 50 }; // indices: 0, 1, 2, 3, 4 (zero-based)
    // access by index:
    int first = numbers[0]; // first element
    int last  = numbers[4]; // last element
    numbers[2] = 999; // modify element at index 2

    std::cout << "numbers = "; 
    std::cout << "[";
    for (int idx = 0; idx < 5; idx++) { // idx++ is equivalent to saying idx += 1 or idx = idx + 1
        std::cout << arr[idx];
        if (idx + 1 < length) { // even though single statements do not require { }, it is a good practice to always use them
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl; // std::endl means newline, moves the output pointer to the next line
    std::cout << "first = " << first << ", last = " << last << std::endl;

    // static char array (C-style string): ends with '\0' (null terminator)
    // double quotes produce a null-terminated char array automatically - they are required for strings
    char name[] = "Ada"; // name has 4 chars: 'A', 'd', 'a', '\0'
    std::cout << "name (char array) = " << name 
              << " (length without \\0 = " << std::strlen(name) << ")" << std::endl;

    // 2D array example:
    int table[2][3] = { {1,2,3}, {4,5,6} };
    std::cout << "table[1][2] = " << table[1][2] << " (row 1, col 2)" << std::endl;

    // Best practices:
    // - Keep track of sizes separately (we pass size explicitly to functions).
    // - Stay within bounds (0..size-1). Accessing outside is undefined behavior and represents a security vulnerability.

    // --------------------------------------
    // 3 - control structures

    int age = 17;

    // simple if: run a block only if the condition is true
    if (age < 18) {
        std::cout << "You are a minor" << std::endl;
    }

    // if-else: choose between two paths
    if (age >= 18) {
        std::cout << "Adult path executed" << std::endl;
    } else {
        std::cout << "Else path executed (not an adult)" << std::endl;
    }

    // if - else if - else: choose among multiple conditions, first match wins and the rest are not considered
    int score = 85;
    if (score >= 90) {
        std::cout << "Grade: A" << std::endl;
    } else if (score >= 80) {
        std::cout << "Grade: B" << std::endl;
    } else if (score >= 70) {
        std::cout << "Grade: C" << std::endl;
    } else {
        std::cout << "Grade: below C" << std::endl;
    }

    // switch: select by exact value (works with integral/enum types and some others) - alternative for if-else if-else if....
    char command = 'h'; // imagine commands: 'h' = help, 'q' = quit, 'r' = run
    switch (command) {
        case 'h': // execute when command == 'h'
            std::cout << "Help selected" << std::endl;
            break; // exit switch here
        case 'r': // execute when command == 'r'
            std::cout << "Run selected" << std::endl;
            break;
        case 'q': // execute when command == 'q'
            std::cout << "Quit selected" << std::endl;
            break;
        default:
            std::cout << "Unknown command" << std::endl; // execute when no cases match
            break;
    }
    // Note: leaving out 'break;' causes fallthrough into the next case, which we don't want.

    // --------------------------------------
    // 4 - loops

    // FOR LOOP
    // Syntax: for (initializer; condition; increment) { body }
    // - initializer runs once before the loop starts and initializes the required variables for use within the loop
    // - condition is checked at each iteration; if false, loop ends
    // - increment runs after each iteration, defining how to increment/decrement variables declared
    std::cout << "for loop counting 0..4: ";
    for (int counter = 0; counter < 5; counter = counter + 1) {
        std::cout << counter << " ";
    }
    std::cout << std::endl;

    // WHILE LOOP
    // Syntax: while (condition) { body }
    // - checks condition BEFORE running the body; if false at the start, the body will never run
    int n = 3;
    std::cout << "while loop (n down to 0): ";
    while (n >= 0) {
        std::cout << n << " ";
        n = n - 1; // important: update condition, or you’ll loop forever!
    }
    std::cout << std::endl;

    // DO-WHILE LOOP
    // Syntax: do { body } while (condition);
    // - body runs at least once, because the condition is checked AFTER the body
    bool done = false;
    int simulatedInputs[] = {0, 0, 1}; // 1 means " done"
    int idx = 0;

    std::cout << "do-while loop: ";
    do {
        std::cout << "[step]";
        // pretend we "read" a value:
        int userSaysDone = simulatedInputs[idx++];
        if (userSaysDone == 1) {
            done = true;
        }
        // loop continues while !done (i.e., not done)
    } while (!done);
    std::cout << " <- stopped when done became true" << std::endl;

    // --------------------------------------
    // 5.2 - functions examples

    int x = 5, y = 7;
    int sum = add(x, y); // when we call a function, we use ()
    std::cout << "add(" << x << ", " << y << ") = " << sum << std::endl;

    int data[] = {10, 20, 30, 40};
    std::cout << "average([10,20,30,40]) = " << average(data, 4) << std::endl;

    std::cout << "Divider via void function:\n";
    printDivider('-', 30);
    std::cout << "This line appears below a divider." << std::endl;
    printDivider('=', 30);

    // Best practice tips:
    // - Give functions clear names; one responsibility each.
    // - Use meaningful parameter names.
    // - Use const& where you don't modify inputs.

    // --------------------------------------
    // 6.2 - pointers examples

    int value = 10;
    int* p = &value; // '&' gets the "address-of" a variable; p holds a memory address

    std::cout << "value = " << value << std::endl;
    std::cout << "p (address) = " << p << std::endl; // memory address, relatively high value
    std::cout << "*p (dereferenced) = " << *p << std::endl; // the actual value

    // modify value through the pointer:
    *p = 99;
    std::cout << "After *p = 99, value = " << value << std::endl;

    // always initialize pointers; if you have no address yet, use nullptr.
    int* unsafe; // UNINITIALIZED: BAD (contains garbage)
    int* safe = nullptr; // good practice

    // passing a pointer lets the function modify the original variable
    incrementByPointer(&value); // pass the address of value
    std::cout << "After incrementByPointer(&value), value = " << value << std::endl;

    // swap example:
    int a = 1, b = 2;
    swapByPointer(&a, &b);
    std::cout << "After swapByPointer, a = " << a << ", b = " << b << std::endl;

    // pointers and arrays:
    int nums[] = {5, 10, 15};
    int* pArr = nums; // arrays "decay" to pointer to first element - for readability we prefer int* pArr = &nums[0];
    std::cout << "nums[0] via *(pArr) = " << *pArr << std::endl;
    std::cout << "nums[1] via *(pArr + 1) = " << *(pArr + 1) << " (pointer arithmetic)" << std::endl;

    // const with pointers:
    const int ci = 123;
    const int* ptrToConst = &ci; // you cannot modify *ptrToConst through this pointer
    // *ptrToConst = 456; // ERROR if uncommented

    // Best practices summary:
    // - Prefer references (&) over pointers where possible.
    // - If you must use pointers: initialize them, check for nullptr before dereferencing,
    // avoid pointer arithmetic unless necessary, and document ownership clearly.
    // - We are NOT doing dynamic memory (new/delete) here yet; when you do,
    // always make sure to DELETE the memory ONCE (see more on pointers on the PPT). 

    // --------------------------------------
    // 7.2 - enums example

    // using an unscoped enum (legacy):
    LegacyDay d = WED; // Notice: we write WED (not LegacyDay::WED)
    int d_as_int = d; // implicit conversion to int is allowed (pitfall)
    std::cout << "LegacyDay d = WED -> as int: " << d_as_int << std::endl;

    // using a scoped enum (modern, preferred):
    TrafficLight tl = TrafficLight::Yellow; // must fully qualify the enumerator.

    // switch with scoped enum (cases use fully-qualified names).
    std::cout << "TrafficLight state: ";
    switch (tl) {
        case TrafficLight::Red:    
            std::cout << "Stop" << std::endl; 
            break;
        case TrafficLight::Yellow: 
            std::cout << "Get ready" << std::endl;
            break;
        case TrafficLight::Green:  
            std::cout << "Go" << std::endl;
            break;
    }

    // if you need the underlying integer value, cast explicitly:
    std::cout << "TrafficLight::Red as int = "
              << (int)(TrafficLight::Red) << "\n";

    // Best practices:
    // - Prefer enum class for safety (names are scoped, no silent int conversions).
    // - Choose an underlying type (e.g., uint8_t(char) - 1 byte integer) when size matters (IO, packing).

    return 0; // returning 0 tells the OS the program ended successfully (EXIT_SUCCESS)
    // any other value is treated as an error
}
