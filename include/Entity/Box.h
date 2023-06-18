#ifndef BOX_H
#define BOX_H

#include "Entity/Entity.h"
#include "Object/SimpleObject.h"

typedef Entity Box;

Box* new_Box(ObjectVector* objs);
static void Box_init(Box* self, ObjectVector* objs);

#endif // BOX_H