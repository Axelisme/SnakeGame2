
#include "Entity/Edge.h"
#include "Entity/EntityMap.h"
#include "Entity/EntityList.h"
#include "Engine.h"

Edge* new_Edge(EdgeObject* edges, int num) {
    Edge* edge = (Edge*) al_calloc(1, sizeof(Edge));
    Edge_init(edge, edges, num);
    return edge;
}
void Edge_init(Edge* self, EdgeObject* edges, int num) {
    // Inherited from Entity
    Entity* Eself = (Entity*) self;
    Entity_init(Eself, (Object**)edges, num);
    Eself->type = E_EDGE;
    Eself->isFixed = true;
    Eself->canOverlap = true;
    Eself->AliveAfterTrigger = true;
    Eself->trigger = Edge_trigger;
}

void Edge_trigger(Entity* Eself, void* Vmap, void* VEngine, void* Voverlaps) {
    Edge* self = (Edge*) Eself;
    EntityMap* map = (EntityMap*) Vmap;
    MapEngine* engine = (MapEngine*) VEngine;
    EntityArray* overlaps = (EntityArray*) Voverlaps;
    show_msg("Edge_trigger: delete activators");
    EntityArray* activators = (EntityArray*) self->activators;
    for (int i = 0; i < activators->size; i++) {
        Entity* activator = EA_get(self->activators, i);
        Entity_unmark(activator, map);
        if (activator->type != E_SNAKE) {
            EntityList_remove(engine->entities, activator);
            delete_Entity(activator);
        }
        else engine->state = LOSE;
    }
    self->AliveAfterTrigger = true;
}