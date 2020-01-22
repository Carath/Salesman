#ifndef SALESMAN_H
#define SALESMAN_H


#include "settings.h"


typedef struct
{
	const int CitiesNumber;
	Number **Locations; // citiesNumber * 2
	Number **Net; // citiesNumber * citiesNumber
} Map;


// Euclidean distance between (x1, y1) and (x2, y2).
Number distance(Number x1, Number y1, Number x2, Number y2);


Map* createMap(int citiesNumber);


void freeMap(Map *map);


void initMap(Map *map);


void printMap(Map *map);


// Length of the total path, coming back to the start:
Number path_length(Map *map, int *path, int len);


#endif
