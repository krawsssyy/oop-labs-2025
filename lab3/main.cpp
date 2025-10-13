#define _CRT_SECURE_NO_WARNINGS // for using regular "str*" functions rather than the "_s" ones Visual Studio recommends
#include <iostream> // for std::cin, std::cout
#include <string> // for std::string-related functionalities

/* 1. Write a function that takes in a sentence(as char*), splits it into words and displays the amount of vowels per word(words split by spaces),
effectively creating a frequency vector for the vowels, per word.The sole input is a pointer.
*/
void clearArray(int *arr, int size) {
	// in here, we passed the array as a pointer
	// therefore, we will be able to modify the array itself, rather than having a copy when passed by value (int arr[], ...)
	// since arrays decay to pointers to their first element, we only pass the array normally and like this we can modify it
	// since we can't add references to array as parameters, you can consider this pass-by-reference for arrays
	// for dynamic arrays, we can only get the size of them when they are char arrays
	// for others, we need to save the size and pass it everywhere
	for (int i = 0; i < size; i++)
		arr[i] = 0;
	// small helper for clearing out the array
}

void vowelsFrequencyPerWord(char* input) {
	int freqArr[128] = { 0 }; // initialize the array with all zeros
	// giving the array size 128 since printable characters on the ASCII table are in the range 32 (0x20) - 127(0x7f)
	// though letters will be lower, we can keep it that way
	char* currentWord = strtok(input, " "); // splits the input by the given delimiter, and returns the first part (called a token - strtok)
	// subsequent calls will use NULL as the input string, and will return the next tokens until reaching the end of the string
	while (currentWord) { // equivalent to currentWord != nullptr
		for (int i = 0; i < strlen(currentWord); i++) {
			if (strchr("aeiouAEIOU", currentWord[i])) // strchr checks if the second parameter (a single char) is found in the first (a char array)
				freqArr[currentWord[i]]++; // increment the entry at the position represented by the character
			// characters are a single byte, and only have a special meaning when being represented as characters (see ASCII table)
			// otherwise, they are just integers in the range 0-255
		}
		
		std::cout << "For word \"" << currentWord << "\", we have:" << std::endl;
		// if we want to have double-quotes inside our cout, they need to be escaped (use a \ before them, as I did)
		for (int i = 32; i < 128; i++) { // since our printable characters start from 32, we can start our loop from there
			if (strchr("aeiouAEIOU", i)) { // do the same trick, but in reverse
				// we now represent i as a character and check whether it represnts vowels when converted to a char
				std::cout << (char)i << " = " << freqArr[i] << std::endl;
				// implicitly cast i as a chaar such that cout will actually print the character with that value, rather than printing the integer
			}
		}
		currentWord = strtok(nullptr, " ");
		clearArray(freqArr, 128); // simple helper for clearing out the array after each word
	}
}

/*
2. Write a function that takes in a string (std::string) and converts to it bird speech ("pasareasca").
After each vowel, the letter "p" followed by that vowel is added (for eg., "bine" => "biPInePE"). Then, the new string is returned.
*/

std::string convertToBirdSpeech(const std::string& input) {
	// since we return a new string, we can leave our input as const and not modify it (and reference such that no copy occurs)
	// a std::string datatype is a more flexible char* implementation, allowing more and easier operations on it
	// it being an array, we can access each element (letter) via the "[]" operator, just as in the case of char*
	// inner elements of a std::string are of the type char
	std::string result = ""; // initialize our return string
	std::string vowels = "aeiouAEIOU";
	for (int i = 0; i < input.length(); i++) { // std::string.length() is the equivalent of strlen for char*
		result += input[i]; // result = result + input[i]
		result += vowels.find(input[i]) != std::string::npos ? std::string("P") + input[i] : ""; // ternary operator - simplified if statement
		// condition ? expression_if_true : expression_if_false;
		// std::string.find() is the equivalent of strchr for std::strings
		// this will be equivalent to
		/*
		if (vowels.find(input[i]) != std::string::npos) { // instead of checking against nullptr, we now check against std::string::npos
			result = result + (std::string("P") + input[i]); // we converted "P" (const char*) to std::string via its constructor
			// such that we can use the "+" operator, which is concatenation (equivalent of strcat for char*)
			// std::string + char works, but const char* + char doesn't (at least one needs to be std::string for + to work)
		}
		else {
			result = result + ""; // don't add anything else
		}
		*/
	}
	return result;
}

int main() {

	// 1.
	char* input = new char[256]; // declaring a char array as a pointer and allocating memory using "new"
	std::cout << "Please input string for first requirement: ";
	std::cin.getline(input, 256); // read max size of our array, if input is longer it will be discarded
	// older style of std::cin.getline()
	// we use getline because using regular "std::cin >>" wouldn't work, as it stops after the first space
	// so, if we'd like to read with spaces, we either use std::cin.get() or std::cin.getline()
	// the difference between the two is that std::cin.getline() discards the newline character inserted when pressing enter and doesn't send it to the string
	// whereas std::cin.get() will insert that newline character into our input string
	vowelsFrequencyPerWord(input);
	delete[] input; // since we have allocated our char array using "new", we need to delete it
	// since it is a pointer to an array, we have to use "delete[]" rather than "delete"
	input = nullptr;
	// this is a safety measure to avoid dangling pointers
	// dangling ptrs are a security vulnerability where unused pointers are not nulled out and may leak sensible data in your program

	// 2.
	std::string inputStr;
	std::cout << "Please input string for the second requirement: ";
	std::getline(std::cin, inputStr); // more modern variant of getline, using the one exposed by the "std" namespace
	// instead of std::cin.getline(inputBuffer, maxSize)
	// we do std::getline(inputStream, inputBuffer) (where most of the times inputStream is std::cin (console input))
	// we can also pass other streams here, such as file streams, but those will be later and we'll talk about them when we get there
	// (or string streams)
	// it doesn't require a max size, it will read as much characters as we give it
	std::cout << "String to bird speech is: \"" << convertToBirdSpeech(inputStr) << "\"" << std::endl;

	return 0;
}