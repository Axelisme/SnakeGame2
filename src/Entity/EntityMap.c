
#include "Entity/EntityMap.h"

void EntityMap_init(EntityMap* self, Pos mapSize) {
    // map state
    self->mapSize = mapSize;
    self->entityMap = (Entity***) al_calloc(1,sizeof(Entity**) * mapSize.y);
    for (int i = 0; i < mapSize.y; i++)
        self->entityMap[i] = al_calloc(1,sizeof(Entity*) * mapSize.x);
}
void EntityMap_destroy(EntityMap* self) {
    for (int i = 0; i < self->mapSize.y; i++)
        al_free(self->entityMap[i]);
    al_free(self->entityMap);
}
void EntityMap_clear(EntityMap* self) {
    for (int i = 0; i < self->mapSize.y; i++)
        for (int j = 0; j < self->mapSize.x; j++)
            self->entityMap[i][j] = NULL;
}
bool MapCheck(EntityMap* self, Pos pos) {
    return  pos.x >= 0 && pos.x < self->mapSize.x &&
            pos.y >= 0 && pos.y < self->mapSize.y ;
}
Entity* MapRps(EntityMap* self, Pos pos, Entity* entity) {
    if (!MapCheck(self, pos)) {
        raise_warn("MapRps: pos out of range");
        return NULL;
    }
    Entity* old = self->entityMap[(int)pos.y][(int)pos.x];
    self->entityMap[(int)pos.y][(int)pos.x] = entity;
    if (old) Entity_addActivator(old, entity);
    return old;
}
Entity* MapGet(EntityMap* self, Pos pos) {
    if (!MapCheck(self, pos)) {
        raise_warn("MapRps: pos out of range");
        return NULL;
    }
    return self->entityMap[(int)pos.y][(int)pos.x];
}