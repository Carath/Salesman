#ifndef ANIMATION_H
#define ANIMATION_H


#include "SDLA.h"

#include "settings.h"
#include "user_inputs.h"


// Main function for using the graphical interface:
void animation(void);


// Allocating memory for the paths, and reset some variables:
void resetPaths(void);


// Adapt the epochs number to the current cities number:
void updateEpochsNumber(Map *map, GenParameters *params);


// Updates the number of cities by 'delta', if possible, and calls resetPaths().
void updateCitiesNumber(int delta);


#endif
