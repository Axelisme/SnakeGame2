#ifndef STONE_H
#define STONE_H

#include "Entity/Entity.h"
#include "Object/SimpleObject.h"

typedef Entity Stone;

Stone* new_Stone(Pos pos);
static void Stone_init(Stone* self, Pos pos);

#endif // STONE_H