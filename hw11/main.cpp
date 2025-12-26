#include <iostream>
#include <fstream>
#include <random>
#include <cstring>
#include <cmath> // for sqrt

struct Position { // helper struct for traversing the grid, via a linked list
	int x, y;
	Position* next;
};

class Server {
	private:
		int size = 0;
		char** grid = nullptr;
		std::string filename;

		bool hasValidPath(int finishX, int finishY) { // dfs

			// keep a bool array to save positions where we've been
			bool** visited = new bool*[this->size];
			for (int i = 0; i < size; i++) {
				visited[i] = new bool[this->size];
				memset(visited[i], 0, sizeof(bool) * this->size);
			}

			// manual stack implementation using linked positions
			Position* list = nullptr;

			// push initial position
			Position* startPos = new Position{ 1, 1, list };
			list = startPos;
			bool foundPath = false;

			// while there are still positions we haven't visited or while a path is not yet found
			while (list != nullptr && !foundPath) {
				Position* current = list; // get current position
				list = list->next; // advance list of positions
				int x = current->x;
				int y = current->y;

				// cleanup
				delete current;

				// skip position if out of bounds or on invalid terrain or already visited - do it first to ensure next checks are within bounds
				if (x < 1 || x >= this->size - 1 || y < 1 || y >= this->size - 1)
					continue;
				if (visited[x][y] || this->grid[x][y] == '#' || this->grid[x][y] == 'M')
					continue;

				visited[x][y] = true;

				if (x == finishX && y == finishY) {
					foundPath = true;
					break;
				}

				// all adjacent positions to our current one
				Position* positions[] = {
					new Position{x + 1, y, nullptr},
					new Position{x - 1, y, nullptr},
					new Position{x, y + 1, nullptr},
					new Position{x, y - 1, nullptr}
				};

				// link and add them to the list
				for (int i = 0; i < 4; i++) {
					positions[i]->next = list;
					list = positions[i];
				}
			}

			// cleanup
			while (list != nullptr) {
				Position* temp = list;
				list = list->next;
				delete temp;
			}
			for (int i = 0; i < size; i++) {
				delete[] visited[i];
				visited[i] = nullptr;
			}
			delete[] visited;
			visited = nullptr;

			return foundPath;
		}

		
		void generateMap() {
			if (this->grid) { // cleanup previous grid if it exists
				for (int i = 0; i < this->size; i++) {
					delete[] this->grid[i];
					this->grid[i] = nullptr;
				}
				delete[] this->grid;
				this->grid = nullptr;
			}
			this->grid = new char* [this->size];
			for (int i = 0; i < this->size; i++) {
				this->grid[i] = new char[this->size];
				memset(this->grid[i], '.', this->size); // initialize the floor everywhere, and then add walls and others later
			}
			this->grid[1][1] = 'P'; // mark starting position
			for (int i = 0; i < this->size; i++) {
				this->grid[i][0] = '#'; // add walls surrounding the map
				this->grid[0][i] = '#';
				this->grid[this->size - 1][i] = '#';
				this->grid[i][this->size - 1] = '#';
			}

			srand(time(NULL)); // initialize and seed random generator with the current time
			int finishPosX = 1 + (rand() % (this->size - 2)); // generate a random number between 0 and size - 3, and add 1 to it, to be between 1 and size - 2, as to avoid corners
			int finishPosY = 1 + (rand() % (this->size - 2));
			if (finishPosX == 1 && finishPosY == 1) { // regenerate them in case they match the starting position
				finishPosX = 1 + (rand() % (this->size - 2));
				finishPosY = 1 + (rand() % (this->size - 2));
			}
			this->grid[finishPosX][finishPosY] = 'T';

			for (int i = 1; i < this->size - 1; i++) { // avoid changing the outer walls
				for (int j = 1; j < this->size - 1; j++) {
					if (!(i == 1 && j == 1) && !(i == finishPosX && j == finishPosY)) { // ensure we don't change the start and end positions
						int terrain = rand() % 5;
						if (terrain == 2) { // 3:1 ratio for floors against walls
							// randomly chosen value
							this->grid[i][j] = '#';
						}
						else if (terrain == 4) { // another 3:1 ratio for floors, but against monsters
							this->grid[i][j] = 'M'; 
						}
					}
				}
			}

			if (!this->hasValidPath(finishPosX, finishPosY)) {
				this->generateMap();
			}
		}

		void serialize() {
			std::ofstream ofs(this->filename, std::ios::binary | std::ios::out | std::ios::trunc);
			// use out for creating the file if it doesnt exist and use trunc for overwriting it
			for (int i = 0; i < this->size; i++) {
				ofs.write(this->grid[i], this->size); // since sizeof(char) is 1, and for each one we have this->size elements, we have this->size bytes
			}
			ofs.close();
		}
	public:
		Server(int size, std::string filename): size(0), filename("") { // give them some default values
			if (size < 1) {
				std::cout << "Size cannot be less than 1! Set a correct size and try again." << std::endl;
			}
			else {
				if (filename.empty()) {
					std::cout << "Output filename cannot be empty! Set it correctly and try again." << std::endl;
				}
				else {
					this->size = size;
					this->filename = filename;
					this->generateMap();
					this->serialize();
				}
				
			}
		} // we only provide the size, and the server will auto generate the map

		// no default constructor, since we already give default values for them
		// and no copy constructor since the server is supposed to be unique, so no need for that

		~Server() {
			if (this->grid) {
				for (int i = 0; i < this->size; i++) {
					delete[] this->grid[i];
					this->grid[i] = nullptr;
				}
				delete[] this->grid;
				this->grid = nullptr;
			}
		}

		void setSize(int size) {
			if (size < 1) {
				std::cout << "Size cannot be less than 1! Set a correct size and try again." << std::endl;
				// don't change if wrong
			}
			else {
				this->size = size;
				this->generateMap();
				this->serialize();
			}
		}

		void setFilename(std::string filename) {
			if (filename.empty()) {
				std::cout << "Output filename cannot be empty! Set it correctly and try again." << std::endl;
			}
			else {
				this->filename = filename;
				this->serialize();
			}
		}
};

class Client {
	private:
		int size = 0;
		char** grid = nullptr;
		const int startX = 1;
		const int startY = 1;
		std::string filename;

		void deserialize() {
			std::ifstream ifs(this->filename, std::ios::binary | std::ios::ate); // std::ios::ate points the file position pointer at the end already, for usage for tellg
			if (!ifs.is_open()) {
				throw "File used in deserialization is not available!";
			}

			if (this->grid) { // cleanup for previous grid if it exists
				for (int i = 0; i < this->size; i++) {
					delete[] this->grid[i];
					this->grid[i] = nullptr;
				}
				delete[] this->grid;
				this->grid = nullptr;
			}
			
			int fileSize = ifs.tellg(); // determine filesize (determine the byte offset of the file pointer - since we used "ate", that's at the end, so it gives us the filesize)
			this->size = (int)(sqrt(fileSize / 1)); // determine size of grid
			// sizeof(char) = 1

			this->grid = new char* [this->size];
			for (int i = 0; i < this->size; i++) {
				this->grid[i] = new char[this->size];
			}

			ifs.seekg(0); // set file position pointer back to 0 (since ate put it at the end)

			for (int i = 0; i < this->size; i++) {
				ifs.read(this->grid[i], this->size);
			}
			ifs.close();
		}

		void printMap() const { // helper for printing the map
			std::cout << std::endl;
			for (int i = 0; i < this->size; i++) {
				for (int j = 0; j < this->size; j++) {
					std::cout << this->grid[i][j];
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}

		void play() {
			int score = 0;
			std::string input;
			int playerX = this->startX, playerY = this->startY;
			// main game loop
			std::cout << "Starting game..." << std::endl << std::endl;
			while (true) {
				std::cout << "Score: " << score << std::endl << std::endl;
				this->printMap();

				std::cout << std::endl << "Enter move (UP/DOWN/LEFT/RIGHT/EXIT): ";
				std::cin >> input; std::cout << std::endl;

				// save last valid position
				int prevX = playerX;
				int prevY = playerY;

				// process input
				if (input == "EXIT") {
					std::cout << std::endl << "Goodbye!";
					break;
				}
				else if (input == "UP")
					playerX--;
				else if (input == "DOWN")
					playerX++;
				else if (input == "LEFT")
					playerY--;
				else if (input == "RIGHT")
					playerY++;
				else {
					std::cout << "Invalid input! Try again." << std::endl;
					continue;
				}

				// since we ensured the map is surrounded by walls, no need for bound checks

				// check terrain conditions
				if (this->grid[playerX][playerY] == 'M') {
					std::cout << "You ran into a monster! Game over!" << std::endl;
					break;
				}
				else if (this->grid[playerX][playerY] == '#') {
					std::cout << "You cannot move into walls! Try again." << std::endl;
					playerX = prevX;
					playerY = prevY;
					continue;
				}
				else if (this->grid[playerX][playerY] == 'T') {
					std::cout << "Congratulations! You reached the treasure!" << std::endl;
					std::cout << "Final score: " << score + 1 << std::endl;
					break;
				}

				// update map
				this->grid[prevX][prevY] = '.';
				this->grid[playerX][playerY] = 'P';

				score++;
			}
		}

	public:
		Client(std::string filename) :filename("") {
			if (filename.empty()) {
				std::cout << "Output filename cannot be empty! Set it correctly and try again." << std::endl;
			}
			else {
				this->filename = filename;
				this->deserialize();
				this->play();
			}
		}

		void setFilename(std::string filename) {
			if (filename.empty()) {
				std::cout << "Output filename cannot be empty! Set it correctly and try again." << std::endl;
			}
			else {
				this->filename = filename;
				this->deserialize();
				this->play();
			}
		}
};

int main() {
	std::string filename = "map.bin";
	int size;
	std::cout << "Enter map size: "; std::cin >> size;

	Server server(size, filename);
	Client client(filename);
	return 0;
}