#include <stdio.h>
#include <stdlib.h>

#include "permutations.h"
#include "matrix.h"


BigInt fact(int n)
{
	if (n >= 22)
	{
		printf("\n'unsigned long long' type is not big enough to hold fact(%d).\n\n", n);
		return 0;
	}

	BigInt result = 1;

	for (int i = 1; i <= n; ++i)
		result *= i;

	return result;
}


inline void swap(int *array, int i, int j)
{
	int buffer = array[i];

	array[i] = array[j];

	array[j] = buffer;
}


// Rotates the values in the given array. Needed: start <= end.
void rotate(int *array, int start, int end, Direction dir)
{
	if (dir == LEFT)
	{
		int start_value = array[start];

		for (int i = start; i < end; ++i)
			array[i] = array[i + 1];

		array[end] = start_value;
	}

	else // RIGHT
	{
		int end_value = array[end];

		for (int i = end; i > start; --i)
			array[i] = array[i - 1];

		array[start] = end_value;
	}
}


// Mirror the values between start and end. Needed: start <= end.
void mirror(int *array, int start, int end)
{
	int n = (end - start + 1) / 2; // central value isn't swapped if the number of value is odd.

	for (int i = 0; i < n; ++i)
		swap(array, start + i, end - i);
}


// Printing all permutations using Heap Algorithm:
void print_Heap_rec(int *array, int size, int n)
{
	if (size == 1)
	{
		printIntMatrix(&array, 1, n);
		return;
	}

	print_Heap_rec(array, size - 1, n); // optimization!

	for (int i = 0; i < size - 1; ++i)
	{
		int pos = size % 2 == 1 ? 0 : i;

		swap(array, pos, size - 1);

		print_Heap_rec(array, size - 1, n);
	}
}


// Showing the Heap algorithm in action:
void demo_Heap(int n)
{
	int *array = (int*) calloc(n, sizeof(int));

	// Starting from a permutation:
	for (int i = 0; i < n; ++i)
		array[i] = i;

	print_Heap_rec(array, n, n);

	free(array);
}
