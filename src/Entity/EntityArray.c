
#include "Entity/EntityArray.h"

EntityArray* new_EntityArray() {
    EntityArray* self = (EntityArray*) al_calloc(1,sizeof(EntityArray));
    EntityArray_init(self);
    return self;
}
void EntityArray_init(EntityArray* self) {
    self->array = NULL;
    self->size = 0;
}
void EntityArray_destroy(EntityArray* self) {
    EntityArray_clear(self);
}
void delete_EntityArray(EntityArray* self) {
    EntityArray_destroy(self);
    al_free(self);
}
void EntityArray_clear(EntityArray* self) {
    al_free(self->array);
    self->array = NULL;
    self->size = 0;
}
void EntityArray_push_back(EntityArray* self, Entity* entity) {
    self->array = al_realloc(self->array, sizeof(Entity*) * (self->size + 1));
    self->array[self->size] = entity;
    self->size++;
}
Entity* EA_get(EntityArray* self, int index) {
    if (index < 0 || index >= self->size) {raise_err("EA_get: index out of range");return NULL;}
    return self->array[index];
}
bool EntityArray_have(EntityArray* self, Entity* entity) {
    for (int i = 0; i < self->size; i++) if (self->array[i] == entity)
            return true;
    return false;
}
void EntityArray_merge(EntityArray* self, EntityArray* other) {
    if (self == other || other->size == 0) return;
    self->array = al_realloc(self->array, sizeof(Entity*) * (self->size + other->size));
    for (int i = 0; i < other->size; i++)
        self->array[self->size + i] = other->array[i];
    self->size += other->size;
    EntityArray_clear(other);
}
bool EA_is_empty(EntityArray* self) {
    return self->size == 0;
}
int EA_len(EntityArray* self) {
    return self->size;
}
