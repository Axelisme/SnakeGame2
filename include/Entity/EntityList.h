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
void EntityList_push_back(EntityList* self, Entity* entity);
void EntityList_push_front(EntityList* self, Entity* entity);
void EntityList_pop_back(EntityList* self);
void EntityList_pop_front(EntityList* self);
void EntityList_from_array(EntityList* self, EntityArray* array);
void EntityList_remove(EntityList* self, Entity* entity);
void EntityList_clear(EntityList* self);
void EntityList_merge(EntityList* self, EntityList* other);
bool EL_is_empty(EntityList* self);
int EL_len(EntityList* self);

#endif // ENTITYLIST_H