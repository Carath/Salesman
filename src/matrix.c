#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "utilities.h"


// Creating matrices:


int** createIntMatrix(int rows, int cols) // Every field is initialized to 0.
{
	int **matrix = (int**) calloc(rows, sizeof(int*));

	if (matrix == NULL)
	{
		printf("\nImpossible to allocate enough memory for a matrix.\n\n");
		return NULL;
	}

	for (int i = 0; i < rows; ++i)
	{
		matrix[i] = (int*) calloc(cols, sizeof(int));

		if (matrix[i] == NULL)
		{
			printf("\nImpossible to allocate enough memory for matrix[%d].\n\n", i);
			return NULL;
		}
	}

	return matrix;
}


Number** createNumberMatrix(int rows, int cols) // Every field is initialized to 0.
{
	Number **matrix = (Number**) calloc(rows, sizeof(Number*));

	if (matrix == NULL)
	{
		printf("\nImpossible to allocate enough memory for a matrix.\n\n");
		return NULL;
	}

	for (int i = 0; i < rows; ++i)
	{
		matrix[i] = (Number*) calloc(cols, sizeof(Number));

		if (matrix[i] == NULL)
		{
			printf("\nImpossible to allocate enough memory for matrix[%d].\n\n", i);
			return NULL;
		}
	}

	return matrix;
}


// Freeing matrices:


void freeIntMatrix(int **matrix, int rows)
{
	if (matrix == NULL)
		return;

	for (int i = 0; i < rows; ++i)
		free(matrix[i]);

	free(matrix);
}


void freeNumberMatrix(Number **matrix, int rows)
{
	if (matrix == NULL)
		return;

	for (int i = 0; i < rows; ++i)
		free(matrix[i]);

	free(matrix);
}


// Printing matrices:


void printIntMatrix(int **matrix, int rows, int cols)
{
	if (matrix == NULL)
		return;

	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
			printf(" %d", matrix[i][j]);

		// printf("\n");
	}

	printf("\n");
}


void printNumberMatrix(Number **matrix, int rows, int cols)
{
	if (matrix == NULL)
		return;

	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
			printf("%8.2f", matrix[i][j]);

		printf("\n");
	}

	printf("\n");
}


// Copying matrices:


void copyIntMatrix(int **source, int **dest, int rows, int cols)
{
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
			dest[i][j] = source[i][j];
	}
}


void copyNumberMatrix(Number **source, Number **dest, int rows, int cols)
{
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
			dest[i][j] = source[i][j];
	}
}


// Filling randomly a Number matrix with uniform distribution:


void randomNumberMatrix_uniform(Number **matrix, int rows, int cols, Number min, Number max)
{
	if (matrix == NULL)
		return;

	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
			matrix[i][j] = RAND_FLOAT() * (max - min) + min;
	}
}
