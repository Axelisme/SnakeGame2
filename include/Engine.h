#ifndef ENGINE_H
#define ENGINE_H

#include "Entity/Entity.h"
#include "Entity/EntityMap.h"
#include "Entity/EntityList.h"
#include "Entity/EntityArray.h"

#define INIT_GRAVITY DIRECTION_DOWN

typedef enum OPERATION {
    OP_MOVE_UP,
    OP_MOVE_DOWN,
    OP_MOVE_LEFT,
    OP_MOVE_RIGHT,
    OP_NONE
} OPERATION;

typedef struct MapEngine {
    // Properties
    Pos mapSize;
    // Status
    PLAYER_STATE state;
    Direction gravity;
    // Entities
    EntityList* entities;
    Entity* snake;
} MapEngine;

void MapEngine_init(MapEngine* self, Pos mapSize, EntityList* entities, Entity* snake);
void MapEngine_destroy(MapEngine* self);
void MapEngine_draw(MapEngine* self, ShiftWindow* sw, ALLEGRO_BITMAP* backbuffer);
PLAYER_STATE MapEngine_process(MapEngine* self, OPERATION op);
void ME_setGravity(MapEngine* self, Direction dir);

static void _resetAll(EntityList* entities, Entity* snake);
static bool _canShift(Entity* self, Direction dir, EntityMap* map, EntityArray* shiftObjs);
static void _dealWithOverlap(MapEngine* self, EntityMap* map, EntityArray* overlaps);
static Direction _getDirection(OPERATION op);
static bool _SnakeCanMove(Snake* snake, Direction dir, EntityMap* map, EntityArray* overlaps);
static void _ShiftEntity(EntityArray* shiftObjs, Direction dir, EntityMap* map, EntityArray* overlaps);
static void _removeDiedEntities(MapEngine* self, EntityList* entities, Entity* snake);

#endif // ENGINE_H