
#include "Entity/Spike.h"
#include "Engine.h"

Spike* new_Spike(ObjectVector* objs) {
    Spike* spike = (Spike*) al_calloc(1, sizeof(Spike));
    Spike_init(spike, objs);
    return spike;
}
static void Spike_init(Spike* self, ObjectVector* objs) {
    // Inherited from Entity
    Entity* Eself = (Entity*) self;
    Entity_init(Eself, objs);
    show_msg("Spike_init");
    Eself->type = E_SPIKE;
    Eself->isFixed = true;
    Eself->canOverlap = true;
    Eself->trigger = Spike_trigger;
}

static void Spike_trigger(Entity* Eself, MapEngine* Engine, EntityMap* Map, EntityArray* overlaps) {
    if (!Eself->Alive) return;
    Spike* self = (Spike*) Eself;
    EntityArray* activators = (EntityArray*) self->activators;
    for (int i = 0; i < activators->size; i++) {
        Entity* activator = EA_get(Eself->activators, i);
        if (activator->type == E_SNAKE) {
            show_msg("Spike_trigger: kill snake");
            activator->Alive = false;
        }
    }
}