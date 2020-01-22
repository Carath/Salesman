#ifndef DRAWING_H
#define DRAWING_H


#include "SDLA.h"

#include "settings.h"
#include "salesman.h"


extern SDL_Renderer *renderer;

extern TTF_Font *font_medium;

extern SDL_Color Yellow;

extern Map *animation_map;

extern Number brute_force_pathLength;
extern double brute_force_time;

extern Number genetic_pathLength;
extern double genetic_time;


typedef enum {ONLY_CITIES, ALL_PATHS} DrawOption;

typedef enum {WAITING, TOO_FEW, TOO_MANY} Warning;


void draw_map(Map *map, DrawOption dropt);


void draw_path(Map *map, int *path, SDL_Color *color);


void draw_boxes(void);


void draw_infos(void);


// Bypasses 'render_scene' as the message must be drawn without delay:
void draw_message(Warning warning);


#endif
