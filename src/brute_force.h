#ifndef BRUTE_FORCE_H
#define BRUTE_FORCE_H


#include "settings.h"
#include "salesman.h"


// Find the smallest path length, by brute force: every permutations of the n-1 first cities are found using
// a non recursive version of the Heap algorithm. Last city is fixed, as it is the starting/end point.
void brute_Heap(Map *map, int *best_path, Number *best_pathLength, double *time);


// Find the smallest path length, by brute force: every permutations of the n-1 first cities are found using
// the Steinhaus–Johnson–Trotter algorithm. Last city is fixed, as it is the starting/end point.
// Since this algorithm produces no permutations symmetric to each other in the first half, we can stop midway.
// Also, only consecutive transpositions are made, so one does not need to compute all the paths length anymore:
void brute_Trotter(Map *map, int *best_path, Number *best_pathLength, double *time);


#endif
