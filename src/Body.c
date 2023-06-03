// rewrite in c
#include "Body.h"
#include "global.h"
#include "Object.h"
#include <math.h>


Body* new_Body(Pos pos, OBJ_TYPE type, ALLEGRO_BITMAP* image_head,
                      ALLEGRO_BITMAP* image_body_straight,
                      ALLEGRO_BITMAP* image_body_turn,
                      DIRCTION from_dirc,DIRCTION to_dirc) {
    Body* body = (Body*)malloc(sizeof(Body));
    Body_init(body, pos, type, image_head,
                      image_body_straight,
                      image_body_turn,
                      from_dirc,to_dirc);
    return body;
}
void Body_init(Body* body, Pos pos, OBJ_TYPE type, ALLEGRO_BITMAP* image_head,
                      ALLEGRO_BITMAP* image_body_straight,
                      ALLEGRO_BITMAP* image_body_turn,
                      DIRCTION from_dirc,DIRCTION to_dirc)  {
    // inherit from Object
    Object_init((Object*)body, pos, type, image_body_straight, OBJECT_IMAGE_SIZE, OBJECT_IMAGE_SIZE);
    show_msg("Body_init start");
    // Body Info
    body->from_dirc = from_dirc;
    body->to_dirc = to_dirc;
    // Display
    body->image_head = image_head;
    body->image_body_straight = image_body_straight;
    body->image_body_turn = image_body_turn;
    // method
    body->obj.draw = Body_draw;
    show_msg("Body_init finish");
}
void Body_draw(Object* obj) {
    Body* body = (Body*)obj;
    ALLEGRO_BITMAP *img;
    double angle;
    if(obj->type==HEAD) img = body->image_head;
    else if(body->from_dirc==body->to_dirc) img = body->image_body_straight;
    else img = body->image_body_turn;
    if(obj->type==HEAD || body->from_dirc==body->to_dirc) {
        switch(body->to_dirc) {
            case UP:
                angle = 0;
                break;
            case RIGHT:
                angle = M_PI_2;
                break;
            case DOWN:
                angle = M_PI;
                break;
            case LEFT:
                angle = 3*M_PI_2;
                break;
            default: {
                raise_warn("Unknown head direction");
                angle = 0;
            }
        }
    }
    else {
        if       (body->to_dirc==RIGHT&&body->from_dirc==UP)    angle = 0;
        else if  (body->to_dirc==DOWN &&body->from_dirc==RIGHT) angle = M_PI_2;
        else if  (body->to_dirc==LEFT &&body->from_dirc==DOWN)  angle = M_PI;
        else if  (body->to_dirc==UP   &&body->from_dirc==LEFT)  angle = 3*M_PI_2;
        else if  (body->to_dirc==RIGHT&&body->from_dirc==DOWN)  angle = 3*M_PI_2;
        else if  (body->to_dirc==DOWN &&body->from_dirc==LEFT)  angle = 0;
        else if  (body->to_dirc==LEFT &&body->from_dirc==UP)    angle = M_PI_2;
        else if  (body->to_dirc==UP   &&body->from_dirc==RIGHT) angle = M_PI;
        else {
            raise_warn("Unknown body turn direction");
            angle = 0;
        }
    }
    if(img==NULL) {
        raise_warn("try to draw null body");
        return;
    }
    const pos_t img_w = al_get_bitmap_width(img);
    const pos_t img_h = al_get_bitmap_height(img);
    float w_scale = 0.5;
    float h_scale = 1;
    if(angle==0 || angle==M_PI) {
        w_scale = OBJECT_IMAGE_SIZE*CHUNK_WIDTH/img_w;
        h_scale = OBJECT_IMAGE_SIZE*CHUNK_HEIGHT/img_h;
    }
    else {
        w_scale = OBJECT_IMAGE_SIZE*CHUNK_HEIGHT/img_w;
        h_scale = OBJECT_IMAGE_SIZE*CHUNK_WIDTH/img_h;
    }

    al_draw_scaled_rotated_bitmap(img,
                                  img_w/2,img_h/2,
                                  CHUNK_WIDTH*(obj->pos.x - obj->window_x),CHUNK_HEIGHT*(obj->pos.y - obj->window_y),
                                  w_scale,h_scale,
                                  angle,
                                  0
                                 );
}

/*
#include "Body.h"
#include <cmath>

void Body::draw() {
    ALLEGRO_BITMAP *img;
    double angle;
    if(type==HEAD) img = image_head;
    else if(from_dirc==to_dirc) img = image_body_straight;
    else img = image_body_turn;
    if(type==HEAD || from_dirc==to_dirc) {
        switch(to_dirc) {
            case UP:
                angle = 0;
                break;
            case RIGHT:
                angle = M_PI_2;
                break;
            case DOWN:
                angle = M_PI;
                break;
            case LEFT:
                angle = 3*M_PI_2;
                break;
            default: {
                raise_warn("Unknown head direction");
                angle = 0;
            }
        }
    }
    else {
        if       (to_dirc==RIGHT&&from_dirc==UP)    angle = 0;
        else if  (to_dirc==DOWN &&from_dirc==RIGHT) angle = M_PI_2;
        else if  (to_dirc==LEFT &&from_dirc==DOWN)  angle = M_PI;
        else if  (to_dirc==UP   &&from_dirc==LEFT)  angle = 3*M_PI_2;
        else if  (to_dirc==RIGHT&&from_dirc==DOWN)  angle = 3*M_PI_2;
        else if  (to_dirc==DOWN &&from_dirc==LEFT)  angle = 0;
        else if  (to_dirc==LEFT &&from_dirc==UP)    angle = M_PI_2;
        else if  (to_dirc==UP   &&from_dirc==RIGHT) angle = M_PI;
        else {
            raise_warn("Unknown body turn direction");
            angle = 0;
        }
    }
    if(img==nullptr) {
        raise_warn("try to draw null body");
        return;
    }
    const float img_w = al_get_bitmap_width(img);
    const float img_h = al_get_bitmap_height(img);
    float w_scale = 0.5;
    float h_scale = 1;
    if(angle==0||angle==M_PI) {
        w_scale = OBJECT_IMAGE_SIZE*CHUNK_WIDTH/img_w;
        h_scale = OBJECT_IMAGE_SIZE*CHUNK_HEIGHT/img_h;
    }
    else {
        w_scale = OBJECT_IMAGE_SIZE*CHUNK_HEIGHT/img_w;
        h_scale = OBJECT_IMAGE_SIZE*CHUNK_WIDTH/img_h;
    }

    al_draw_scaled_rotated_bitmap(img,
                                  img_w/2,img_h/2,
                                  CHUNK_WIDTH*(pos.second-window_x),CHUNK_HEIGHT*(pos.first-window_y),
                                  w_scale,h_scale,
                                  angle,
                                  0
                                 );
}

bool Body::update() {
    Object::update();
    if(move_dirc!=NONE) {
        move_object(DIRC_TO_POS(move_dirc));
        move_dirc = NONE;
    }
    return true;
}

Body::Body(Pos _pos,OBJ_TYPE T,
           ALLEGRO_BITMAP *img_head,
           ALLEGRO_BITMAP *img_body_straight,
           ALLEGRO_BITMAP *img_body_turn,
           DIRCTION _from,DIRCTION _to):
      Object(_pos,T,img_body_straight,OBJECT_IMAGE_SIZE,OBJECT_IMAGE_SIZE)
{
    show_msg("Create body begin");
    image_head = img_head;
    image_body_straight = img_body_straight;
    image_body_turn = img_body_turn;

    move_dirc = NONE;
    from_dirc = _from;
    to_dirc = _to;
    show_msg("Create body done");
}

Body::~Body() {
    show_msg("Delete body begin");
    show_msg("Delete body done");
}
*/