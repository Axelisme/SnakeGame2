#ifndef ENTITYLIST_H
#define ENTITYLIST_H

// take Entity as node to build a double linked list
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
void EntityList_remove(EntityList* self, Entity* entity);
void EntityList_clear(EntityList* self);
void EntityList_merge(EntityList* self, EntityList* other);

typedef struct EntityArray {
    int size;
    Entity** array;
} EntityArray;

EntityArray* new_EntityArray();
void EntityArray_init(EntityArray* self);
void EntityArray_destroy(EntityArray* self);
void delete_EntityArray(EntityArray* self);
void EntityArray_clear(EntityArray* self);
void EntityArray_push_back(EntityArray* self, Entity* entity);
Entity* EA_get(EntityArray* self, int index);
bool EntityArray_have(EntityArray* self, Entity* entity);

#endif // ENTITYLIST_H