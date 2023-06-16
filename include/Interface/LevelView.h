#ifndef LEVEL_H
#define LEVEL_H

#include "Interface/Interface.h"
#include "Utility/ShiftWindow.h"
#include "Engine.h"

#define LEVEL_NUM 4
typedef enum LEVEL_ID {
    LEVEL_1 = 1,
    LEVEL_2,
    LEVEL_3,
    LEVEL_4
} LEVEL_ID;

typedef struct LevelView {
    // Inherited from Interface
    Interface interface;
    // Info
    LEVEL_ID level_id;
    // state
    PLAYER_STATE PState;
    // Display
    ShiftWindow shift_window;
    // Entity
    EntityList entity_list;
    EntityList snakes;
    // Engine
    MapEngine engine;
} LevelView;

LevelView* new_Level(LEVEL_ID level_id);
void Level_init(LevelView* self, LEVEL_ID level_id);
void Level_destroy(LevelView* self);
void delete_Level(Interface* Iself);

static void Level_draw(Interface* Iself, ALLEGRO_BITMAP* backbuffer);
static void Level_event_recorder(Interface* Iself, ALLEGRO_EVENT event);
static void Level_event_dealer(Interface* Iself);
static void Level_deal_win(LevelView* self);
static void Level_deal_lose(LevelView* self);
static void Level_loader(LevelView* self, LEVEL_ID level_id);

#endif // LEVEL_H