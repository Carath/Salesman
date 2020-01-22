#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "salesman.h"
#include "matrix.h"


// Euclidean distance between (x1, y1) and (x2, y2).
inline Number distance(Number x1, Number y1, Number x2, Number y2)
{
	Number delta_x = x1 - x2, delta_y = y1 - y2;

	return sqrt(delta_x * delta_x + delta_y * delta_y);
}


Map* createMap(int citiesNumber)
{
	Map *map = (Map*) calloc(1, sizeof(Map));

	*(int*) &(map -> CitiesNumber) = citiesNumber;

	map -> Locations = createNumberMatrix(citiesNumber, 2);

	map -> Net = createNumberMatrix(citiesNumber, citiesNumber);

	initMap(map);

	return map;
}


void freeMap(Map *map)
{
	if (map == NULL)
		return;

	freeNumberMatrix(map -> Locations, map -> CitiesNumber);

	freeNumberMatrix(map -> Net, map -> CitiesNumber);

	free(map);
}


void initMap(Map *map)
{
	if (map == NULL)
		return;

	randomNumberMatrix_uniform(map -> Locations, map -> CitiesNumber, 2, 0, DIST_BOUND);

	for (int i = 0; i < map -> CitiesNumber; ++i)
	{
		for (int j = i + 1; j < map -> CitiesNumber; ++j)
		{
			Number x1 = map -> Locations[i][0];
			Number y1 = map -> Locations[i][1];

			Number x2 = map -> Locations[j][0];
			Number y2 = map -> Locations[j][1];

			map -> Net[i][j] = distance(x1, y1, x2, y2);

			map -> Net[j][i] = map -> Net[i][j];
		}
	}
}


void printMap(Map *map)
{
	if (map == NULL)
		return;

	printf("\nNumber of cities: %d\n\n", map -> CitiesNumber);

	printf("Locations:\n\n");
	printNumberMatrix(map -> Locations, map -> CitiesNumber, 2);

	printf("Net:\n\n");
	printNumberMatrix(map -> Net, map -> CitiesNumber, map -> CitiesNumber);
}


// Length of the total path, coming back to the start:
Number path_length(Map *map, int *path, int len)
{
	if (len != map -> CitiesNumber)
	{
		printf("\nIncompatible lengths: %d vs %d.\n\n", len, map -> CitiesNumber);
		exit(EXIT_FAILURE);
	}

	Number length = map -> Net[path[len - 1]][path[0]];

	for (int i = 0; i < len - 1; ++i)
		length += map -> Net[path[i]][path[i + 1]];

	return length;
}
