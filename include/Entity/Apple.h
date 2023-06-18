#ifndef APPLE_H
#define APPLE_H

#include "Entity.h"
#include "Object/SimpleObject.h"

typedef Entity Apple;

Apple* new_Apple(Pos pos);
void Apple_init(Apple* self, Pos pos);

static void Apple_trigger(Entity* Eself, MapEngine* Engine, EntityMap* Map, EntityArray* overlaps);

#endif // APPLE_H
