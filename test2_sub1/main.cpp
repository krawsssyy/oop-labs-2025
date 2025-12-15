#include <iostream>
#include <string>

enum class WebsiteType { NEWS, BLOG, TECHNICAL, ECOMMERCE, GENERAL };

class WebsiteReport {
	private:
		std::string title;
		int noDays;
		int* visitorsPerDay;
		WebsiteType type;
		static int totalReports;

		bool isArrayValid(int* arr, int size) { // small helper for checking restriction
			if (!arr || size < 1) {
				return true;
			}
			for (int i = 0; i < size; i++) {
				if (arr[i] < 0) {
					return false;
				}
			}
			return true;
		}
	public:
		// default constructor
		WebsiteReport() {
			this->title = "(empty)";
			this->noDays = 0;
			this->visitorsPerDay = nullptr;
			this->type = WebsiteType::GENERAL;
			totalReports++; // increment the static attribute
		}

		// parametrized constructor for all parameters
		WebsiteReport(std::string title, int noDays, int* visitorsPerDay, WebsiteType type) {
			if (title.length() < 3) {
				std::cout << "The title cannot have a length less than 3!" << std::endl;
				this->title = "";
			}
			else {
				this->title = title;
			}

			bool isArrValid = true; // use a flag to avoid code duplication
			if (noDays < 1) {
				std::cout << "The number of days for the report cannot be less than 1!" << std::endl;
				isArrValid = false;
			}
			else {
				if (!visitorsPerDay) {
					std::cout << "The array of visitors cannot be empty!" << std::endl;
					isArrValid = false;
				}
				else {
					if (!this->isArrayValid(visitorsPerDay, noDays)) {
						std::cout << "Given array does not respect restrictions!" << std::endl;
						isArrValid = false;
					}
					else {
						this->noDays = noDays;
						this->visitorsPerDay = new int[this->noDays];
						memcpy(this->visitorsPerDay, visitorsPerDay, this->noDays * sizeof(int));
					}

				}
			}

			if (!isArrValid) {
				this->noDays = 0;
				this->visitorsPerDay = nullptr;
			}
			this->type = type;
			totalReports++; // increment the static attribute
		}

		// copy constructor
		WebsiteReport(const WebsiteReport& other) {
			this->title = other.title;
			this->noDays = other.noDays;
			if (!this->noDays) { // check whether the array is allocated in the other object
				// no need to validate anything else, since that object was already validated on creation
				this->visitorsPerDay = nullptr;
			}
			else {
				this->visitorsPerDay = new int[this->noDays];
				memcpy(this->visitorsPerDay, other.visitorsPerDay, this->noDays * sizeof(int));
			}
			this->type = other.type;
			totalReports++;
		}

		~WebsiteReport() {
			if (this->visitorsPerDay) {
				delete[] this->visitorsPerDay;
				this->visitorsPerDay = nullptr;
			}
		}

		int* getVisitorsPerDay() const {
			if (!this->visitorsPerDay) {
				return nullptr;
			}
			else {
				int* copy = new int[this->noDays];
				memcpy(copy, this->visitorsPerDay, this->noDays * sizeof(int));
				return copy;
			}
		}

		void setVisitorsPerDay(int noDays, int* visitorsPerDay) {
			if (this->visitorsPerDay) { // clear the array no matter what, so we avoid code duplication
				delete[] this->visitorsPerDay;
				this->visitorsPerDay = nullptr;
			}
			if (noDays < 1) {
				std::cout << "The number of days for the report cannot be less than 1!" << std::endl;
				this->noDays = 0;
			}
			else {
				if (!visitorsPerDay) {
					std::cout << "The array of visitors cannot be empty!" << std::endl;
					this->noDays = 0;
				}
				else {
					if (!this->isArrayValid(visitorsPerDay, noDays)) {
						std::cout << "Given array does not respect restrictions!" << std::endl;
						this->noDays = 0;
					}
					else {
						this->noDays = noDays;
						this->visitorsPerDay = new int[this->noDays];
						memcpy(this->visitorsPerDay, visitorsPerDay, this->noDays * sizeof(int));
					}
				}
			}
		}

		WebsiteReport operator*(int newValue) { // WebsiteReport * int
			WebsiteReport copyObj(*this); // make a copy and implement the functionality on it, returning this one afterwards
			// so we don't modify our current object
			if (!copyObj.visitorsPerDay) {
				copyObj.visitorsPerDay = new int[1];
				copyObj.visitorsPerDay[0] = newValue;
			}
			else {
				int* copy = new int[copyObj.noDays + 1];
				memcpy(copy, copyObj.visitorsPerDay, copyObj.noDays * sizeof(int));
				copy[copyObj.noDays] = newValue;
				delete[] copyObj.visitorsPerDay;
				copyObj.visitorsPerDay = copy;
				copyObj.noDays++;
			}
			return copyObj;
		}

		float operator()() const { // function call operator
			if (!this->visitorsPerDay) {
				return 0.f;
			}
			else {
				float sum = 0;
				for (int i = 0; i < this->noDays; i++) {
					sum += this->visitorsPerDay[i];
				}
				return sum / this->noDays;
			}
		}

		void prettyPrint() const {
			std::cout << std::endl << "Title: " << this->title << std::endl;
			std::cout << "Number of days: " << this->noDays << std::endl;
			std::cout << "Daily visitors: ";
			if (this->visitorsPerDay) {
				for (int i = 0; i < this->noDays; i++) {
					std::cout << this->visitorsPerDay[i] << " ";
				}
				std::cout << std::endl;
			}
			else {
				std::cout << "(None)" << std::endl;
			}

			std::string typeString = "";
			switch (this->type) {
				case WebsiteType::NEWS:
					typeString = "NEWS";
					break;
				case WebsiteType::BLOG:
					typeString = "BLOG";
					break;
				case WebsiteType::TECHNICAL:
					typeString = "TECHNICAL";
					break;
				case WebsiteType::ECOMMERCE:
					typeString = "ECOMMERCE";
					break;
				case WebsiteType::GENERAL:
					typeString = "GENERAL";
					break;
			}
			std::cout << "Type: " << typeString << std::endl;
		}
};

int WebsiteReport::totalReports = 0; // initialize static attribute

int main() {
	WebsiteReport a; // default constructor test
	std::cout << "obj a: ";
	a.prettyPrint(); // also test pretty print function

	int vals[] = { 10, 11, 12 };
	WebsiteReport b("test", 3, vals, WebsiteType::TECHNICAL); // parametrized constructor test
	std::cout << "obj b: ";
	b.prettyPrint();

	WebsiteReport c(a); // copy constructor test
	std::cout << "obj c: ";
	c.prettyPrint();

	WebsiteReport* d = new WebsiteReport;
	std::cout << "obj d: ";
	d->prettyPrint();
	delete d; // destructor test
	d = nullptr;

	c.setVisitorsPerDay(3, vals); // test setter

	int* vals2 = c.getVisitorsPerDay(); // test getter
	std::cout << "obj c values after setter (via getter): ";
	for (int i = 0; i < 3; i++) {
		std::cout << vals2[i] << " ";
	}
	std::cout << std::endl;

	delete[] vals2; // clean it up once we're done with it
	vals2 = nullptr;

	WebsiteReport e = c * 13; // add new element (testing operator *)
	std::cout << "obj e (c * 13): ";
	e.prettyPrint();

	std::cout << "avg values of e: " << e() << std::endl; // testing function call operator

	return 0;
}