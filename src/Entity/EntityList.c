
#include "Entity/EntityList.h"
#include "Entity/EntityArray.h"

void EntityList_init(EntityList* self) {
    self->front = NULL;
    self->back = NULL;
    self->size = 0;
}
void EntityList_destroy(EntityList* self) {
    for (Entity* cur = self->front; cur != NULL; cur = cur->next)
        cur->deleter(cur);
    EntityList_clear(self);
}
void EntityList_insert(EntityList* self, Entity* entity) {
    // insert to sorted entity list
    if (self->front == NULL) {
        self->front = entity;
        self->back = entity;
    } else {
        Entity* cur = self->front;
        while (cur && Entity_compare(&entity, &cur) > 0)
            cur = cur->next;
        if (cur == NULL) {
            self->back->next = entity;
            entity->prev = self->back;
            self->back = entity;
        } else if (cur->prev == NULL) {
            entity->next = self->front;
            self->front->prev = entity;
            self->front = entity;
        } else {
            entity->prev = cur->prev;
            entity->next = cur;
            cur->prev->next = entity;
            cur->prev = entity;
        }
    }
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

