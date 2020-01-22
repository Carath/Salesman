#ifndef USER_INPUTS_H
#define USER_INPUTS_H


#include "SDLA.h"

#include "settings.h"
#include "genetic.h"


typedef enum {UNDEFINED, CHOICE_BRUTE, CHOICE_GENETIC} PathChoice;


extern const Uint8 *Keyboard_state;

extern int quit;
extern int render_scene;

extern Map *animation_map;

extern GenParameters *genParams;

extern PathChoice choice;

extern int *brute_path;
extern int *gen_path;

extern Number brute_force_pathLength;
extern double brute_force_time;

extern Number genetic_pathLength;
extern double genetic_time;

extern int brute_done;


// Main function for handling user inputs:
void input_control(void);


// Returns 1 if the given key is actually pressed, and if so updates 'last_pressed_key' and 'key_still_down'.
int key_pressed(SDL_Keycode key);


#endif
