#ifndef EDGE_H
#define EDGE_H

#include "Entity/Entity.h"
#include "Object/SimpleObject.h"

typedef Entity Edge;

Edge* new_Edge(ObjectVector* objs);
static void Edge_init(Edge* self, ObjectVector* objs);

static void Edge_trigger(Entity* Eself, MapEngine* Engine, EntityMap* Map, EntityArray* overlaps);

#endif // EDGE_H