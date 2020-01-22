///////////////////////////////////////////////////////////////////////////////
// The following code is an implementation of the PCG Random Number Generator.
// Credit to Melissa O'Neill <oneill@pcg-random.org>
// http://www.pcg-random.org
///////////////////////////////////////////////////////////////////////////////


#ifndef PCGRAND32_H
#define PCGRAND32_H


#include <stdint.h>


// Initialize the random number generator. For a non deterministic initialization,
// one can use time(NULL) as seed:
void pcg32rand_init(uint64_t seed);


// Returns a random 32 bit unsigned integer between 0 and bound - 1.
// This version is biased, but is fast:
uint32_t pcg32rand_bounded(uint32_t bound);


// Returns a random float in [0, 1).
float pcg32rand_float(void);


#endif
