#ifndef GANEWINDOW_H
#define GANEWINDOW_H

#include "global.h"
#include "Interface/Interface.h"
#define INIT_DISPLAY_WIDTH 800
#define INIT_DISPLAY_HEIGHT 800
#define DISPLAY_FPS 30
#define FIRST_INTERFACE INTERFACE_BASIC
#define INTERFACE_MAX_NUM 10

typedef enum GAMEWINDOW_STATE {
    GAMEWINDOW_RUNING = 1,
    GAMEWINDOW_EXIT = 0
} GAMEWINDOW_STATE;

typedef struct  GameWindow {
    // display
    int width;
    int height;
    ALLEGRO_DISPLAY* display;
    // sound
    ALLEGRO_SAMPLE* background_sample;
    ALLEGRO_SAMPLE_INSTANCE* background_music;
    bool Mute;
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

void _GameWindow_load(GameWindow* self);
Interface* _create_Interface(INTERFACE_TYPE type);
void _GameWindow_deal_event(GameWindow* self);

#endif // GANEWINDOW_H