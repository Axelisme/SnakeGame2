#ifndef LEVEL_H
#define LEVEL_H

#include "Interface/Interface.h"

typedef enum LEVEL_ID {
    LEVEL_1 = 1,
    LEVEL_2,
    LEVEL_3,
    LEVEL_4
} LEVEL_ID;

typedef enum LEVEL_STATE {
    LS_PLAYING = 0,
    LS_WIN,
    LS_LOSE
} LEVEL_STATE;

typedef struct Level {
    // Inherited from Interface
    Interface interface;
    // Info
    LEVEL_ID level_id;
} Level;

Level* new_Level(LEVEL_ID level_id);
void Level_init(Level* self, LEVEL_ID level_id);
void Level_destroy(Level* self);
void delete_Level(Interface* Iself);

static void Level_draw(Interface* Iself, ALLEGRO_BITMAP* backbuffer);
static void Level_event_recorder(Interface* Iself, ALLEGRO_EVENT event);
static void Level_event_dealer(Interface* Iself);
static void Level_loader(Level* self, LEVEL_ID level_id);

#endif // LEVEL_H