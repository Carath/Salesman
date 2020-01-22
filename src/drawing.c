#include "drawing.h"


#define Y_STEP 80
#define Y_TXT_STEP 40


static char buffer_key[20];
static char buffer_string[100];


static SDL_Rect big_margin_rect = {0, 0, BIG_MARGIN, WINDOW_HEIGHT};


static SDL_Rect cities_rect = {BIG_MARGIN + SMALL_MARGIN, SMALL_MARGIN,
	WINDOW_WIDTH - BIG_MARGIN - 2 * SMALL_MARGIN, WINDOW_HEIGHT - 2 * SMALL_MARGIN};


static int Xrescale(SDL_Rect *rect, Number x)
{
	return rect -> x + x * rect -> w / DIST_BOUND;
}


static int Yrescale(SDL_Rect *rect, Number y)
{
	return rect -> y + y * rect -> h / DIST_BOUND;
}


void draw_map(Map *map, DrawOption dropt)
{
	if (map == NULL)
		return;

	SDL_Rect rect = {0, 0, CITY_SIZE, CITY_SIZE};

	for (int i = 0; i < map -> CitiesNumber; ++i)
	{
		int x1 = Xrescale(&cities_rect, map -> Locations[i][0]);
		int y1 = Yrescale(&cities_rect, map -> Locations[i][1]);

		// Drawing the paths:
		if (dropt == ALL_PATHS)
		{
			SDLA_SetDrawColor(255, 255, 255); // white

			for (int j = i + 1; j < map -> CitiesNumber; ++j)
			{
				int x2 = Xrescale(&cities_rect, map -> Locations[j][0]);
				int y2 = Yrescale(&cities_rect, map -> Locations[j][1]);

				SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
			}
		}

		// Drawing the cities:

		SDLA_SetDrawColor(0, 255, 0); // lime

		rect.x = x1 - CITY_SIZE / 2;
		rect.y = y1 - CITY_SIZE / 2;

		SDL_RenderFillRect(renderer, &rect);
	}
}


void draw_path(Map *map, int *path, SDL_Color *color)
{
	if (map == NULL || path == NULL || color == NULL)
		return;

	SDLA_SetDrawColor(color -> r, color -> g, color -> b);

	for (int i = 0; i < map -> CitiesNumber; ++i)
	{
		int current_city = path[i];
		int next_city = path[(i + 1) % map -> CitiesNumber];

		int x1 = Xrescale(&cities_rect, map -> Locations[current_city][0]);
		int y1 = Yrescale(&cities_rect, map -> Locations[current_city][1]);

		int x2 = Xrescale(&cities_rect, map -> Locations[next_city][0]);
		int y2 = Yrescale(&cities_rect, map -> Locations[next_city][1]);

		SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
	}
}


void draw_boxes(void)
{
	SDLA_SetDrawColor(255, 255, 255); // white

	SDL_RenderDrawRect(renderer, &big_margin_rect);

	SDL_RenderDrawRect(renderer, &cities_rect);
}


void draw_infos(void)
{
	// Cities number:
	sprintf(buffer_string, "Cities number: %d", animation_map -> CitiesNumber);
	SDLA_SlowDrawText(font_medium, &Yellow, SMALL_MARGIN, SMALL_MARGIN + 0 * Y_STEP + 0 * Y_TXT_STEP, buffer_string);
	sprintf(buffer_key, "%s", SDL_GetKeyName(DECR_KEY_1));
	sprintf(buffer_string, "-1 / +1: [%s/%s]", buffer_key, SDL_GetKeyName(INCR_KEY_1));
	SDLA_SlowDrawText(font_medium, &Yellow, SMALL_MARGIN, SMALL_MARGIN + 0 * Y_STEP + 1 * Y_TXT_STEP, buffer_string);
	sprintf(buffer_key, "%s", SDL_GetKeyName(DECR_KEY_10));
	sprintf(buffer_string, "-10 / +10: [%s/%s]", buffer_key, SDL_GetKeyName(INCR_KEY_10));
	SDLA_SlowDrawText(font_medium, &Yellow, SMALL_MARGIN, SMALL_MARGIN + 0 * Y_STEP + 2 * Y_TXT_STEP, buffer_string);

	// Brute force:
	sprintf(buffer_string, "Brute force: [%s]", SDL_GetKeyName(BRUTE_KEY));
	SDLA_SlowDrawText(font_medium, &Yellow, SMALL_MARGIN, SMALL_MARGIN + 1 * Y_STEP + 2 * Y_TXT_STEP, buffer_string);
	sprintf(buffer_string, "%.3f km, %.2f s", brute_force_pathLength, brute_force_time);
	SDLA_SlowDrawText(font_medium, &Yellow, SMALL_MARGIN, SMALL_MARGIN + 1 * Y_STEP + 3 * Y_TXT_STEP, buffer_string);

	// Genetic:
	sprintf(buffer_string, "Genetic: [%s]", SDL_GetKeyName(GEN_KEY));
	SDLA_SlowDrawText(font_medium, &Yellow, SMALL_MARGIN, SMALL_MARGIN + 2 * Y_STEP + 3 * Y_TXT_STEP, buffer_string);
	sprintf(buffer_string, "%.3f km, %.2f s", genetic_pathLength, genetic_time);
	SDLA_SlowDrawText(font_medium, &Yellow, SMALL_MARGIN, SMALL_MARGIN + 2 * Y_STEP + 4 * Y_TXT_STEP, buffer_string);

	// Refresh:
	sprintf(buffer_string, "New map: [%s]", SDL_GetKeyName(REFRESH_KEY));
	SDLA_SlowDrawText(font_medium, &Yellow, SMALL_MARGIN, SMALL_MARGIN + 3 * Y_STEP + 4 * Y_TXT_STEP, buffer_string);
}


// Bypasses 'render_scene' as the message must be drawn without delay:
void draw_message(Warning warning)
{
	// Clearing the text zone:

	int y_start = SMALL_MARGIN + 7 * Y_STEP;

	SDL_Rect message_rect = {0, y_start, BIG_MARGIN - 10, WINDOW_HEIGHT - y_start};

	SDLA_SetDrawColor(0, 0, 0); // black

	SDL_RenderFillRect(renderer, &message_rect);

	// Drawing the message:

	if (warning == WAITING)
	{
		SDLA_SlowDrawText(font_medium, &Yellow, SMALL_MARGIN, y_start, "Computing...");
	}

	else if (warning == TOO_FEW)
	{
		SDLA_SlowDrawText(font_medium, &Yellow, SMALL_MARGIN, y_start, "Number of cities");
		SDLA_SlowDrawText(font_medium, &Yellow, SMALL_MARGIN, y_start + Y_TXT_STEP, "can't be < 3.");
	}

	else // TOO_MANY
	{
		SDLA_SlowDrawText(font_medium, &Yellow, SMALL_MARGIN, y_start, "Number of cities is too");
		SDLA_SlowDrawText(font_medium, &Yellow, SMALL_MARGIN, y_start + Y_TXT_STEP, "big for brute force.");
	}

	SDL_RenderPresent(renderer);
}
