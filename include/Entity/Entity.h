#ifndef ENTITY_H
#define ENTITY_H

#include "Object/Object.h"
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

typedef struct EntityMap EntityMap;
typedef struct EntityList EntityList;
typedef struct EntityArray EntityArray;
typedef struct MapEngine MapEngine;

typedef struct Entity {
    // Properties
    EntityType type;
    bool isFixed;
    bool canOverlap;
    // Status
    bool beSupported;
    EntityArray* activators;
    bool AliveAfterTrigger;
    // Objects
    int objNum;
    Object** objList;
    // for EntityList
    struct Entity* prev;
    struct Entity* next;
    // method
    void (*reset)(struct Entity*);
    void (*addObject)(struct Entity*, Object*);
    void (*draw)(struct Entity*, ShiftWindow*, ALLEGRO_BITMAP*);
    void (*shift)(struct Entity*, Direction);
    void (*trigger)(struct Entity*, MapEngine*, EntityMap*, EntityArray*);
    void (*deleter)(struct Entity*);
} Entity;

Entity* new_Entity(Object** objs, int num);
void Entity_init(Entity* self, Object** objs, int num);
void Entity_destroy(Entity* self);
void delete_Entity(Entity* self);
void Entity_status_reset(Entity* self);
void Entity_addActivator(Entity* self, Entity* activator);
void Entity_addObject(Entity* self, Object* obj);
void Entity_draw(Entity* self, ShiftWindow* sw, ALLEGRO_BITMAP* backbuffer);
void Entity_shift(Entity* self, Direction dir);
void Entity_trigger(Entity* self, MapEngine* Engine, EntityMap* Map, EntityArray* overlaps);

#endif // ENTITY_H