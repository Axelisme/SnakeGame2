#ifndef ENTITYMAP_H
#define ENTITYMAP_H

#include "Entity/Entity.h"
#include "Entity/EntityList.h"
#include "Entity/Snake.h"
#include "Interface/Level.h"

typedef struct EntityMap {
    // level state
    LEVEL_STATE levelState;
    // map state
    int MapWidth;
    int MapHeight;
    Entity*** entityMap;
} EntityMap;

void EntityMap_init(EntityMap* self, int width, int height);
void EntityMap_destroy(EntityMap* self);
Entity* MapRps(EntityMap* self, Pos pos, Entity* entity);
Entity* MapGet(EntityMap* self, Pos pos);

void Entity_mark(Entity* self, EntityMap* map, EntityArray* overlaps);
void Entity_unmark(Entity* self, EntityMap* map);
bool Entity_canShift(Entity* self, Direction dir, EntityMap* map);

#endif // ENTITYMAP_H