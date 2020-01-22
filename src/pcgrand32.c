///////////////////////////////////////////////////////////////////////////////
// The following code is an implementation of the PCG Random Number Generator.
// Credit to Melissa O'Neill <oneill@pcg-random.org>
// http://www.pcg-random.org
///////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "pcgrand32.h"


#define GEN32_USED pcg32_fast


static uint64_t const multiplier = 6364136223846793005u;
static uint64_t const increment  = 1442695040888963407u; // Or an arbitrary odd constant
static uint64_t       mcg_state  = 0xcafef00dd15ea5e5u; // Must be odd


// Regular version, with great statistical properties:
uint32_t pcg32(void)
{
	uint64_t oldstate = mcg_state;
	uint32_t rot = oldstate >> 59u; // 59 = 64 - 5

	mcg_state = oldstate * multiplier + increment;
	oldstate ^= oldstate >> 18u; // 18 = (64 - 27)/2

	uint32_t xorshifted = oldstate >> 27u; // 27 = 32 - 5

	return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}


// A bit weaker than the regular pcg32() version, but faster:
uint32_t pcg32_fast(void)
{
	uint64_t oldstate = mcg_state;
	uint32_t rot = oldstate >> 61u; // 61 = 64 - 3

	mcg_state = oldstate * multiplier;
	oldstate ^= oldstate >> 22u;

	return (uint32_t) (oldstate >> (22 + rot)); // 22 = 32 - 3 - 7
}


// Initialize the random number generator. For a non deterministic initialization,
// one can use time(NULL) as seed:
void pcg32rand_init(uint64_t seed)
{
	mcg_state = 2 * seed + 1;
	GEN32_USED();
}


// Returns a random 32 bit unsigned integer between 0 and bound - 1.
// This version is biased, but is fast:
uint32_t pcg32rand_bounded(uint32_t bound)
{
	return GEN32_USED() % bound;
}


// Returns a random float in [0, 1).
float pcg32rand_float(void)
{
	return (float) GEN32_USED() / UINT_MAX; // UINT_MAX > RAND_MAX !!!
}


/////////////////////////////////////////////////////////////////////////
// Demo:
/////////////////////////////////////////////////////////////////////////

// #include <time.h>


// int main(void)
// {
// 	pcg32rand_init(time(NULL));

// 	for (int i = 0; i < 10; ++i)
// 	{
// 		printf("%d\n", pcg32rand_bounded(13));
// 	}

// 	return 0;
// }
