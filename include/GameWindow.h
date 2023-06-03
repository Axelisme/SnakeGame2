#ifndef GANEWINDOW_H
#define GANEWINDOW_H

#include "global.h"
#include "Menu.h"
#include "Level.h"

typedef struct GameWindow {
    // display
    ALLEGRO_DISPLAY* display;
    int width;
    int height;

    // menu
    Menu *menu;

    // level
    Level *level;

    // state
    GAME_STATE state;

    // event
    ALLEGRO_EVENT_QUEUE *event_queue;
    ALLEGRO_EVENT event;

    // Time
    ALLEGRO_TIMER *timer;
} GameWindow;

GameWindow* new_GameWindow();
void GameWindow_init(GameWindow* self);
void GameWindow_draw(GameWindow* self);
bool GameWindow_update(GameWindow* self);
void GameWindow_game_load(GameWindow* self);
void GameWindow_game_play(GameWindow* self);
void GameWindow_game_reset(GameWindow* self);
void GameWindow_game_destroy(GameWindow* self);
void GameWindow_game_begin(GameWindow* self);
void GameWindow_game_process(GameWindow* self);
GameWindow* delete_GameWindow(GameWindow* self);

#endif // GANEWINDOW_H

/*
#include "global.h"
#include "Menu.h"
#include "Level.h"
#include <string>

using namespace std;

class GameWindow {
public:
    static bool Mute;
    static int want_level;

    // Draw function
    void draw();
    // update object
    bool update();

    // game process
    void game_load();
    void game_play();
    void game_reset();
    void game_destroy();

    // Create and destroy
    GameWindow();
    ~GameWindow();

private:
    // inner process
    void game_begin();
    void game_process();

    // display 
    ALLEGRO_DISPLAY* display = nullptr;
    const int width = DISPLAY_WIDTH;
    const int height = DISPLAY_HEIGHT;

    // menu
    Menu *menu = nullptr;

    // level
    Level *level = nullptr;

    // state
    GAME_STATE state = GAME_MENU;

    // event
    ALLEGRO_EVENT_QUEUE *event_queue = nullptr;
    ALLEGRO_EVENT event;

    // Time
    ALLEGRO_TIMER *timer = nullptr;
};
*/