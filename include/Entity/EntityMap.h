#ifndef ENTITYMAP_H
#define ENTITYMAP_H

#include "Entity/Entity.h"
#include "Entity/EntityList.h"
#include "Entity/Snake.h"

typedef enum PLAYER_STATE {
    PLAYING = 0,
    WIN,
    LOSE
} PLAYER_STATE;

typedef struct EntityMap {
    // level state
    PLAYER_STATE levelState;
    // map state
    Pos mapSize;
    Entity*** entityMap;
    // gravity
    Direction gravity;
} EntityMap;

void EntityMap_init(EntityMap* self, Pos mapSize);
void EntityMap_destroy(EntityMap* self);
void EntityMap_clear(EntityMap* self);
bool MapCheck(EntityMap* self, Pos pos);
Entity* MapRps(EntityMap* self, Pos pos, Entity* entity);
Entity* MapGet(EntityMap* self, Pos pos);
void Entity_mark(Entity* self, EntityMap* map, EntityArray* overlaps);
void Entity_unmark(Entity* self, EntityMap* map);

#endif // ENTITYMAP_H