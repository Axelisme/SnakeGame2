#ifndef STONE_H
#define STONE_H

#include "Entity/Entity.h"
#include "Object/SimpleObject.h"

typedef Entity Stone;

Stone* new_Stone(ObjectVector* objs);
static void Stone_init(Stone* self, ObjectVector* objs);

#endif // STONE_H