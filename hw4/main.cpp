#include <iostream>
#include <limits> // std::numeric_limits for robust ignore
#include <string> // for std::getline
#include <sstream> // for std::stringstream

struct Edge { // it is always a good practice to have a default value for your members
	// without initializing, they would contain garbage values from the memory
	// if some issue occurs somewhere somehow, we would have garbage values
	// but if we initialize them, this ensure we at least get something valid
	int startNode = 0;
	int endNode = 0;
};

struct Node {
	int idx = 0;
	int degree = 0;
	Edge* edges = nullptr;
};

int getDegree(int* row, int size) { // helper function to get the degree of a node
	int degree = 0;
	for (int i = 0; i < size; i++) {
		if (row[i]) { // equivalent to if row[i] != 0
			degree++;
		}
	}
	return degree;
}

void printMenu() { // helper function for printing the menu
	std::cout << std::endl << "1. List all nodes and their info" << std::endl;
	std::cout << "2. Get info about a node" << std::endl;
	std::cout << "3. Check if an edge exists" << std::endl;
	std::cout << "4. Exit" << std::endl;
	std::cout << "Enter option: ";
}

enum MENU_OPT {LIST_NODES = 1, GET_NODE, CHECK_EDGE, EXIT}; // declare an enum as a helper for the menu options
// using = 1 for the first value tells it to start numbering the values from 1
// we esentially create aliases for numbers, giving them a more special meaning inside our program
// be careful when using plain enums
// in this case, we needed the values to actually represent numbers and be able to be successfully used as such
// but in more general cases, we would use "enum class", since it is the safer and better option for this
// plain enums get implicitly converted to int, and therefore may overlap with other unrelated ints
// also, the aliases get placed in the current scope (don't need MENU_OPT:: access), and may collide with other names
// in our case, these values won't ever change, and we can ensure that all usages of these values are correct, and I wanted to showcase it
// for your bigger projects, use "enum class"

void printNode(Node n) { // helper function for printing a node
	std::cout << std::endl << " Node " << n.idx + 1 << std::endl; // 1-index the node
	std::cout << "  Degree " << n.degree << std::endl;
	std::cout << "  Edges ";
	for (int i = 0; i < n.degree; i++) {
		std::cout << "(" << n.edges[i].startNode + 1 << ", " << n.edges[i].endNode + 1 << ")" << (i != n.degree - 1 ? ", " : ""); // 1-index the edges as well
		// print the comma only if we're not at the end
	}
	std::cout << std::endl;
}

int main() {
	int n, exitCode = 0;
	Node* nodes = nullptr; // since we use goto, it would've bypassed the creation of dynamic memory (in this case the nodes array), and C++ does not allow it
	// therefore, the use of goto must not circumvent any dynamic memory initialization
	// to fix this, we must initialize the variable before
	std::cout << "Enter number of nodes in the graph: "; std::cin >> n;
	if (n < 1) {
		std::cout << "Number of nodes must be at least 1!" << std::endl;
		return 1; // exit if invalid number of nodes
		// nothing was yet allocated with "new", so we can just exit
	}

	int** adjMatrix = new int*[n]; // correctly initialize matrix
	for (int i = 0; i < n; i++) {
		adjMatrix[i] = new int[n];
	}

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore leftover newline from previous input (>>)
	std::string line;
	int readIdx = 0;

	while (readIdx < n) {
		std::cout << "Enter line " << readIdx + 1 << " (values separated by space) : ";
		std::getline(std::cin, line);
		std::stringstream ss(line); // define a stringstream and initialize it with our string
		// a string stream allows us to use our string as a stream (similar to cin/cout, which are output streams, or ifstream/ofstream, which are streams for input/output from a file)
		// it buffers the string and allows us to perform various streaming operations:
		// such as reading different values from it with >>, appending to it with << or using it with getline
		int lineIdx = 0; // index for the value in the current line
		int intVal;
		while (ss >> intVal) { // or std::getline(ss, val, ' ')), which continuously splits our given line by space and save the value in an std::string "val"
			// after that, we could've used stoi - string to integer, to convert it to an int
			// in our case, we have declared our intVal as an integer, so it will attempt to read an integer from the stringstream
			// since >> stops at space, it is perfect for our usecase, as it will consume the values in there, split by space, 1 by 1
			// and it will automatically convert them to int
			if (intVal != 0 && intVal != 1) {
				std::cout << "Wrong value for the adjacency matrix! It can only be 1 or 0." << std::endl;
				exitCode = 1;
				goto CLEANUP; // mark the error and jump to cleanup, skipping everything else
			}
			if (lineIdx >= n) { // if too many values
				std::cout << "Entered too many values for row " << readIdx + 1 << std::endl;
				exitCode = 1;
				goto CLEANUP; // mark the error and jump to cleanup
			}
			adjMatrix[readIdx][lineIdx++] = intVal; // first uses the value of lineIdx to index the matrix, and after that it increases its value by 1
		}
		if (lineIdx != n) { // ensure we have the correct amount of values
			// used != n since at the last read, lineIdx is n - 1, and as we do ++ on it, it becomes n
			std::cout << "Entered too few values for row " << readIdx + 1 << std::endl;
			exitCode = 1;
			goto CLEANUP;
		}
		readIdx++;
	}

	std::cout << "Got matrix:" << std::endl; // print matrix to ensure everything went fine
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			std::cout << adjMatrix[i][j] << " ";
		}
		std::cout << std::endl;
	}

	nodes = new Node[n]; // convert to list of nodes
	// had we also declared it here instead of just allocating it, our previous use of goto would've lead to this never being allocated
	// which is not allowed in C++, and hence we had to declare it somewhere before we used goto, to ensure that everything is properly initialized when jumping around
	for (int i = 0; i < n; i++) {
		nodes[i].idx = i;
		nodes[i].degree = getDegree(adjMatrix[i], n);
		nodes[i].edges = new Edge[nodes[i].degree];
		for (int j = 0, k = 0; j < n; j++) { // in the initializer statement of the for loop we can declare whatever variables we want
			// in this case, we have also declared k, which will be the index of the of the edge
			if (adjMatrix[i][j]) { // if edge
				nodes[i].edges[k++] = { i, j }; // assign the edge
				// since the underlying type is Edge, we can directly instantiate one using the brackets
			}
		}
	}


	int menuOption;
	while (true) { // enter the main loop of the application
		// it runs endlessly until the user exits
		printMenu();
		std::cin >> menuOption;
		if (std::cin.fail()) { // if our read failed (e.g. the user entered characters instead of numbers)
			// we need this since we are in an infinite loop, and reading a wrong value would cause the menu to be printed endlessly
			std::cin.clear(); // clear input stream
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // remove lingering '\n' that might interfere with next input
			std::cout << "Invalid input! Please enter a number." << std::endl;
			continue; // skip the remaining code and jump to the next step of the loop
			// ideally, to ensure a correct error message each time, we'd want to have this for each read
			// the checks in place already prevent bad values, but the error messages may be misleading
		}

		if (menuOption == MENU_OPT::LIST_NODES) { // good practice to also provide the type of the enum, to ensure no naming conflicts
			for (int i = 0; i < n; i++) {
				printNode(nodes[i]);
			}
		}
		else if (menuOption == MENU_OPT::GET_NODE) {
			int nodeToRead;
			std::cout << std::endl << "Enter the index of the node: "; std::cin >> nodeToRead;
			nodeToRead--; // since nodes are 1-indexed, we make it 0-indexes for use in our program
			// equivalent to nodeToRead = nodeToRead - 1;
			if (nodeToRead < 0 || nodeToRead > n - 1) { // ensure correct bounds
				std::cout << "Invalid node number! Try again." << std::endl;
			}
			else {
				printNode(nodes[nodeToRead]);
			}
		}
		else if (menuOption == MENU_OPT::CHECK_EDGE) {
			int startIdx, endIdx;
			std::cout << std::endl << "Enter starting edge: "; std::cin >> startIdx;
			std::cout << "Enter end edge: "; std::cin >> endIdx;
			startIdx--; // 0-index
			endIdx--; // 0-index
			if (startIdx < 0 || startIdx > n - 1 || endIdx < 0 || endIdx > n - 1) { // ensure correct values for both
				std::cout << "Invalid edge indices! Try again." << std::endl;
			}
			else {
				bool edgeExists = false;
				for (int i = 0; i < nodes[startIdx].degree; i++) {
					if (nodes[startIdx].edges[i].endNode == endIdx) {
						edgeExists = true;
						break;
					}
				}
				// since we have imposed no checks to ensure the graph is undirected (i.e. edge from nodeA to nodeB implies that there's an edge from nodeB to nodeA)
				// we only check the edges starting from the given node

				if (edgeExists) {
					std::cout << std::endl << "Edge between nodes " << startIdx + 1 << " and " << endIdx + 1 << " exists!" << std::endl; // 1-index
				}
				else {
					std::cout << std::endl << "Edge between nodes " << startIdx + 1 << " and " << endIdx + 1 << " doesn't exist!" << std::endl; // 1-index
				}
			}
		}
		else if (menuOption == MENU_OPT::EXIT) {
			std::cout << std::endl << "Good bye!" << std::endl;
			break; // break out of the loop, essentially terminating it
		}
		else {
			std::cout << std::endl << "Invalid option! Try again." << std::endl;
		}
	}

CLEANUP: // cleanup label for when we error
	if (nodes) {
		// since there's a chance that we error and bypass the allocation of nodes, we need to ensure it is allocated before we de-allocate
		for (int i = 0; i < n; i++) {
			if (nodes[i].edges) {
				delete[] nodes[i].edges;
				nodes[i].edges = nullptr;
			}
		}
		delete[] nodes;
		nodes = nullptr;
	}

	for (int i = 0; i < n; i++) {
		if (adjMatrix[i]) {
			delete[] adjMatrix[i];
			adjMatrix[i] = nullptr;
		}
	}
	delete[] adjMatrix;
	adjMatrix = nullptr;

	return exitCode;
}