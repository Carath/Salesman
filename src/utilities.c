#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "utilities.h"


double time_elapsed(clock_t start)
{
	return (double) (clock() - start) / CLOCKS_PER_SEC;
}


// Returns a random number in [0, 1).
Number uniform_random(void)
{
	return (Number) rand() / RAND_MAX;
}


// Compute an error ratio from a given reference:
Number compare(Number ref, Number x)
{
	Number comparison = (x - ref) / ref;

	return comparison;
}


// Checks if x = y, short of EPSILON:
int epsilonEquality(Number x, Number y)
{
	return fabs(x - y) < EPSILON;
}


// Checks if x < y, short of EPSILON:
int epsilonStrInequality(Number x, Number y)
{
	return x + EPSILON < y;
}
