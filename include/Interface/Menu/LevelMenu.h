#ifndef LEVEL_MENU_H
#define LEVEL_MENU_H

#include "Interface/Interface.h"

typedef enum LEVEL_MENU_STATE {
    LEVEL_MENU_LEVEL_1 = 1,
    LEVEL_MENU_LEVEL_2,
    LEVEL_MENU_LEVEL_3,
    LEVEL_MENU_LEVEL_4,
    LEVEL_MENU_EXIT,
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
    ALLEGRO_BITMAP *  level_exit_image;
} LevelMenu;

LevelMenu* new_LevelMenu();
void LevelMenu_init(LevelMenu* self);
void LevelMenu_destroy(LevelMenu* self);
void delete_LevelMenu(Interface* Iself);
void LevelMenu_draw(Interface* Iself, ALLEGRO_BITMAP* backbuffer);
INTERFACE_INFO LevelMenu_update(Interface* Iself);
void LevelMenu_event_record(Interface* Iself, ALLEGRO_EVENT event);

static void _LevelMenu_deal_event(LevelMenu* self);
static void _LevelMenu_init_image(LevelMenu* self);
static void _LevelMenu_load_image(LevelMenu* self);
static ALLEGRO_BITMAP* _LevelMenu_current_image(LevelMenu* self);
static LEVEL_MENU_STATE _LevelMenu_next_state(LevelMenu* self);
static LEVEL_MENU_STATE _LevelMenu_prev_state(LevelMenu* self);
static void _LevelMenu_enter_state(LevelMenu* self);
static void _LevelMenu_escape(LevelMenu* self);

#endif // LEVEL_MENU_H



