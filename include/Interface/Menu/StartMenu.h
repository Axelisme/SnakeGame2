#ifndef START_MENU_H
#define START_MENU_H

#include "Interface/Menu/MultiMenu.h"

typedef enum START_MENU_STATE {
    START_MENU_STATE_START = 0,
    START_MENU_STATE_LEVEL = 1,
    START_MENU_STATE_MUSIC = 2,
    START_MENU_STATE_GUIDE = 3,
    START_MENU_STATE_EXIT = 4
} START_MENU_STATE;

typedef MultiMenu StartMenu;

StartMenu* new_StartMenu();
void StartMenu_init(StartMenu* self);

static void _StartMenu_enter_state(StartMenu* self);

#endif // START_MENU_H



