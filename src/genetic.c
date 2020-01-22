#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "genetic.h"
#include "matrix.h"
#include "permutations.h"
#include "utilities.h"


// Creates genetic search parameters, initialized with default values. To be freed with a regular free() call.
GenParameters* initGenParameters(void)
{
	GenParameters *params = (GenParameters*) calloc(1, sizeof(GenParameters));

	params -> Epochs = 1000;
	params -> PopulationSize = 16;
	params -> MaxMutationNumber = 1;
	params -> SelectOpt = UNIFORMLY;
	params -> MutationOpt = MIRROR;
	params -> GooseOpt = NO_GOOSE_FULL;
	params -> SymOpt = SYM_ALLOWED;
	params -> Binfo = NO_INFO;

	return params;
}


void printGenParameters(GenParameters *params)
{
	printf("\nEpochs: %d\nPopulationSize: %d\nMaxMutationNumber: %d\nSelectOpt: %d\nMutationOpt: %d\nGooseOpt: %d\nSymOpt: %d\nBinfo: %d\n",
		params -> Epochs, params -> PopulationSize, params -> MaxMutationNumber, params -> SelectOpt, params -> MutationOpt,
		params -> GooseOpt, params -> SymOpt, params -> Binfo);
}


// Searching the worst path, i.e the one with higher total length:
int indexWorstPath(Number *pathLength_array, int population_size)
{
	Number worst_pathLength = pathLength_array[0];
	int indexWorst = 0;

	for (int index = 1; index < population_size; ++index)
	{
		if (STR_INEQUALITY(worst_pathLength, pathLength_array[index]))
		{
			worst_pathLength = pathLength_array[index];
			indexWorst = index;
		}
	}

	return indexWorst;
}


// Searching the best path, i.e the one with lower total length:
int indexBestPath(Number *pathLength_array, int population_size)
{
	Number best_pathLength = pathLength_array[0];
	int indexBest = 0;

	for (int index = 1; index < population_size; ++index)
	{
		if (STR_INEQUALITY(pathLength_array[index], best_pathLength))
		{
			best_pathLength = pathLength_array[index];
			indexBest = index;
		}
	}

	return indexBest;
}


// Finding the smallest epoch at which the best path has been found, once the genetic search is done:
int minEpochBest(Number *pathLength_array, int *infoArray, int population_size, int indexBest)
{
	Number best_pathLength = pathLength_array[indexBest];
	int min_epoch_best = infoArray[indexBest];

	for (int index = 0; index < population_size; ++index)
	{
		if (epsilonEquality(pathLength_array[index], best_pathLength) && STR_INEQUALITY(infoArray[index], min_epoch_best))
			min_epoch_best = infoArray[index];
	}

	return min_epoch_best;
}


// Choose a path whith higher probabiblity the better it is.
// 'pathLength_array' is an array containing the lengths of each path of the population.
// 'sum_inv_values' is the sum of all the inverse of lengths from the population, and is already computed,
// for better performances. The inverse of a path length, divided by 'sum_inv_values', is the used distribution,
// i.e the fitness function of a path is the inverse of the path length:
int choose_probabilistic(Number *pathLength_array, int population_size, Number sum_inv_pathLength)
{
	Number threshold = RAND_FLOAT() * sum_inv_pathLength;

	int index = 0;

	Number partial_sum = 1. / pathLength_array[0];

	while (STR_INEQUALITY(partial_sum, threshold))
	{
		++index;

		if (index == population_size)
		{
			--index;
			break;
		}

		partial_sum += 1. / pathLength_array[index];
	}

	return index;
}


// Selecting a random path in the given population:
int selection(Number *pathLength_array, int population_size, Number sum_inv_pathLength, SelectionOption choiceOpt)
{
	if (choiceOpt == UNIFORMLY)
		return RAND_BOUNDED(population_size);

	else if (choiceOpt == PROBABILISTIC)
		return choose_probabilistic(pathLength_array, population_size, sum_inv_pathLength);

	else // BEST
		return indexBestPath(pathLength_array, population_size);
}


// Mutates the new path by swaping cities randomly. The last city must be left untouched. Requires citiesNumber >= 3.
void mutation(int *new_path, int citiesNumber, MutationOption swapOpt)
{
	int city_1, city_2;

	if (swapOpt == SWAP_CONSECUTIVE)
	{
		city_1 = RAND_BOUNDED(citiesNumber - 2);

		swap(new_path, city_1, city_1 + 1);
	}

	else
	{
		if (swapOpt == MIXED) // deciding if a swap, rotate or mirror must be done.
			swapOpt = 1 + RAND_BOUNDED(3);

		// Findind city_1 != city_2 randomly:

		city_1 = RAND_BOUNDED(citiesNumber - 1);

		do
		{
			city_2 = RAND_BOUNDED(citiesNumber - 1);
		}
		while (city_1 == city_2);

		if (swapOpt == SWAP_ANY)
			swap(new_path, city_1, city_2);

		else
		{
			int city_start, city_end;

			if (city_1 < city_2)
			{
				city_start = city_1;
				city_end = city_2;
			}

			else
			{
				city_start = city_2;
				city_end = city_1;
			}

			// Now we are sure that city_1 < city_2.

			if (swapOpt == ROTATE)
			{
				int dir = RAND_BOUNDED(2);

				rotate(new_path, city_start, city_end, dir);
			}

			else // MIRROR
				mirror(new_path, city_start, city_end);
		}
	}
}


// Replacing the worst path by a new one, if the latter is better, and if so updates 'sum_inv_pathLength':
int replace(Map *map, int **population, Number *pathLength_array, Number *sum_inv_pathLength, int *new_path, int indexWorst,
	int *infoArray, int current_epoch, int sym_option)
{
	Number new_pathLength = path_length(map, new_path, map -> CitiesNumber);

	if (STR_INEQUALITY(new_pathLength, pathLength_array[indexWorst]))
	{
		// Making sure that no unwanted symmetry is added:
		if (sym_option == SYM_UNALLOWED && new_path[0] > new_path[map -> CitiesNumber - 2])
			mirror(new_path, 0, map -> CitiesNumber - 2);

		// Updating the sum of the inverse of the lengths:
		*sum_inv_pathLength += 1. / new_pathLength - 1. / pathLength_array[indexWorst];

		// Replacing the worst path:
		for (int j = 0; j < map -> CitiesNumber; ++j)
			population[indexWorst][j] = new_path[j];

		// Updating the length of the new path:
		pathLength_array[indexWorst] = new_pathLength;

		if (infoArray != NULL) // Should happen iif params -> Binfo == INFO.
			infoArray[indexWorst] = current_epoch;

		return 1;
	}

	return 0;
}


// Genetic search. A small path length is beeing seeked by evolving from a starting path.
// Last city of any path is keeped fixed, as it is set to be the starting/end point.
void smallest_genetic(Map *map, int *best_path, Number *best_pathLength, double *time, GenParameters *params, float *best_epoch_ratio)
{
	clock_t start = clock();

	if (map == NULL || best_path == NULL || best_pathLength == NULL || time == NULL)
	{
		printf("\nNULL argument in 'smallest_genetic'.\n\n");
		exit(EXIT_FAILURE);
	}

	///////////////////////////////////////////////////////
	// Setup:

	int citiesNumber = map -> CitiesNumber;
	int population_size = params -> PopulationSize;

	if (citiesNumber < 3)
	{
		printf("\nInsufficient 'citiesNumber'.\n\n");
		exit(EXIT_FAILURE);
	}

	// Choosing a starting path:
	int *path_init = (int*) calloc(citiesNumber, sizeof(int));

	for (int i = 0; i < citiesNumber; ++i)
		path_init[i] = i; // a trivial permutation.

	// Total length of the starting path:
	Number pathLength_init = path_length(map, path_init, citiesNumber);

	// Creating a population of paths:
	int **population = createIntMatrix(population_size, citiesNumber);

	// Creating an array containing all the total lengths for each path of the population:
	Number *pathLength_array = (Number*) calloc(population_size, sizeof(Number));

	// Creating an array that will be filled (if desired) with the epoch at which each member of the population was found:
	int *infoArray = params -> Binfo == INFO ? (int*) calloc(population_size, sizeof(int)) : NULL;

	// Copying the starting path all over 'population', and updating 'pathLength_array':
	for (int index = 0; index < population_size; ++index)
	{
		copyIntMatrix(&path_init, population + index, 1, citiesNumber);

		pathLength_array[index] = pathLength_init;
	}

	// Sum of all the inverse of lengths from the population:
	Number sum_inv_pathLength = population_size / pathLength_init;

	// Buffer that will contain the newborn paths:
	int *path_buffer = (int*) calloc(citiesNumber, sizeof(int));

	///////////////////////////////////////////////////////
	// Starting the evolution process:

	for (int epoch = 0; epoch < params -> Epochs; ++epoch)
	{
		// Selecting a random path in the given population:
		int indexSelectedPath = selection(pathLength_array, population_size, sum_inv_pathLength, params -> SelectOpt);

		// Copying the chosen path into the buffer:
		copyIntMatrix(population + indexSelectedPath, &path_buffer, 1, citiesNumber);

		if (params -> GooseOpt == GOOSE)
		{
			// Choosing a number of swaps to do:
			int swapNumber = 1 + RAND_BOUNDED(params -> MaxMutationNumber);

			// Searching the worst path, i.e the one with higher total length:
			int indexWorst = indexWorstPath(pathLength_array, population_size);

			for (int s = 0; s < swapNumber; ++s)
			{
				// Mutates the new path by swaping cities randomly. The last city must be left untouched.
				mutation(path_buffer, citiesNumber, params -> MutationOpt);
			}

			// Replacing the worst path by a new one, if the latter is at least as good. Updates 'sum_inv_pathLength':
			replace(map, population, pathLength_array, &sum_inv_pathLength, path_buffer, indexWorst, infoArray, epoch, params -> SymOpt);
		}

		else if (params -> GooseOpt == NO_GOOSE_MAX)
		{
			int swapNumber = params -> MaxMutationNumber;

			// Searching the worst path, i.e the one with higher total length:
			int indexWorst = indexWorstPath(pathLength_array, population_size);

			for (int s = 0; s < swapNumber; ++s)
			{
				// Mutates the new path by swaping cities randomly. The last city must be left untouched.
				mutation(path_buffer, citiesNumber, params -> MutationOpt);

				// Replacing the worst path by a new one, if the latter is at least as good. Updates 'sum_inv_pathLength':
				replace(map, population, pathLength_array, &sum_inv_pathLength, path_buffer, indexWorst, infoArray, epoch, params -> SymOpt);
			}
		}

		else // NO_GOOSE_FULL
		{
			int swapNumber = params -> MaxMutationNumber;

			for (int s = 0; s < swapNumber; ++s)
			{
				// Searching the worst path, i.e the one with higher total length:
				int indexWorst = indexWorstPath(pathLength_array, population_size);

				// Mutates the new path by swaping cities randomly. The last city must be left untouched.
				mutation(path_buffer, citiesNumber, params -> MutationOpt);

				// Replacing the worst path by a new one, if the latter better. Updates 'sum_inv_pathLength':
				replace(map, population, pathLength_array, &sum_inv_pathLength, path_buffer, indexWorst, infoArray, epoch, params -> SymOpt);
			}
		}
	}

	///////////////////////////////////////////////////////
	// Retrieving the results:

	// Finding the smallest length found:
	int indexBest = indexBestPath(pathLength_array, population_size);

	// Saving the best found path:
	copyIntMatrix(population + indexBest, &best_path, 1, citiesNumber);

	// Saving the best found length:
	*best_pathLength = pathLength_array[indexBest];

	// Saving the best found path's epoch ratio:
	if (best_epoch_ratio != NULL && infoArray != NULL)
	{
		int min_epoch_best = minEpochBest(pathLength_array, infoArray, population_size, indexBest);

		*best_epoch_ratio = (float) min_epoch_best / params -> Epochs;
	}

	///////////////////////////////////////////////////////
	// Freeing everything:

	freeIntMatrix(population, population_size);
	free(path_init);
	free(pathLength_array);
	free(infoArray);
	free(path_buffer);

	*time = time_elapsed(start);

	// printf("\nsmallest_genetic:\n -> Time elapsed: %.3f s, found: %.6f km\n\n", *time, *best_pathLength);
}
