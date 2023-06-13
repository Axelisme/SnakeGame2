
#include "Entity/EntityMap.h"

void EntityMap_init(EntityMap* self, int width, int height) {
    self->levelState = LS_PLAYING;
    self->MapWidth = width;
    self->MapHeight = height;
    self->entityMap = (Entity***) al_calloc(1,sizeof(Entity**) * width);
    for (int i = 0; i < width; i++)
        self->entityMap[i] = al_calloc(1,sizeof(Entity*) * height);
}
void EntityMap_destroy(EntityMap* self) {
    for (int i = 0; i < self->MapWidth; i++)
        al_free(self->entityMap[i]);
    al_free(self->entityMap);
}
Entity* MapRps(EntityMap* self, Pos pos, Entity* entity) {
    if (pos.x < 0 || pos.x >= self->MapWidth || pos.y < 0 || pos.y >= self->MapHeight) {
        raise_warn("MapRps: pos out of range");
        return NULL;
    }
    Entity* old = self->entityMap[(int)pos.x][(int)pos.y];
    self->entityMap[(int)pos.x][(int)pos.y] = entity;
    return old;
}
Entity* MapGet(EntityMap* self, Pos pos) {
    if (pos.x < 0 || pos.x >= self->MapWidth || pos.y < 0 || pos.y >= self->MapHeight) {
        raise_warn("MapGet: pos out of range");
        return NULL;
    }
    return self->entityMap[(int)pos.x][(int)pos.y];
}
void Entity_mark(Entity* self, EntityMap* map, EntityArray* overlaps) {
    if (!self->objList) return;
    for (int i = 0; i < self->objNum; i++) {
        Entity* origin = MapRps(map, self->objList[i]->pos, self);
        EntityArray_push_back(overlaps, origin);
    }
}
void Entity_unmark(Entity* self, EntityMap* map) {
    if (!self->objList) return;
    for (int i = 0; i < self->objNum; i++)
        MapRps(map, self->objList[i]->pos, NULL);
}
bool Entity_canShift(Entity* self, Direction dir, EntityMap* map) {
    if (!self->objList) return false;
    if (self->isFixed || self->beSupported) return false;
    bool canShift = true;
    Entity_unmark(self, map);
    for (int i = 0; i < self->objNum; i++) {
        Pos pos = self->objList[i]->pos;
        Pos next_pos = heading_pos(pos, dir);
        Entity* next_entity = MapGet(map, next_pos);
        if (!next_entity || next_entity->canOverlap) continue;
        if (!Entity_canShift(next_entity, dir, map)) {
            canShift = false;
            break;
        }
    }
    Entity_mark(self, map, NULL);
    self->beSupported = !canShift;
    return canShift;
}