#ifndef GLOBAL_H
#define GLOBAL_H
#undef __STRICT_ANSI__

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdbool.h>

// Debug
#define Debug 0
#define Show 0

// Game Info
#define UPDATE_FPS 60
#define FIRST_STATE GAME_MENU
#define MAP_WIDTH  39
#define MAP_HEIGHT 24
#define LEVEL_NUM 4
#define FIRST_LEVEL 1

// Display
#define DISPLAY_FPS 30
#define DISPLAY_WIDTH 800
#define DISPLAY_HEIGHT 800
#define CHUNK_W_NUM 16
#define CHUNK_H_NUM 16
#define CHUNK_WIDTH   (((double)DISPLAY_WIDTH )/CHUNK_W_NUM)
#define CHUNK_HEIGHT  (((double)DISPLAY_HEIGHT)/CHUNK_H_NUM)
#define SEE_MAP_LEFT 0
#define SEE_MAP_RIGHT 39
#define SEE_MAP_TOP 0
#define SEE_MAP_BUTTOM 24
#define MIDDLEGROUND_RATIO 0.7
#define SHOW_BACK_FOG 1
#define OBJECT_IMAGE_SIZE 3.0

// Player
#define KEY_LOCK_NUM 0
#define ANI_NUM 4
#define ANI_TOTAL_COUNT 50

// Global parameter
extern bool GameWindow_Mute;
extern int GameWindow_want_level;

// Color
#define COLOR_NUM 8
#define BLACK  al_map_rgb(  0,  0,  0)
#define RED    al_map_rgb(255,  0,  0)
#define GREEN  al_map_rgb(  0,255,  0)
#define BLUE   al_map_rgb(  0,  0,255)
#define YELLOW al_map_rgb(255,255,  0)
#define CMY    al_map_rgb(  0,255,255)
#define PINK   al_map_rgb(255,  0,255)
#define WRITE  al_map_rgb(255,255,255)
#define GRAY   al_map_rgb(128,128,128)

// Other
#define MAX_PATH_LEN 1024
#define nullptr NULL

typedef enum GAME_STATE {
    GAME_MENU,
    GAME_LEVEL,
    GAME_LEVEL_PUASE,
    GAME_TERMINATE
} GAME_STATE;

#define MENU_STATE_NUM 11
typedef enum MENU_STATE {
    IN_MENU,
    START_MENU,
    START_MENU_LEVEL,
    START_MENU_MUSIC,
    START_MENU_GUID,
    START_MENU_EXIT,
    LEVEL_MENU,
    LEVEL_MENU_1,
    LEVEL_MENU_2,
    LEVEL_MENU_3,
    LEVEL_MENU_4,
    LEVEL_MENU_EXIT,
    GUIDE
} MENU_STATE;

#define LEVEL_STAT_NUM 4
typedef enum LEVEL_STAT {
    KEEP = 0,
    STOP,
    NEXT,
    RESTART
} LEVEL_STAT;

#define DIRCTION_NUM 5
typedef enum DIRCTION {
    NONE = 0,
    LEFT,
    UP,
    DOWN,
    RIGHT
} DIRCTION;

#define EFFECT_NUM 4
typedef enum EFFECT {
    NOEFFECT,
    SHORTEN,
    TRIGER_SPIKE,
    CH_GRAVITY
} EFFECT;

// Error
void raise_err(const char* );
void raise_warn(const char* );
void show_msg(const char* );

// Init
void init_game();

// Path
void path_join(char* , const char* );

#endif // GLOBAL_H