#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "global.h"
#include "Interface/Interface.h"
#define INIT_DISPLAY_WIDTH 900
#define INIT_DISPLAY_HEIGHT 900
#define DISPLAY_FPS 60
#define FIRST_INTERFACE INTERFACE_IN_MENU
#define INTERFACE_MAX_NUM 10

typedef enum GAMEWINDOW_STATE {
    GAMEWINDOW_RUNNING = 1,
    GAMEWINDOW_EXIT = 0
} GAMEWINDOW_STATE;

typedef struct  GameWindow {
    // display
    ALLEGRO_DISPLAY* display;
    // Icon
    ALLEGRO_BITMAP* icon;
    // sound
    ALLEGRO_SAMPLE* background_sample;
    // state
    GAMEWINDOW_STATE state;
    // event
    ALLEGRO_EVENT event;
    // Interface
    int interface_num;
    Interface* interfaces[INTERFACE_MAX_NUM];
} GameWindow;

GameWindow* new_GameWindow();
void GameWindow_init(GameWindow* self);
void GameWindow_destroy(GameWindow* self);
void delete_GameWindow(GameWindow* self);
void GameWindow_draw(GameWindow* self);
GAMEWINDOW_STATE GameWindow_update(GameWindow* self);
void GameWindow_event_record(GameWindow* self, ALLEGRO_EVENT event);

static Interface* _create_Interface(CHILD_INFO type_info);
static void _GameWindow_deal_event(GameWindow* self);

#endif // GAMEWINDOW_H