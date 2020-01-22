#ifndef SETTINGS_H
#define SETTINGS_H


///////////////////////////////////////////////////////////////
// General declarations to be included everywhere:
///////////////////////////////////////////////////////////////


// #define Number float
#define Number double


#define EPSILON 0.000001


#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x < y ? y : x)


// Careful, don't use this inside a function:
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(*array))


///////////////////////////////////////////////////////////////
// Choice of method for the floating point strict inequality:
///////////////////////////////////////////////////////////////


#define INEQ_REGULAR // faster.
// #define INEQ_EPSILON // more numerically stable.


#if defined INEQ_REGULAR
	#define STR_INEQUALITY(x, y) \
		(x < y)

#elif defined INEQ_EPSILON
	#define STR_INEQUALITY(x, y) \
		epsilonStrInequality(x, y)
#endif


///////////////////////////////////////////////////////////////
// Choice of Random Number generator:
///////////////////////////////////////////////////////////////


// #define UNIX_RAND
#define PCG32 // Faster!


#if defined UNIX_RAND
	#define RAND_INIT(seed) \
		srand(seed)

	#define RAND_BOUNDED(bound) \
		(rand() % (bound))

	#define RAND_FLOAT \
		uniform_random

#elif defined PCG32
	#include "pcgrand32.h"

	#define RAND_INIT(seed) \
		pcg32rand_init(seed)

	#define RAND_BOUNDED(bound) \
		pcg32rand_bounded(bound)

	#define RAND_FLOAT \
		pcg32rand_float
#endif


///////////////////////////////////////////////////////////////
// Graphical settings:
///////////////////////////////////////////////////////////////


#define USE_HARDWARE_ACCELERATION 0

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define BIG_MARGIN 400
#define SMALL_MARGIN 50

#define CITY_SIZE 12

#define START_CITY_NUMBER 10
#define MIN_CITIES_NUMBER 3
#define MAX_CITIES_NUMBER_BRUTE 13

#define DIST_BOUND 20. // arbitrary.


///////////////////////////////////////////////////////////////
// Hotkeys:
///////////////////////////////////////////////////////////////

#define QUIT_KEY SDLK_ESCAPE
#define REFRESH_KEY SDLK_SPACE
#define DECR_KEY_1 SDLK_l
#define INCR_KEY_1 SDLK_m
#define DECR_KEY_10 SDLK_o
#define INCR_KEY_10 SDLK_p
#define BRUTE_KEY SDLK_b
#define GEN_KEY SDLK_g


#endif
