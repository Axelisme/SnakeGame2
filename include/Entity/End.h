#ifndef END_H
#define END_H

#include "Entity.h"
#include "Object/SimpleObject.h"

typedef Entity End;

End* new_End(Pos pos);
static void End_init(End* self, Pos pos);

static void End_trigger(Entity* Eself, MapEngine* Engine, EntityMap* Map, EntityArray* overlaps);

#endif // END_H

