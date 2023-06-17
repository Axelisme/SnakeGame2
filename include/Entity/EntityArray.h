#ifndef ENTITY_ARRAY_H
#define ENTITY_ARRAY_H

#include "Entity/Entity.h"

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
void EntityArray_merge(EntityArray* self, EntityArray* other);
void EntityArray_sort(EntityArray* self);
bool EA_is_empty(EntityArray* self);
int EA_len(EntityArray* self);


#endif // ENTITY_ARRAY_H