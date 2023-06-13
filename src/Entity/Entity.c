
#include "global.h"
#include "Entity/Entity.h"

Entity* new_Entity() {
    Entity* self = (Entity*)al_calloc(1,sizeof(Entity));
    Entity_init(self);
    return self;
}
void Entity_init(Entity* self) {
    self->type = E_BASIC;
    self->isFixed = true;
    self->canOverlap = false;
    Entity_setDefault(self);
    self->objNum = 0;
    self->objList = NULL;
    self->prev = NULL;
    self->next = NULL;
    self->draw = Entity_draw;
    self->trigger = Entity_trigger;
    self->deleter = delete_Entity;
}
void Entity_destroy(Entity* self) {
    show_msg("Entity_destroy");
    if (self->objList) {
        for (int i = 0; i < self->objNum; i++)
            delete_Object(self->objList[i]);
        al_free(self->objList);
    }
}
void delete_Entity(Entity* self) {
    Entity_destroy(self);
    al_free(self);
}
void Entity_setDefault(Entity* self) {
    self->beSupported = false;
    self->activator = NULL;
}
void Entity_addObject(Entity* self, Object* obj) {
    self->objNum++;
    self->objList = (Object**)al_realloc(self->objList, sizeof(Object*)*self->objNum);
    self->objList[self->objNum-1] = obj;
}
void Entity_draw(Entity* self, ShiftWindow* sw) {
    if (!self->objList) return;
    for (int i = 0; i < self->objNum; i++) {
        Object* obj = self->objList[i];
        obj->draw(obj, sw);
    }
}
void Entity_shift(Entity* self, Direction dir) {
    if (!self->objList) return;
    for (int i = 0; i < self->objNum; i++) {
        Object* obj = self->objList[i];
        Pos pos = obj->pos;
        Pos next_pos = heading_pos(pos, dir);
        obj->pos = next_pos;
    }
}
void Entity_trigger(Entity* self, void* Vmap, void* Voverlaps) {
    return;
}