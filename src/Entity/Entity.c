
#include "global.h"
#include "Entity/Entity.h"
#include "Entity/EntityList.h"
#include "Entity/EntityArray.h"
#include "Engine.h"

Entity* new_Entity(ObjectVector* objs) {
    Entity* self = (Entity*)al_calloc(1,sizeof(Entity));
    Entity_init(self, objs);
    return self;
}
void Entity_init(Entity* self, ObjectVector* objs) {
    // Properties
    self->type = E_BASIC;
    self->isFixed = true;
    self->canOverlap = false;
    // Status
    self->activators = new_EntityArray();
    Entity_status_reset(self);
    self->Alive = true;
    // Objects
    ObjectVector_init(&self->objList);
    ObjV_cat(&self->objList, objs);
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
    self->activators = NULL;
    // Objects
    ObjectVector_destroy(&self->objList);
    // for EntityList
    self->prev = NULL;
    self->next = NULL;
}
void delete_Entity(Entity* self) {
    Entity_destroy(self);
    al_free(self);
}
void Entity_status_reset(Entity* self) {
    self->beSupported = unknown;
    EntityArray_clear(self->activators);
}
void Entity_addActivator(Entity* self, Entity* activator) {
    if (!EntityArray_have(self->activators, activator))
        EntityArray_push_back(self->activators, activator);
}
void Entity_addObject(Entity* self, Object* obj) {
    ObjV_push_back(&self->objList, obj);
}
void Entity_draw(Entity* self, ShiftWindow* sw, ALLEGRO_BITMAP* backbuffer) {
    for (int i = 0; i < len(&self->objList); i++) {
        Object* obj = ObjV_get(&self->objList, i);
        obj->draw(obj, sw, backbuffer);
    }
}
void Entity_shift(Entity* self, Direction dir) {
    if (!self->Alive) return;
    for (int i = 0; i < len(&self->objList) ; i++) {
        Object* obj = ObjV_get(&self->objList, i);
        obj->shift(obj, DIR_TO_POS(dir));
    }
}
void Entity_trigger(Entity* self, MapEngine* Engine, EntityMap* Map, EntityArray* overlaps) {
    self->reset(self);
    self->Alive = false;
}
bool Entity_mark(Entity* self, EntityMap* map, EntityArray* overlaps) {
    if (!self->Alive) return false;
    if (!inMap(self, map->mapSize)) return false;
    for (int i = 0; i < len(&self->objList); i++) {
        Pos pos = ObjV_get(&self->objList, i)->pos;
        Entity* origin = MapRps(map, pos, self);
        if (origin && !EntityArray_have(overlaps, origin))
            EntityArray_push_back(overlaps, origin);
    }
    return true;
}
void Entity_unmark(Entity* self, EntityMap* map) {
    if (!self->Alive) return;
    for (int i = 0; i < len(&self->objList); i++) {
        Pos pos = ObjV_get(&self->objList, i)->pos;
        Entity* origin = MapRps(map, pos, NULL);
        if (origin != self)
            raise_warn("Entity_unmark: unmark on wrong entity");
    }
}
bool inMap(Entity* entity, Pos mapSize) {
    for (int i = 0; i < len(&entity->objList); i++) {
        Pos pos = ObjV_get(&entity->objList, i)->pos;
        if (pos.x < 0 || pos.x >= mapSize.x || pos.y < 0 || pos.y >= mapSize.y)
            return false;
    }
    return true;
}
int Entity_compare(const void* a, const void* b) {
    // sort by type
    Entity* ea = *(Entity**)a;
    Entity* eb = *(Entity**)b;
    return ea->type - eb->type;
}