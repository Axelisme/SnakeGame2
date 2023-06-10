#ifndef LEVEL_H
#define LEVEL_H

#include "Interface/Interface.h"

typedef enum LEVEL_ID {
    LEVEL_1 = 1,
    LEVEL_2,
    LEVEL_3,
    LEVEL_4
} LEVEL_ID;

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
void Level_draw(Interface* Iself, ALLEGRO_BITMAP* backbuffer);
void Level_event_recorder(Interface* Iself, ALLEGRO_EVENT event);
void Level_event_dealer(Interface* Iself);

#endif // LEVEL_H