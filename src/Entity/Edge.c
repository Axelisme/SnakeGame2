
#include "Entity/Edge.h"
#include "Engine.h"

Edge* new_Edge(ObjectVector* objs) {
    Edge* edge = (Edge*) al_calloc(1, sizeof(Edge));
    Edge_init(edge, objs);
    return edge;
}
static void Edge_init(Edge* self, ObjectVector* objs) {
    // Inherited from Entity
    Entity* Eself = (Entity*) self;
    Entity_init(Eself, objs);
    Eself->type = E_EDGE;
    Eself->isFixed = true;
    Eself->canOverlap = true;
    Eself->trigger = Edge_trigger;
}

static void Edge_trigger(Entity* Eself, MapEngine* Engine, EntityMap* Map, EntityArray* overlaps) {
    Edge* self = (Edge*) Eself;
    show_msg("Edge_trigger: delete activators");
    EntityArray* activators = (EntityArray*) self->activators;
    for (int i = 0; i < activators->size; i++) {
        Entity* activator = EA_get(Eself->activators, i);
        activator->Alive = false;
    }
}