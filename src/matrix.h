#ifndef MATRIX_H
#define MATRIX_H


#include "settings.h"


// Creating matrices:

int** createIntMatrix(int rows, int cols); // Every field is initialized to 0.

Number** createNumberMatrix(int rows, int cols); // Every field is initialized to 0.


// Freeing matrices:

void freeIntMatrix(int **matrix, int rows);

void freeNumberMatrix(Number **matrix, int rows);


// Printing matrices:

void printIntMatrix(int **matrix, int rows, int cols);

void printNumberMatrix(Number **matrix, int rows, int cols);


// Copying matrices:

void copyIntMatrix(int **source, int **dest, int rows, int cols);

void copyNumberMatrix(Number **source, Number **dest, int rows, int cols);


// Filling randomly a Number matrix with uniform distribution:

void randomNumberMatrix_uniform(Number **matrix, int rows, int cols, Number min, Number max);


#endif
