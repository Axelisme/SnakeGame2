
#include "Entity/EntityList.h"
#include "Entity/EntityArray.h"

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
void EntityList_from_array(EntityList* self, EntityArray* array) {
    for (int i = 0; i < EA_len(array); i++)
        EntityList_push_back(self, EA_get(array, i));
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
bool EL_is_empty(EntityList* self) {
    return self->front == NULL;
}
int EL_len(EntityList* self) {
    return self->size;
}

