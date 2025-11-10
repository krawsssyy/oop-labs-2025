#include <iostream>
#include <string>
#include <cstring>
// 0.5pt.  Implement the Book class, with the following attributes (all class attributes are private)
enum class Genre { FICTION, NONFICTION, TECHNICAL, NOVEL, OTHER };
class Book {
	private:
		// 0.2pt. All class attributes have default values
		std::string title = "";
		int numberOfEditions = 0;
		float* pricesPerEdition = nullptr;
		Genre genre = Genre::OTHER;
		static int totalBooks;
	public:
		// 0.2pt. Define the default class constructor. genre must be “OTHER”, and title must be the string “(empty)”.
		Book() { // rest of the attributes are defined in the member definition section
			this->title = "(empty)";
			// 0.1pt. Correctly handle the static attribute
			Book::totalBooks++; // increase the static attribute (counting the number of total books we have created)
		}
		// 0.5pt. Define a public interface for the pricesPerEdition array. The setter will validate the restriction previously imposed. Otherwise, it will print an error message.
		void setPricesPerEdition(float* pricesPerEdition, int numberOfEditions) {
			// validations
			if (numberOfEditions < 1) {
				std::cout << "Number of editions cannot be less than 1!" << std::endl;
				return;
			}
			if (!pricesPerEdition) {
				std::cout << "Prices array cannot be null!" << std::endl;
				return;
			}
			// ensure restriction
			for (int i = 0; i < numberOfEditions; i++) {
				if (pricesPerEdition[i] <= 0) {
					std::cout << "Prices cannot be negative or equal to zero!" << std::endl;
					return;
				}
				if (i > 0 && pricesPerEdition[i] <= pricesPerEdition[i - 1]) {
					std::cout << "Prices must be strictly increasing!" << std::endl;
					return;
				}
			}
			// if we reached here, the restrictions and validations passed, meaning that our array is correct
			this->numberOfEditions = numberOfEditions;
			if (this->pricesPerEdition) {
				delete[] this->pricesPerEdition; // cleanup for current array
			}
			// 0.3pt. The setter does not perform shallow copy.
			this->pricesPerEdition = new float[this->numberOfEditions];
			// use memcpy to copy the exact bytes from the given array to the destionation array
			memcpy(this->pricesPerEdition, pricesPerEdition, sizeof(float) * this->numberOfEditions);
		}

		float* getPricesPerEdition() const {
			if (!this->pricesPerEdition) { // ensure we don't do bad stuff if the array isn't allocated
				return nullptr;
			}
			float* cpy = new float[this->numberOfEditions];
			// 0.3pt. The getter does not perform shallow copy.
			memcpy(cpy, this->pricesPerEdition, sizeof(float) * this->numberOfEditions);
			return cpy;
		}

		// 0.4pt.Define the parametrized constructor(with all parameters) for your class.The constructor must validate the data.
		Book(std::string title, int numberOfEditions, float* pricesPerEdition, Genre genre) {
			// validate title
			if (title.length() < 3) {
				std::cout << "Title cannot have a size smaller than 3!" << std::endl;
				this->title = "(empty)";
			}
			else {
				this->title = title;
			}
			// give them default values in case the setter fails
			this->pricesPerEdition = nullptr;
			this->numberOfEditions = 0;
			this->setPricesPerEdition(pricesPerEdition, numberOfEditions);
			this->genre = genre;
			// 0.1pt. Correctly handle the static attribute.
			Book::totalBooks++;
		}

		// 0.5pt. Correctly define the class destructor.
		~Book() {
			if (this->pricesPerEdition) {
				delete[] this->pricesPerEdition;
				this->pricesPerEdition = nullptr;
			}
			Book::totalBooks--; // decrease the amount of total books
		}

		// 1pt. Implement the compactPrint() function, which will print the details of a Book object on a single line(title genre numberOfEditions total_sum_of_pricesPerEdition).
		void compactPrint() const {
			// deduce genre string
			std::string genreStr;
			switch (this->genre) {
				case Genre::FICTION:
					genreStr = "FICTION";
					break;
				case Genre::NONFICTION:
					genreStr = "NONFICTION";
					break;
				case Genre::NOVEL:
					genreStr = "NOVEL";
					break;
				case Genre::TECHNICAL:
					genreStr = "TECHNICAL";
					break;
				case Genre::OTHER:
					genreStr = "OTHER";
					break;
				default:
					std::cout << "Something really bad occured!" << std::endl;
					return;
			}

			// calculate sum + ensure the array is valid
			float sum = 0.f;
			if (this->pricesPerEdition) {
				for (int i = 0; i < this->numberOfEditions; i++) {
					sum += this->pricesPerEdition[i];
				}
			}

			// do the print
			std::cout << this->title << " " << genreStr << " " << this->numberOfEditions << " " << sum << std::endl;
		}

		// 2pt. Implement the addEdition(float newPrice) function, which will add a new edition to the dynamic array, if the new price adheres to the given restrictions.
		void addEdition(float newPrice) {
			if (newPrice <= 0) {
				std::cout << "New price cannot be negative or equal to zero!" << std::endl;
				return;
			}

			if (this->pricesPerEdition) {
				// ensure restriction
				if (this->pricesPerEdition[this->numberOfEditions - 1] >= newPrice) {
					std::cout << "New price cannot be less than the last one!" << std::endl;
					return;
				}
				// restriction valid
				// resize, since we're in the case where we already allocated the array
				float* aux = new float[this->numberOfEditions + 1];
				memcpy(aux, this->pricesPerEdition, sizeof(float) * this->numberOfEditions);
				aux[this->numberOfEditions++] = newPrice; // add new price and also increment the number of editions
				// cleanup our initial array
				delete[] this->pricesPerEdition;
				this->pricesPerEdition = aux; // move the new array into our class'
			}
			else {
				this->pricesPerEdition = new float[1];
				this->pricesPerEdition[this->numberOfEditions++] = newPrice;
			}
		}


};

int Book::totalBooks = 0; // initialize static attribute

int main() {
	// 0.5pt. Test the constructor in the main function, by creating a sample object with it
	Book b1;

	// 0.2pt.Test the previous setter in main.
	float* setTestArr = new float[3];
	setTestArr[0] = 1.f; setTestArr[1] = 2.f; setTestArr[2] = 3.f; // we cannot initialize dynamic arrays using { }, as in the case of static arrays
	// so we do this manully for the test, or use a for loop
	b1.setPricesPerEdition(setTestArr, 3);
	// 0.2pt. Test the previous getter in main and correctly handle the memory.
	float* getTestArr = b1.getPricesPerEdition();
	// ensure match
	bool ok = true;
	for (int i = 0; i < 3; i++) {
		if (setTestArr[i] != getTestArr[i]) {
			ok = false;
			break;
		}
	}
	if (ok) {
		std::cout << "Arrays match!" << std::endl;
	}
	else {
		std::cout << "Arrays don't match!" << std::endl;
	}
	// cleanup
	// since we copied the array in the set inside our class' attribute, we can now safely delete this one, since the class has a safe copy
	delete[] setTestArr;
	setTestArr = nullptr;
	// since the getter gives us a dynamic array, we are resposible for cleaning it up
	delete[] getTestArr;
	getTestArr = nullptr;

	// 0.5pt. Correctly test the constructor in main.
	float* ctorArr = new float[4];
	ctorArr[0] = 1.f; ctorArr[1] = 3.f; ctorArr[2] = 5.f; ctorArr[3] = 7.f;
	std::string title = "Sunrise on the Reaping";
	Book b2(title, 4, ctorArr, Genre::FICTION);
	// cleanup
	// the constructor uses the deep copy for the prices array, so we can safely clean up the array, avoiding double-frees
	delete[] ctorArr;
	ctorArr = nullptr;

	// 0.5pt. Test the previous functionality in main, by printing the details of at least 2 books.
	b1.compactPrint();
	b2.compactPrint();

	// 1pt. Test the previous functionality in main for at least 2 books (1 correct + 1 incorrect).
	b1.addEdition(-2.0f);
	b2.addEdition(9.f);
	b2.compactPrint();

	return 0;
}
