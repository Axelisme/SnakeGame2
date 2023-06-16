#ifndef GLOBAL_H
#define GLOBAL_H

#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_audio.h"
#include "allegro5/allegro_acodec.h"
#include <stdbool.h>

// Debug
#define Debug 1

// Color
#define COLOR_NUM 9
#define BLACK  al_map_rgb(  0,  0,  0)
#define GRAY   al_map_rgb(128,128,128)
#define WRITE  al_map_rgb(255,255,255)
#define RED    al_map_rgb(255,  0,  0)
#define GREEN  al_map_rgb(  0,255,  0)
#define BLUE   al_map_rgb(  0,  0,255)
#define CMY    al_map_rgb(  0,255,255)
#define PINK   al_map_rgb(255,  0,255)
#define YELLOW al_map_rgb(255,255,  0)

// Other
#define MAX_PATH_LEN 1024
#define nullptr NULL

// Error
void raise_err(const char* );
void raise_warn(const char* );
void show_msg(const char* );

// Macro
#define swap(a,b) {typeof(a) tmp = a; a = b; b = tmp;}

#endif // GLOBAL_H