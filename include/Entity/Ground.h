#ifndef GROUND_H
#define GROUND_H

#include "Entity/Entity.h"
#include "Object/SimpleObject.h"

typedef Entity Ground;

Ground* new_Ground(ObjectVector* objs);
void Ground_init(Ground* self, ObjectVector* objs);

#endif // GROUND_H