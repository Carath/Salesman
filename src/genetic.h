#ifndef GENETIC_H
#define GENETIC_H


#include "settings.h"
#include "salesman.h"


// N.B: No crossover is being done during the genetic search, as no natural way of doing so has been found so far.


// Empirical result: given a fixed set of parameters, experiments show that the genetic search will stagnate
// passed a remarquably stable (see benchmarks) epochs number (this number changes a bit, but not by much). It has also been observed
// that while a greater population size may help to find better solutions given enough time, using too big of a population size may
// be detrimental to the search if it hasn't been given enough time to reach the stagnation point... Therefore, if the search
// duration is being fixed, one needs to tune the population size in order to be sure that it reaches said stagnation point before
// the end of the search.


// Option of selection of an individual from the population:
// - UNIFORMLY: random selection.
// - PROBABILISTIC: an individual is chosen with a probability proportional to its fitness value.
// - BEST: the current best individual is selected. This is mainly useful to simulate a population of size 1.
typedef enum {UNIFORMLY, PROBABILISTIC, BEST} SelectionOption;


// Type of mutation used on a path:
// - SWAP_CONSECUTIVE: two randomly chosen consecutive cities are swapped.
// - SWAP_ANY: two randomly chosen cities are swapped.
// - ROTATE: a random subset of the path receive a left or right shift.
// - MIRROR: a random subset of the path is mirrored.
// - MIXED: a mutation randomly chosen between SWAP_ANY, ROTATE or MIRROR is done.
typedef enum {SWAP_CONSECUTIVE, SWAP_ANY, ROTATE, MIRROR, MIXED} MutationOption;


// Option managing both the number and the pattern of mutations beeing applied to the same chosen individual during an epoch:
// - GOOSE: A random number of mutations between 1 and 'MaxMutationNumber' is chosen, then said number of mutations is done,
//   and the final result is saved if good enough. The option's name comes from the fact that the individual moves at random,
//   like in the Game of the Goose ;)
// - NO_GOOSE_MAX: 'MaxMutationNumber' mutations are done, and each of them may replace the spot of the worst individual
//   at the given epoch. This spot stays fixed during the current epoch, even if a better solution have been written here.
// - NO_GOOSE_FULL: 'MaxMutationNumber' mutations are done, each of them being able to replace the worst individual, while said
//   worst individual is being updated after each mutation.
typedef enum {GOOSE, NO_GOOSE_MAX, NO_GOOSE_FULL} GooseOption;


// Information on epochs number rentability option:
// - NO_INFO: nothing is done.
// - INFO: the ratio of the epoch at which the best individual has been found once the search is over, by the total epochs
//   number is saved. Considerations have been taken in case said individual has been found many times: the smallest epoch
//   is saved. This option is useful for monitoring the search efficiency: if said ratio is close to 1, it may be better
//   to search for a longer time; if instead it is consistently close to 0, either lowering the total epochs number or
//   increasing the population size may be advisable.
typedef enum {NO_INFO, INFO} BestInfo;


//
typedef enum {SYM_ALLOWED, SYM_UNALLOWED} SymOption;


// Genetic search parameters:
typedef struct
{
	int Epochs;
	int PopulationSize;
	int MaxMutationNumber;
	SelectionOption SelectOpt;
	MutationOption MutationOpt;
	GooseOption GooseOpt;
	BestInfo Binfo;
	SymOption SymOpt;
} GenParameters;


// Creates genetic search parameters, initialized with default values. To be freed with a regular free() call.
GenParameters* initGenParameters(void);


void printGenParameters(GenParameters *params);


// Searching the worst path, i.e the one with higher total length:
int indexWorstPath(Number *pathLength_array, int population_size);


// Searching the best path, i.e the one with lower total length:
int indexBestPath(Number *pathLength_array, int population_size);


// Finding the smallest epoch at which the best path has been found, once the genetic search is done:
int minEpochBest(Number *pathLength_array, int *infoArray, int population_size, int indexBest);


// Choose a path whith higher probabiblity the better it is.
// 'pathLength_array' is an array containing the lengths of each path of the population.
// 'sum_inv_values' is the sum of all the inverse of lengths from the population, and is already computed,
// for better performances. The inverse of a path length, divided by 'sum_inv_values', is the used distribution,
// i.e the fitness function of a path is the inverse of the path length:
int choose_probabilistic(Number *pathLength_array, int population_size, Number sum_inv_pathLength);


// Selecting a random path in the given population:
int selection(Number *pathLength_array, int population_size, Number sum_inv_pathLength, SelectionOption choiceOpt);


// Mutates the new path by swaping cities randomly. The last city must be left untouched. Requires citiesNumber >= 3.
void mutation(int *new_path, int citiesNumber, MutationOption swapOpt);


// Replacing the worst path by a new one, if the latter is better, and if so updates 'sum_inv_pathLength':
int replace(Map *map, int **population, Number *pathLength_array, Number *sum_inv_pathLength, int *new_path, int indexWorst,
	int *infoArray, int current_epoch, int sym_option);


// Genetic search. A small path length is beeing seeked by evolving from a starting path.
// Last city of any path is keeped fixed, as it is set to be the starting/end point.
void smallest_genetic(Map *map, int *best_path, Number *best_pathLength, double *time, GenParameters *params, float *best_epoch_ratio);


#endif
