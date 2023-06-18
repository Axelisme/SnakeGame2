#ifndef SPIKE_H
#define SPIKE_H

#include "Entity.h"
#include "Object/SimpleObject.h"

typedef Entity Spike;

Spike* new_Spike(ObjectVector* objs);
static void Spike_init(Spike* self, ObjectVector* objs);

static void Spike_trigger(Entity* Eself, MapEngine* Engine, EntityMap* Map, EntityArray* overlaps);

#endif // SPIKE_H