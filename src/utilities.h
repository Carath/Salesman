#ifndef UTILITIES_H
#define UTILITIES_H


#include <time.h>

#include "settings.h"


double time_elapsed(clock_t start);


// Returns a random number in [0, 1).
Number uniform_random(void);


// Compute an error ratio from a given reference:
Number compare(Number ref, Number x);


// Checks if x = y, short of EPSILON:
int epsilonEquality(Number x, Number y);


// Checks if x < y, short of EPSILON:
int epsilonStrInequality(Number x, Number y);


#endif
