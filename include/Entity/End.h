#ifndef END_H
#define END_H

#include "Entity.h"
#include "Object/SimpleObject.h"

typedef Entity End;

End* new_End(ObjectVector* objs);
static void End_init(End* self, ObjectVector* objs);

static void End_trigger(Entity* Eself, MapEngine* Engine, EntityMap* Map, EntityArray* overlaps);

#endif // END_H

