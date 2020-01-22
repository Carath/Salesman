#include <stdio.h>
#include <stdlib.h>

#include "animation.h"
#include "drawing.h"


// Global variables:

const Uint8 *Keyboard_state;

SDL_Window *window;
SDL_Renderer *renderer;

TTF_Font *font_medium;

SDL_Color Yellow = {255, 255, 0, 255};
SDL_Color Red = {255, 0, 0, 255};
SDL_Color Magenta = {255, 0, 255, 255};

int quit = 0;
int render_scene = 1;

Map *animation_map;

GenParameters *genParams;

PathChoice choice = UNDEFINED;

int *brute_path;
int *gen_path;

Number brute_force_pathLength = 0;
double brute_force_time = 0;

Number genetic_pathLength = 0;
double genetic_time = 0;

int brute_done = 0;


// Main function for using the graphical interface:
void animation(void)
{
	////////////////////////////////////////////////////////////
	// Initializing SDLA - rendering:

	SDLA_Init(&window, &renderer, "Travelling Salesman", WINDOW_WIDTH, WINDOW_HEIGHT,
		USE_HARDWARE_ACCELERATION, SDLA_BLENDED);

	////////////////////////////////////////////////////////////
	// Font loading:

	char font_name[] = "/usr/share/fonts/truetype/dejavu/DejaVuSerif-Bold.ttf";

	font_medium = TTF_OpenFont(font_name, 25);

	if (font_medium == NULL)
	{
		printf("\nCannot load the font: %s\n\n", font_name);
		exit(EXIT_FAILURE);
	}

	////////////////////////////////////////////////////////////
	// For user inputs:

	Keyboard_state = SDL_GetKeyboardState(NULL);

	////////////////////////////////////////////////////////////
	// Creating a map:

	animation_map = createMap(START_CITY_NUMBER);

	////////////////////////////////////////////////////////////
	// Creating paths:

	resetPaths();

	////////////////////////////////////////////////////////////
	// Parameters for the genetic algorithm:

	genParams = initGenParameters();

	genParams -> PopulationSize = 64;
	genParams -> MaxMutationNumber = 1;
	genParams -> SelectOpt = UNIFORMLY;
	genParams -> MutationOpt = MIRROR;
	genParams -> GooseOpt = NO_GOOSE_FULL;
	genParams -> SymOpt = SYM_UNALLOWED;
	genParams -> Binfo = INFO;

	updateEpochsNumber(animation_map, genParams);

	////////////////////////////////////////////////////////////
	// Starting the event loop:

	// int frame_index = 0; // for controlling the scene rendering.

	while (!quit)
	{
		input_control();

		// Rendering:
		if (render_scene)
		{
			SDLA_ClearWindow(NULL);

			if (choice == CHOICE_BRUTE)
				draw_path(animation_map, brute_path, &Red);

			else if (choice == CHOICE_GENETIC)
				draw_path(animation_map, gen_path, &Magenta);

			draw_map(animation_map, ONLY_CITIES);
			// draw_map(animation_map, ALL_PATHS);

			// draw_boxes();

			draw_infos();

			// printf("frame_index: %d\n", frame_index);
			// ++frame_index;

			SDL_RenderPresent(renderer);
		}

		render_scene = 0;
	}

	////////////////////////////////////////////////////////////
	// Freeing everything:

	free(brute_path);
	free(gen_path);

	free(genParams);
	freeMap(animation_map);

	TTF_CloseFont(font_medium);

	SDLA_Quit();
}


// Allocating memory for the paths, and reset some variables:
void resetPaths(void)
{
	free(brute_path);
	free(gen_path);

	brute_path = (int*) calloc(animation_map -> CitiesNumber, sizeof(int));
	gen_path = (int*) calloc(animation_map -> CitiesNumber, sizeof(int));

	choice = UNDEFINED;

	brute_force_pathLength = 0;
	brute_force_time = 0;

	genetic_pathLength = 0;
	genetic_time = 0;

	brute_done = 0;

	render_scene = 1;
}


// Adapt the epochs number to the current cities number:
void updateEpochsNumber(Map *map, GenParameters *params)
{
	int n = map -> CitiesNumber;

	params -> Epochs = 10000 * n;

	// printf("-> Cities number = %d, epochs number = %d\n", n, params -> Epochs);
}


// Updates the number of cities by 'delta', if possible, and calls resetPaths().
void updateCitiesNumber(int delta)
{
	int new_citiesNumber = animation_map -> CitiesNumber + delta;

	if (new_citiesNumber < MIN_CITIES_NUMBER)
	{
		// printf("Minimum cities number reached: %d.\n", MIN_CITIES_NUMBER);
		draw_message(TOO_FEW);
		return;
	}

	freeMap(animation_map);

	animation_map = createMap(new_citiesNumber);

	if (delta != 0)
		updateEpochsNumber(animation_map, genParams);

	resetPaths();

	// printf("Cities number got modified to: %d.\n", new_citiesNumber);
}
