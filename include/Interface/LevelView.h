#ifndef LEVEL_H
#define LEVEL_H

#include "Interface/Interface.h"
#include "Utility/ShiftWindow.h"
#include "Engine.h"
#define UPDATE_ENGINE_PERIOD 3
#define DIED_STEP 4
#define BACKGROUND_MOVE_SPEED make_Pos(1, 0.5)

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
    int update_engine_period;
    Pos background_move_speed;
    // state
    PLAYER_STATE PState;
    int update_engine_count;
    int died_image_opacity;
    int died_image_opacity_step;
    // Display
    ShiftWindow shift_window;
    ALLEGRO_BITMAP* background;
    ALLEGRO_BITMAP* died_image;
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
static INTERFACE_INFO Level_update(Interface* Iself);
static void Level_event_recorder(Interface* Iself, ALLEGRO_EVENT event);
static void Level_event_dealer(Interface* Iself);
static void Level_deal_win(LevelView* self);
static void Level_deal_lose(LevelView* self);
static bool Level_update_counter(LevelView* self);
static bool Level_update_died_opacity(LevelView* self);
static Pos Level_get_view_center(LevelView* self);
static void Level_load_default_level(LevelView* self);
static void Level_loader(LevelView* self, LEVEL_ID level_id);

#endif // LEVEL_H