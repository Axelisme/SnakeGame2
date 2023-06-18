#ifndef ENTITYLIST_H
#define ENTITYLIST_H

#include "Entity/Entity.h"

typedef struct EntityList {
    int size;
    Entity* front;
    Entity* back;
} EntityList;

void EntityList_init(EntityList* self);
void EntityList_destroy(EntityList* self);
void EntityList_insert(EntityList* self, Entity* entity);
void EntityList_remove(EntityList* self, Entity* entity);
void EntityList_clear(EntityList* self);
void EntityList_merge(EntityList* self, EntityList* other);
bool EL_is_empty(EntityList* self);
int EL_len(EntityList* self);

#endif // ENTITYLIST_H