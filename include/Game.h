#ifndef GAME_H
#define GAME_H

#include "GameWindow.h"
#define NAME_MAX_LENGTH 1024
#define UPDATE_FPS 60
#define RENDER_FPS 30

typedef struct Game {
    // Info
    char name[NAME_MAX_LENGTH];
    // game body
    GameWindow window;
    // event
    ALLEGRO_EVENT_QUEUE *event_queue;
    // Timer
    ALLEGRO_TIMER *update_timer;
    ALLEGRO_TIMER *render_timer;
} Game;

Game* new_Game();
void Game_init(Game* self);
void Allegro_init();
void Game_destroy(Game* self);
void delete_Game(Game* self);
void Game_run(Game* self);

#endif // GAME_H