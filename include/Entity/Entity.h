#ifndef ENTITY_H
#define ENTITY_H

#include "Entity/Object.h"
#include "Utility/ShiftWindow.h"

typedef enum EntityType {
    E_EDGE = -1,
    E_GROUND,
    E_SNAKE,
    E_END,
    E_APPLE,
    E_STONE,
    E_SPIKE,
    E_BUTTON,
    E_BASIC
} EntityType;

typedef struct Entity {
    // Properties
    EntityType type;
    bool isFixed;
    bool canOverlap;
    // Status
    bool beSupported;
    struct Entity* activator;
    // Objects
    int objNum;
    Object** objList;
    // for EntityList
    struct Entity* prev;
    struct Entity* next;
    // method
    void (*setDefault)(struct Entity*);
    void (*addObject)(struct Entity*, Object*);
    void (*draw)(struct Entity*, ShiftWindow*);
    void (*shift)(struct Entity*, Direction);
    void (*trigger)(struct Entity*, void*, void*);
    void (*deleter)(struct Entity*);
} Entity;

Entity* new_Entity();
void Entity_init(Entity* self);
void Entity_destroy(Entity* self);
void delete_Entity(Entity* self);
void Entity_setDefault(Entity* self);
void Entity_addObject(Entity* self, Object* obj);
void Entity_draw(Entity* self, ShiftWindow* sw);
void Entity_shift(Entity* self, Direction dir);
void Entity_trigger(Entity* self, void* Vmap, void* Voverlaps);

#endif // ENTITY_H