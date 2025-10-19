#include <iostream>
#include <iomanip> // for std::setprecision

// helper function for allocating a dynamic matrix, since we'll do it multiple times
float** allocateMatrix(int rows, int cols) {
    float** matrix = new float* [rows];
    // as per the pointers presentation given, when we want a matrix, we use **
    // esentially we will have an array of *, in which each * variable points to an array of the underlying type (in this case, float)
    // something like
    // float** matrix = new float* [rows]; // { ptr_to_arr_0, ptr_to_arr_1, ...., ptr_to_arr_rows }
    // therefore creating a 2d array, a matrix
    for (int i = 0; i < rows; i++) {
        matrix[i] = new float[cols];
        // ptr_to_arr_i = { element_of_arr_i_0, element_of_arr_i_1, ..., element_of_arr_i_cols}
        memset(matrix[i], 0, cols * sizeof(float)); // initialize all elements with 0
        // SYNTAX: memset(destination_as_ptr, value_as_int, size_in_bytes)
        // - it sets the given value for the amount of bytes given at the specified memory address
        // beware: the size is in bytes, therefore we need to specify exactly how many bytes
        // we will have num of values * sizeof(type_of_array)
        // as sizeof returns the size in bytes of the type, therefore calculating the correct amount of bytes
    }
    // and now, at the end, we have
    // matrix = { { element_of_arr_0_0, element_of_arr_0_1, ..., element_of_arr_0_cols}, ..., { element_of_arr_rows_0, element_of_arr_rows_1, ..., element_of_arr_rows_cols } }
    // effectively obtaining a matrix of size rows x cols
    return matrix;
}

// helper function for dynamically allocating a matrix, since we'll do it multiple times
void deallocateMatrix(float** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        // when deleting, we do the same as in allocationg
        // we delete each row, which is a pointer to an array
        // it being a pointer to an array, we use delete[]
        delete[] matrix[i];
        matrix[i] = nullptr; // don't forget this!
        // so, in essence, we delete each of the ptr_to_arr_i from our initial array of pointers
    }
    delete[] matrix; // now we delete the original array, one which is an array of pointers
    matrix = nullptr; // and we also null this one out, preventing any data leakage
}

// actual function for multiplying matrices
float** multiplyMatrices(float** A, int rowsA, int colsA, float** B, int rowsB, int colsB, int& resultRows, int& resultCols) {
    // we use references for resultRows and resultCols as we want to get their values back in main, such that we can print our matrix there and know its correct size
    // set result dimensions and allocate result matrix
    resultRows = rowsA;
    resultCols = colsB;
    float** result = allocateMatrix(resultRows, resultCols);
    if (result == nullptr) {
        std::cout << "Allocation failed for result!" << std::endl;
        return nullptr;
    }

    // perform the multiplication algorithm
    // result[i][j] = sum from k = 1 to k = colsA  of A[i][k] * B[k][j] (1-indexed)
    // when moved to 0-indexed as we have in arrays, it looks as such:
    for (int i = 0; i < resultRows; i++) {
        for (int j = 0; j < resultCols; j++) {
            // no need to initialize the sum as when allocating the arrays we already initialize them with the default value (0)
            // otherwise, we would've done here: result[i][j] = 0.0f; // the "f" there signifies that the value is a float
            // otherwise, 0.0 is treated as a double and there would've been a small conversion happen internally to make it float
            for (int k = 0; k < colsA; k++) { // do sum
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return result;
}

int main() {
    int rowsA, colsA, rowsB, colsB;
    std::cout << "Enter dimensions for the first matrix" << std::endl;
    std::cout << "Rows: "; std::cin >> rowsA;
    std::cout << "Columns: "; std::cin >> colsA;
    std::cout << "Enter dimensions for the second matrix" << std::endl;
    std::cout << "Rows: "; std::cin >> rowsB;
    std::cout << "Columns: "; std::cin >> colsB;

    // check for multiplication condition
    // this also doubles as a bounds check, as if rowsB < colsA, we will have an issue where we read from matrixB out-of-bounds
    // always please ensure that you work within the bounds of your arrays by any means necessary, no matter if they are dynamic or static
    bool isNotValid = colsA != rowsB; // you can initialize bools directly with conditions
    if (isNotValid) {
        std::cout << std::boolalpha << "Matrix dimensions incompatible for multiplication, since (colsA != rowsB) = " << isNotValid << std::endl;
        // std:boolalpha tells std::cout to print booleans as true/false, rather than 1/0
        // no need to deallocate anything in this case since nothing was allocated yet
        return 1; // any exit code from main different from 0 signifies failure
    }

    // allocate and read matrices
    float** matrixA = allocateMatrix(rowsA, colsA);
    if (matrixA == nullptr) { // good check to ensure that some allocations didn't fail
        // it is always necessary in practice to check your pointers before using them
        std::cout << "Allocation failed for first matrix!" << std::endl;
        return 1;
        // no need to deallocate anything in this case since nothing was allocated (correctly)
    }
    float** matrixB = allocateMatrix(rowsB, colsB);
    if (matrixB == nullptr) {
        std::cout << "Allocation failed for second matrix!" << std::endl;
        deallocateMatrix(matrixA, rowsA);
        // now we need to deallocate the first matrix since it was correctly allocated before
        return 1;
    }
    std::cout << "Enter elements for first matrix (" << rowsA << "x" << colsA << "):" << std::endl;
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsA; j++) {
            std::cout << "Element [" << i << "][" << j << "]: ";
            std::cin >> matrixA[i][j];
        }
    }
    std::cout << "Enter elements for second matrix (" << rowsB << "x" << colsB << "):" << std::endl;
    for (int i = 0; i < rowsB; i++) {
        for (int j = 0; j < colsB; j++) {
            std::cout << "Element [" << i << "][" << j << "]: ";
            std::cin >> matrixB[i][j];
        }
    }

    int resultRows = -1, resultCols = -1; // give them unattainable values as initial values
    // not really necessary but good practice to always initialize your variables, no matter the type
    float** result = multiplyMatrices(matrixA, rowsA, colsA, matrixB, rowsB, colsB, resultRows, resultCols);
    // display result if successful
    if (result != nullptr) { 
        std::cout << "First * Second = " << std::endl;
        for (int i = 0; i < resultRows; i++) {
            for (int j = 0; j < resultCols; j++) {
                std::cout << std::fixed << std::setprecision(2) << result[i][j] << " "; // show float numbers with exactly 2 decimal points
            }
            std::cout << std::endl;
        }
        deallocateMatrix(result, resultRows); // if successful, cleanup after print
        // since the function created a pointer and didn't deallocate it at the end, it is up to us now to deallocate it
        // when we're done with it, so make sure you do that every single time
        // as soon as you're done with a pointer, deallocate it (correctly)
        // be very aware of all the pointers you create (whether in main or in other functions), and track them
        // carefully across your program, such that you don't have memory leaks and potential issues due to them
        // always initialize them when declaring, and be careful of the actions you do with them
        // as soon as you're done, clean it up and move on, without ever using it again
    } // if it is not successful, we have printed the error inside the multiplication function, so we just skip stuff in here

    // cleanup for the input matrices - needs to be done regardless of the result of the multiplication
    deallocateMatrix(matrixA, rowsA);
    deallocateMatrix(matrixB, rowsB);

    return 0;
}