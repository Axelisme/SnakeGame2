#ifndef LEVEL_MENU_H
#define LEVEL_MENU_H

#include "Interface/Interface.h"

typedef enum LEVEL_MENU_STATE {
    LEVEL_MENU_LEVEL_1 = 1,
    LEVEL_MENU_LEVEL_2,
    LEVEL_MENU_LEVEL_3,
    LEVEL_MENU_LEVEL_4
} LEVEL_MENU_STATE;

typedef struct LevelMenu {
    // Inherited from Interface
    Interface interface;
    // Info
    LEVEL_MENU_STATE menu_state;
    // Image
    ALLEGRO_BITMAP *  level_1_image;
    ALLEGRO_BITMAP *  level_2_image;
    ALLEGRO_BITMAP *  level_3_image;
    ALLEGRO_BITMAP *  level_4_image;
} LevelMenu;



#endif // LEVEL_MENU_H



