#ifndef SNAKE_H
#define SNAKE_H
// rewrite in c
#include "Body.h"
#include "ObjectQueue.h"
#include "global.h"

typedef struct Snake {
    ObjectQueue *body;
    Pos head;
    DIRCTION move_direction;
    DIRCTION heading;
    bool isFall;
    bool can_eat_apple;
    bool isDied;
    float window_width;
    float window_height;
    ALLEGRO_BITMAP * Image_head;
    ALLEGRO_BITMAP * Image_body_straight;
    ALLEGRO_BITMAP * Image_body_turn;
    ALLEGRO_BITMAP * Image_tail;
} Snake;

Snake* new_Snake(Pos[], int,
                 ALLEGRO_BITMAP *,
                 ALLEGRO_BITMAP *,
                 ALLEGRO_BITMAP *,
                 ALLEGRO_BITMAP *);
void Snake_init(Snake*, Pos[], int,
                 ALLEGRO_BITMAP *,
                 ALLEGRO_BITMAP *,
                 ALLEGRO_BITMAP *,
                 ALLEGRO_BITMAP *);
void Snake_draw(Snake*);
bool Snake_update(Snake*);
Pos Snake_Next_Pos(Snake*);
void Snake_Shorten(Snake*);
void Snake_Move_all(Snake*);
void Snake_Move_forward(Snake*);
void Snake_Move_extend(Snake*);
void Snake_destory(Snake*);
void delete_Snake(Snake*);

#endif // SNAKE_H
/*
#include "global.h"
#include "Object.h"
#include "Body.h"
#include <deque>

class Snake {
public:
    void draw();
    bool update();

    Pos Next_Pos();
    void Shorten();

    Snake(std::vector<Pos>&,ALLEGRO_BITMAP *,
                            ALLEGRO_BITMAP *,
                            ALLEGRO_BITMAP *,
                            ALLEGRO_BITMAP *);
    ~Snake();

    bool isFall;
    bool can_eat_apple;
    bool isDied;
    DIRCTION move_direction;
    DIRCTION heading;

    float window_width = CHUNK_W_NUM;
    float window_height = CHUNK_H_NUM;

    Pos head;

    std::deque<Body*> body;

private:
    void Move_all();
    void Move_forward();
    void Move_extend();

    // image
    ALLEGRO_BITMAP * Image_head = nullptr;
    ALLEGRO_BITMAP * Image_body_straight = nullptr;
    ALLEGRO_BITMAP * Image_body_turn = nullptr;
    ALLEGRO_BITMAP * Image_tail = nullptr;
};
*/
