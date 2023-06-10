#ifndef LEVEL_MENU_H
#define LEVEL_MENU_H

#include "Interface/Menu/MultiMenu.h"

typedef enum LEVEL_MENU_STATE {
    LEVEL_MENU_LEVEL_1 = 0,
    LEVEL_MENU_LEVEL_2 = 1,
    LEVEL_MENU_LEVEL_3 = 2,
    LEVEL_MENU_LEVEL_4 = 3,
    LEVEL_MENU_EXIT,
} LEVEL_MENU_STATE;

typedef MultiMenu LevelMenu;

LevelMenu* new_LevelMenu();
void LevelMenu_init(LevelMenu* self);

static void _LevelMenu_enter_state(LevelMenu* self);

#endif // LEVEL_MENU_H



