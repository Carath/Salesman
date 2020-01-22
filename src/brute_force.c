#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "brute_force.h"
#include "permutations.h"
#include "utilities.h"


// Find the smallest path length, by brute force: every permutations of the n-1 first cities are found using
// a non recursive version of the Heap algorithm. Last city is fixed, as it is the starting/end point.
void brute_Heap(Map *map, int *best_path, Number *best_pathLength, double *time)
{
	clock_t start = clock();

	if (map == NULL || best_path == NULL || best_pathLength == NULL || time == NULL)
	{
		printf("\nNULL argument in 'brute_Heap'.\n\n");
		exit(EXIT_FAILURE);
	}

	int n = map -> CitiesNumber;

	if (n < 3)
	{
		printf("\nInsufficient 'citiesNumber'.\n\n");
		exit(EXIT_FAILURE);
	}

	int *path = (int*) calloc(n, sizeof(int));
	int *buffer = (int*) calloc(n - 1, sizeof(int));

	for (int i = 0; i < n; ++i)
		path[i] = i;

	for (int i = 0; i < n - 1; ++i)
		buffer[i] = 0;

	// Processing the first path:
	*best_pathLength = path_length(map, path, n);

	for (int k = 0; k < n; ++k)
		best_path[k] = path[k];

	int i = 0;

	while (i < n - 1)
	{
		if (buffer[i] < i)
		{
			int pos = i % 2 == 0 ? 0 : buffer[i];

			swap(path, pos, i);

			Number new_pathLength = path_length(map, path, n);

			if (new_pathLength < *best_pathLength)
			{
				*best_pathLength = new_pathLength;

				for (int k = 0; k < n; ++k)
					best_path[k] = path[k];
			}

			++(buffer[i]);

			i = 0;
		}

		else
		{
			buffer[i] = 0;

			++i;
		}
	}

	free(buffer);
	free(path);

	*time = time_elapsed(start);

	// printf("\nbrute_Heap:\n -> Time elapsed: %.3f s, found: %.6f km (optimum)\n\n", *time, *best_pathLength);
}


// Next permutaion produced by the Steinhaus–Johnson–Trotter algorithm:
inline static int Trotter_next_permutation(int *path, int *positions, int *directions, int n, int *pos)
{
	int largestMobile = n - 1, idxToSwap;

	while (largestMobile >= 0)
	{
		idxToSwap = positions[largestMobile] + directions[largestMobile];

		if (idxToSwap >= 0 && idxToSwap < n && path[idxToSwap] < largestMobile)
			break;

		--largestMobile;
	}

	if (largestMobile < 0) // No mobile value has been found.
	{
		printf("\nDone!\n");
		*pos = 0;
		return 1;
	}

	int idxLargestMobile = positions[largestMobile];
	int toSwap = path[idxToSwap];

	swap(path, idxLargestMobile, idxToSwap);
	swap(positions, largestMobile, toSwap);

	// Changing directions:
	for (int i = largestMobile + 1; i != n; ++i)
		directions[i] = -directions[i];

	// Save the first index of where the swap takes place:
	*pos = MIN(idxLargestMobile, idxToSwap);

	return 0;
}


// Find the smallest path length, by brute force: every permutations of the n-1 first cities are found using
// the Steinhaus–Johnson–Trotter algorithm. Last city is fixed, as it is the starting/end point.
// Since this algorithm produces no permutations symmetric to each other in the first half, we can stop midway.
// Also, only consecutive transpositions are made, so one does not need to compute all the paths length anymore:
void brute_Trotter(Map *map, int *best_path, Number *best_pathLength, double *time)
{
	clock_t start = clock();

	if (map == NULL || best_path == NULL || best_pathLength == NULL || time == NULL)
	{
		printf("\nNULL argument in 'brute_Trotter'.\n\n");
		exit(EXIT_FAILURE);
	}

	int n = map -> CitiesNumber;

	if (n < 3)
	{
		printf("\nInsufficient 'citiesNumber'.\n\n");
		exit(EXIT_FAILURE);
	}

	int *path = (int*) calloc(n, sizeof(int));
	int *positions = (int*) calloc(n, sizeof(int));
	int *directions = (int*) calloc(n, sizeof(int));

	// First permutation:
	for (int i = 0; i < n; ++i)
		path[i] = i; // Modifying this would require to shift the path.

	for (int i = 0; i < n - 1; ++i)
		positions[i] = i;

	for (int i = 0; i < n - 1; ++i)
		directions[i] = -1; // going left.

	// Processing the first path:
	*best_pathLength = path_length(map, path, n);

	Number current_pathLength = *best_pathLength;

	for (int k = 0; k < n; ++k)
		best_path[k] = path[k];

	// We will generate only the first half of all the permutations:
	BigInt midpoint = fact(n - 1) / 2;

	for (BigInt i = 1; i < midpoint; ++i)
	{
		int pos;

		Trotter_next_permutation(path, positions, directions, n - 1, &pos);

		int prev_city = pos == 0 ? path[n - 1] : path[pos - 1];
		int curr_city = path[pos];
		int next_city = path[pos + 1];
		int next_next_city = path[pos + 2]; // cannot buffer overflow.

		// Careful! The swap has already been done!
		current_pathLength += map -> Net[prev_city][curr_city] - map -> Net[prev_city][next_city];
		current_pathLength += map -> Net[next_city][next_next_city] - map -> Net[curr_city][next_next_city];

		if (current_pathLength < *best_pathLength)
		{
			*best_pathLength = current_pathLength;

			for (int k = 0; k < n; ++k)
				best_path[k] = path[k];
		}
	}

	free(path);
	free(positions);
	free(directions);

	*time = time_elapsed(start);

	// printf("\nbrute_Trotter:\n -> Time elapsed: %.3f s, found: %.6f km (optimum)\n\n", *time, *best_pathLength);
}
