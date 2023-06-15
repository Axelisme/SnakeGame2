
#include "global.h"
#include "Entity/Entity.h"
#include "Entity/EntityList.h"
#include "Engine.h"

Entity* new_Entity(Object** objs, int num) {
    Entity* self = (Entity*)al_calloc(1,sizeof(Entity));
    Entity_init(self, objs, num);
    return self;
}
void Entity_init(Entity* self, Object** objs, int num) {
    // Properties
    self->type = E_BASIC;
    self->isFixed = true;
    self->canOverlap = false;
    // Status
    self->activators = new_EntityArray();
    Entity_status_reset(self);
    self->AliveAfterTrigger = false;
    // Objects
    self->objNum = num;
    self->objList = (Object**)al_calloc(num, sizeof(Object*));
    for (int i = 0; i < num; i++)
        self->objList[i] = objs[i]->copy(objs[i]);
    // for EntityList
    self->prev = NULL;
    self->next = NULL;
    // method
    self->reset = Entity_status_reset;
    self->addObject = Entity_addObject;
    self->draw = Entity_draw;
    self->shift = Entity_shift;
    self->trigger = Entity_trigger;
    self->deleter = delete_Entity;
}
void Entity_destroy(Entity* self) {
    show_msg("Entity_destroy");
    // Status
    delete_EntityArray(self->activators);
    // Objects
    if (self->objList) {
        for (int i = 0; i < self->objNum; i++)
            delete_Object(self->objList[i]);
        al_free(self->objList);
    }
    self->objNum = 0;
    self->objList = NULL;
}
void delete_Entity(Entity* self) {
    Entity_destroy(self);
    al_free(self);
}
void Entity_status_reset(Entity* self) {
    self->beSupported = false;
    EntityArray_clear(self->activators);
}
void Entity_addActivator(Entity* self, Entity* activator) {
    if (!EntityArray_have(self->activators, activator))
        EntityArray_push_back(self->activators, activator);
}
void Entity_addObject(Entity* self, Object* obj) {
    self->objNum++;
    self->objList = (Object**)al_realloc(self->objList, sizeof(Object*)*self->objNum);
    self->objList[self->objNum-1] = obj->copy(obj);
}
void Entity_draw(Entity* self, ShiftWindow* sw, ALLEGRO_BITMAP* backbuffer) {
    if (!self->objList) return;
    for (int i = 0; i < self->objNum; i++) {
        Object* obj = self->objList[i];
        obj->draw(obj, sw, backbuffer);
    }
}
void Entity_shift(Entity* self, Direction dir) {
    if (!self->objList) return;
    for (int i = 0; i < self->objNum; i++) {
        Object* obj = self->objList[i];
        obj->shift(obj, DIR_TO_POS(dir));
    }
}
void Entity_trigger(Entity* self, MapEngine* Engine, EntityMap* Map, EntityArray* overlaps) {
    self->AliveAfterTrigger = false;
}