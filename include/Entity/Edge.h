#ifndef EDGE_H
#define EDGE_H

#include "Entity/Entity.h"
#include "Object/SimpleObject.h"

typedef Entity Edge;

Edge* new_Edge(EdgeObject* edges, int num);
void Edge_init(Edge* self, EdgeObject* edges, int num);

void Edge_trigger(Entity* Eself, MapEngine* Engine, EntityMap* Map, EntityArray* overlaps);

#endif // EDGE_H