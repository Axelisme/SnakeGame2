#ifndef OBJECT_ARRAY_H
#define OBJECT_ARRAY_H

#include "Object/Object.h"

typedef struct ObjectVector {
    Object** array;
    int size;
} ObjectVector;

ObjectVector* ObjectVector_new();
void ObjectVector_init(ObjectVector* self);
void ObjectVector_destroy(ObjectVector* self);
void delete_ObjectVector(ObjectVector* self);

int len(ObjectVector* self);
bool is_empty(ObjectVector* self);
Object* ObjV_get(ObjectVector* self, int index);
void ObjV_push_back(ObjectVector* self, Object* object);
void ObjV_pop_back(ObjectVector* self);
void ObjV_cat(ObjectVector* self, ObjectVector* other);
void ObjV_clear(ObjectVector* self);

#endif // OBJECT_ARRAY_H