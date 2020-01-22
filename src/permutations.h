#ifndef PERMUTATIONS_H
#define PERMUTATIONS_H


#include "settings.h"
#include "salesman.h"


typedef unsigned long long BigInt;

typedef enum {LEFT, RIGHT} Direction;


BigInt fact(int n);


void swap(int *array, int i, int j);


// Rotates the values in the given array. Needed: start <= end.
void rotate(int *array, int start, int end, Direction dir);


// Mirror the values between start and end. Needed: start <= end.
void mirror(int *array, int start, int end);


// Printing all permutations using Heap Algorithm:
void print_Heap_rec(int *array, int size, int n);


// Showing the Heap algorithm in action:
void demo_Heap(int n);


#endif
