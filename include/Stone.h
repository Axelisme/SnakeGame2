#ifndef STONE_H
#define STONE_H
// rewrite in c
#include "global.h"
#include "Object.h"

typedef struct Stone {
    Object obj;
    bool isFall;
} Stone;

Stone* new_Stone(Pos, ALLEGRO_BITMAP *);
void Stone_init(Stone*, Pos, ALLEGRO_BITMAP *);

#endif // STONE_H
/*
#include "global.h"
#include "Object.h"

class Stone: public Object {
public:
    bool update() override;

    Stone(Pos,ALLEGRO_BITMAP *);

private:
    bool isFall = false;
};
*/