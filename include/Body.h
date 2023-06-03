#ifndef BODY_H
#define BODY_H
// rewrite in c
#include "global.h"
#include "Object.h"

typedef struct Body {
    // inherit from Object
    Object obj;
    // Body Info
    DIRCTION from_dirc;
    DIRCTION to_dirc;
    // Display
    ALLEGRO_BITMAP *image_head;
    ALLEGRO_BITMAP *image_body_straight;
    ALLEGRO_BITMAP *image_body_turn;
} Body;

Body* new_Body(Pos, OBJ_TYPE, ALLEGRO_BITMAP*,
                      ALLEGRO_BITMAP*,
                      ALLEGRO_BITMAP*,
                      DIRCTION,DIRCTION);
void Body_init(Body*, Pos, OBJ_TYPE, ALLEGRO_BITMAP*,
                      ALLEGRO_BITMAP*,
                      ALLEGRO_BITMAP*,
                      DIRCTION,DIRCTION);
void Body_draw(Object*);

#endif // BODY_H
/*
#include "global.h"
#include "Object.h"

class Body: public Object {
public:
    void draw() override;
    bool update() override;

    DIRCTION move_dirc = NONE;
    DIRCTION from_dirc;
    DIRCTION to_dirc;

    ALLEGRO_BITMAP *image_head = nullptr;
    ALLEGRO_BITMAP *image_body_straight = nullptr;
    ALLEGRO_BITMAP *image_body_turn = nullptr;

    Body(Pos,OBJ_TYPE,ALLEGRO_BITMAP*,
                      ALLEGRO_BITMAP*,
                      ALLEGRO_BITMAP*,
                      DIRCTION,DIRCTION);
    ~Body();
};
*/

