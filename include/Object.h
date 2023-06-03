#ifndef OBJECT_H
#define OBJECT_H

// rewrite in c
#include "global.h"
#include "Position.h"

#define TYPE_NUM 10
typedef enum OBJ_TYPE {
    EDGE = -1,
    AIR,
    GROUND,
    HEAD,
    BODY,
    END,
    APPLE,
    STONE,
    SPIKE,
    BUTTON,
    SPIKE_DOWN
} OBJ_TYPE;

typedef struct Object {
    // base Info
    OBJ_TYPE type;
    Pos pos;
    DIRCTION move_dirc;

    // Display
    pos_t width;
    pos_t height;
    ALLEGRO_BITMAP * Image;
    pos_t image_width;
    pos_t image_height;
    float rotate_angle;
    pos_t window_x;
    pos_t window_y;

    // animation
    int ani_total_count;
    int ani_count;         //count form ani_total_count to 0
    int ani_num;
    int ani_image_idx;     //the current image

    // method
    void (*draw)(struct Object*);
    bool (*update)(struct Object*);
    void (*move)(struct Object*, DIRCTION);
    void (*deleter)(struct Object*);
} Object;

// Object
Object* newObject(Pos, OBJ_TYPE, ALLEGRO_BITMAP*, pos_t, pos_t);
void Object_init(Object*, Pos, OBJ_TYPE, ALLEGRO_BITMAP*, pos_t, pos_t);
void Object_draw(Object*);
bool Object_update(Object*);
void Object_move(Object*, DIRCTION);
void Object_destroy(Object*);
void delete_Object(Object*);

#endif // OBJECT_H

/*
#include "global.h"
#include <vector>

class Object {
public:
    // type
    OBJ_TYPE type = AIR;
    DIRCTION move_dirc = NONE;
    float rotate_angle = 0;
    EFFECT effect = NOEFFECT;
    std::vector<Pos> Effect_pos;
    bool triger = false;

    // image
    ALLEGRO_BITMAP * Image = nullptr;

    // window x and y
    float window_x = 0;
    float window_y = 0;

    virtual void draw();
    virtual bool update();

    float &getw() {return width;}
    float &geth() {return height;}
    Pos &getPos() {return pos;}
    ALLEGRO_BITMAP * &getImg() {return Image;}

    void move_object(Pos dp) {pos.first+=dp.first;pos.second+=dp.second;}
    Pos ConvertIdx(int idx);

    Object(Pos,OBJ_TYPE,ALLEGRO_BITMAP*,float,float);

protected:

    // animation
    int ani_total_count = ANI_TOTAL_COUNT;
    int ani_count = 0;         //count form ani_total_count to 0
    int ani_num = ANI_NUM;
    int ani_image_idx = 0;     //the current image

    // center position
    Pos pos;

    // width and height
    float width = 0;
    float height = 0;

    // image width and height
    float image_width = 0;
    float image_height = 0;

};
*/