#ifndef BODY_H
#define BODY_H

#include "Object/Object.h"

typedef struct Body {
    // Inherited from Object
    Object object;
    // Body Information
    Direction from;
    Direction to;
} Body;

#endif // BODY_H