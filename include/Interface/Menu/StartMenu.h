#ifndef START_MENU_H
#define START_MENU_H

#include "Interface/Interface.h"

typedef enum START_MENU_STATE {
    START_MENU_STATE_START = 0,
    START_MENU_STATE_LEVEL = 1,
    START_MENU_STATE_MUSIC = 2,
    START_MENU_STATE_GUIDE = 3,
    START_MENU_STATE_EXIT = 4
} START_MENU_STATE;

typedef struct StartMenu {
    // Inherited from Interface
    Interface interface;
    // Info
    START_MENU_STATE menu_state;
    // Image
    ALLEGRO_BITMAP *  start_image;
    ALLEGRO_BITMAP *  level_image;
    ALLEGRO_BITMAP *  music_image;
    ALLEGRO_BITMAP *  guide_image;
    ALLEGRO_BITMAP *  exit_image;
} StartMenu;

StartMenu* new_StartMenu();
void StartMenu_init(StartMenu* self);
void StartMenu_destroy(StartMenu* self);
void delete_StartMenu(Interface* Iself);
void StartMenu_draw(Interface* Iself, ALLEGRO_BITMAP* backbuffer);
INTERFACE_INFO StartMenu_update(Interface* Iself);
void StartMenu_event_record(Interface* Iself, ALLEGRO_EVENT event);

static void _StartMenu_deal_event(StartMenu* self);
static void _StartMenu_init_image(StartMenu* self);
static void _StartMenu_load_image(StartMenu* self);
static ALLEGRO_BITMAP* _StartMenu_current_image(StartMenu* self);
static START_MENU_STATE _StartMenu_next_state(StartMenu* self);
static START_MENU_STATE _StartMenu_prev_state(StartMenu* self);
static void _StartMenu_enter_state(StartMenu* self);
static void _StartMenu_escape(StartMenu* self);

#endif // START_MENU_H



