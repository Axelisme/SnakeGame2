
#include "Entity/EntityList.h"

void EntityList_init(EntityList* self) {
    self->front = NULL;
    self->back = NULL;
    self->size = 0;
}
void EntityList_destroy(EntityList* self) {
    EntityList_clear(self);
}
void EntityList_push_back(EntityList* self, Entity* entity) {
    if (self->back == NULL) {
        self->front = entity;
        self->back = entity;
    } else {
        self->back->next = entity;
        entity->prev = self->back;
        self->back = entity;
    }
    self->size++;
}
void EntityList_push_front(EntityList* self, Entity* entity) {
    if (self->front == NULL) {
        self->front = entity;
        self->back = entity;
    } else {
        self->front->prev = entity;
        entity->next = self->front;
        self->front = entity;
    }
    self->size++;
}
void EntityList_pop_back(EntityList* self) {
    if (self->back == NULL) {
        raise_warn("EntityList_pop_back: empty list");
        return;
    }
    Entity* old = self->back;
    self->back = old->prev;
    if (self->back != NULL)
        self->back->next = NULL;
    else
        self->front = NULL;
    self->size--;
}
void EntityList_pop_front(EntityList* self) {
    if (self->front == NULL) {
        raise_warn("EntityList_pop_front: empty list");
        return;
    }
    Entity* old = self->front;
    self->front = old->next;
    if (self->front != NULL)
        self->front->prev = NULL;
    else
        self->back = NULL;
    self->size--;
}
void EntityList_remove(EntityList* self, Entity* entity) {
    if (entity->prev != NULL)
        entity->prev->next = entity->next;
    else
        self->front = entity->next;
    if (entity->next != NULL)
        entity->next->prev = entity->prev;
    else
        self->back = entity->prev;
    self->size--;
}
void EntityList_clear(EntityList* self) {
    self->front = NULL;
    self->back = NULL;
    self->size = 0;
}
void EntityList_merge(EntityList* self, EntityList* other) {
    if (self == other)
        return;
    if (other->front == NULL)
        return;
    if (self->front == NULL) {
        self->front = other->front;
        self->back = other->back;
    } else {
        self->back->next = other->front;
        other->front->prev = self->back;
        self->back = other->back;
    }
    self->size += other->size;
    EntityList_clear(other);
}

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