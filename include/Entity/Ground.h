#ifndef GROUND_H
#define GROUND_H

#include "Entity/Entity.h"
#include "Object/SimpleObject.h"

typedef Entity Ground;

Ground* new_Ground(GroundObject** objs, int num);
void Ground_init(Ground* self, GroundObject** objs, int num);

#endif // GROUND_H