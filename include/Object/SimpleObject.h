#ifndef SIMPLE_OBJECT_H
#define SIMPLE_OBJECT_H

#include "Object/Object.h"

// TODO: Add destroy function

typedef Object EdgeObject;
EdgeObject* new_EdgeObject(Pos pos);
void EdgeObject_init(EdgeObject* self, Pos pos);

typedef Object AppleObject;
AppleObject* new_AppleObject(Pos pos);
void AppleObject_init(AppleObject* self, Pos pos);

typedef Object ButtonObject;
ButtonObject* new_ButtonObject(Pos pos);
void ButtonObject_init(ButtonObject* self, Pos pos);

typedef Object EndObject;
EndObject* new_EndObject(Pos pos);
void EndObject_init(EndObject* self, Pos pos);

typedef Object GroundObject;
GroundObject* new_GroundObject(Pos pos);
void GroundObject_init(GroundObject* self, Pos pos);

typedef Object SpikeObject;
SpikeObject* new_SpikeObject(Pos pos);
void SpikeObject_init(SpikeObject* self, Pos pos);

typedef Object StoneObject;
StoneObject* new_StoneObject(Pos pos);
void StoneObject_init(StoneObject* self, Pos pos);

typedef Object BoxObject;
BoxObject* new_BoxObject(Pos pos);
void BoxObject_init(BoxObject* self, Pos pos);

#endif // SIMPLE_OBJECT_H