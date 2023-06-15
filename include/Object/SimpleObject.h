#ifndef SIMPLE_OBJECT_H
#define SIMPLE_OBJECT_H

#include "Object/Object.h"

typedef Object EdgeObject;
EdgeObject* new_EdgeObject(Pos pos);

typedef Object AppleObject;
AppleObject* new_AppleObject(Pos pos);

typedef Object ButtonObject;
ButtonObject* new_ButtonObject(Pos pos);

typedef Object EndObject;
EndObject* new_EndObject(Pos pos);

typedef Object GroundObject;
GroundObject* new_GroundObject(Pos pos);

typedef Object SpikeObject;
SpikeObject* new_SpikeObject(Pos pos);

typedef Object StoneObject;
StoneObject* new_StoneObject(Pos pos);

#endif // SIMPLE_OBJECT_H