#include <stdio.h>

#include "user_inputs.h"
#include "drawing.h"
#include "brute_force.h"
#include "animation.h"


static SDL_Event event;
static SDL_Keycode last_pressed_key;
static int key_still_down = 0; // must start with 0.


// Main function for handling user inputs:
void input_control(void)
{
	SDL_WaitEvent(&event);

	if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == QUIT_KEY))
		quit = 1;


	else if (key_still_down) // Nothing happens if the last key is still down.
	{
		if (!Keyboard_state[SDL_GetScancodeFromKey(last_pressed_key)])
			key_still_down = 0;
	}


	// Resetting the animation_map:
	else if (key_pressed(REFRESH_KEY))
	{
		updateCitiesNumber(0);
	}


	else if (key_pressed(DECR_KEY_1))
	{
		updateCitiesNumber(-1);
	}


	else if (key_pressed(INCR_KEY_1))
	{
		updateCitiesNumber(1);
	}


	else if (key_pressed(DECR_KEY_10))
	{
		updateCitiesNumber(-10);
	}


	else if (key_pressed(INCR_KEY_10))
	{
		updateCitiesNumber(10);
	}


	else if (key_pressed(BRUTE_KEY))
	{
		if (animation_map -> CitiesNumber > MAX_CITIES_NUMBER_BRUTE)
		{
			draw_message(TOO_MANY);

			// printf("\nNumber of cities is too big for using the brute force: %d.\n", animation_map -> CitiesNumber);
		}

		else
		{
			if (!brute_done) // The brute force path is only computed once for a given map.
			{
				draw_message(WAITING);

				// brute_Heap(animation_map, brute_path, &brute_force_pathLength, &brute_force_time);

				brute_Trotter(animation_map, brute_path, &brute_force_pathLength, &brute_force_time); // faster!

				brute_done = 1;
			}

			// Setting the (potentially already computed) brute force path to be drawn:

			choice = CHOICE_BRUTE;

			render_scene = 1;
		}
	}


	else if (key_pressed(GEN_KEY)) // A new path is computed every time the key is pressed.
	{
		draw_message(WAITING);

		float epoch_ratio = 0;

		smallest_genetic(animation_map, gen_path, &genetic_pathLength, &genetic_time, genParams, &epoch_ratio);

		if (genParams -> Binfo == INFO)
			printf("epoch_ratio = %.2f %%\n", 100 * epoch_ratio);

		choice = CHOICE_GENETIC;

		render_scene = 1;
	}
}


// Returns 1 if the given key is actually pressed, and if so updates 'last_pressed_key' and 'key_still_down'.
int key_pressed(SDL_Keycode key)
{
	if (Keyboard_state[SDL_GetScancodeFromKey(key)])
	{
		last_pressed_key = key;
		key_still_down = 1;
		return 1;
	}

	else
		return 0;
}
