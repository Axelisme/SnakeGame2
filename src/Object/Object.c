
#include "Object/Object.h"
#include "Interface/Interface.h"

// Image
static const char edge_img_path[]                = "data/image/edge.png";
static const char apple_img_path[]               = "data/image/apple.png";
static const char button_img_path[]              = "data/image/button.png";
static const char end_img_path[]                 = "data/image/end.png";
static const char ground_img_path[]              = "data/image/ground.png";
static const char snake_head_img_path[]          = "data/image/snakeHead.png";
static const char dead_snake_head_img_path[]     = "data/image/deadSnakeHead.png";
static const char snake_body_straight_img_path[] = "data/image/snakeBodyStraight.png";
static const char snake_body_turn_img_path[]     = "data/image/snakeBodyTurn.png";
static const char snake_tail_img_path[]          = "data/image/snakeTail.png";
static const char spike_img_path[]               = "data/image/spike.png";
static const char stone_img_path[]               = "data/image/stone.png";
static const char box_img_path[]                 = "data/image/box.png";
ALLEGRO_BITMAP* edge_img = nullptr;
ALLEGRO_BITMAP* apple_img = nullptr;
ALLEGRO_BITMAP* button_img = nullptr;
ALLEGRO_BITMAP* end_img = nullptr;
ALLEGRO_BITMAP* ground_img = nullptr;
ALLEGRO_BITMAP* snake_head_img = nullptr;
ALLEGRO_BITMAP* dead_snake_head_img = nullptr;
ALLEGRO_BITMAP* snake_body_straight_img = nullptr;
ALLEGRO_BITMAP* snake_body_turn_img = nullptr;
ALLEGRO_BITMAP* snake_tail_img = nullptr;
ALLEGRO_BITMAP* spike_img = nullptr;
ALLEGRO_BITMAP* stone_img = nullptr;
ALLEGRO_BITMAP* box_img = nullptr;

void ObjectClass_init() {
    ObjectClass_destroy();
    show_msg("ObjectClass_init");
    edge_img = al_load_bitmap(edge_img_path);
    apple_img = al_load_bitmap(apple_img_path);
    button_img = al_load_bitmap(button_img_path);
    end_img = al_load_bitmap(end_img_path);
    ground_img = al_load_bitmap(ground_img_path);
    snake_head_img = al_load_bitmap(snake_head_img_path);
    dead_snake_head_img = al_load_bitmap(dead_snake_head_img_path);
    snake_body_straight_img = al_load_bitmap(snake_body_straight_img_path);
    snake_body_turn_img = al_load_bitmap(snake_body_turn_img_path);
    snake_tail_img = al_load_bitmap(snake_tail_img_path);
    spike_img = al_load_bitmap(spike_img_path);
    stone_img = al_load_bitmap(stone_img_path);
    box_img = al_load_bitmap(box_img_path);
    if (!edge_img)                  raise_warn("cat not load edge_img!");
    if (!apple_img)                 raise_warn("cat not load apple_img!");
    if (!button_img)                raise_warn("cat not load button_img!");
    if (!end_img)                   raise_warn("cat not load end_img!");
    if (!ground_img)                raise_warn("cat not load ground_img!");
    if (!snake_head_img)            raise_warn("cat not load snake_head_img!");
    if (!dead_snake_head_img)       raise_warn("cat not load dead_snake_head_img!");
    if (!snake_body_straight_img)   raise_warn("cat not load snake_body_straight_img!");
    if (!snake_body_turn_img)       raise_warn("cat not load snake_body_turn_img!");
    if (!snake_tail_img)            raise_warn("cat not load snake_tail_img!");
    if (!spike_img)                 raise_warn("cat not load spike_img!");
    if (!stone_img)                 raise_warn("cat not load stone_img!");
    if (!box_img)                   raise_warn("cat not load box_img!");
}
void ObjectClass_destroy() {
    show_msg("ObjectClass_destroy");
    if (edge_img) al_destroy_bitmap(edge_img);
    if (apple_img) al_destroy_bitmap(apple_img);
    if (button_img) al_destroy_bitmap(button_img);
    if (end_img) al_destroy_bitmap(end_img);
    if (ground_img) al_destroy_bitmap(ground_img);
    if (snake_head_img) al_destroy_bitmap(snake_head_img);
    if (dead_snake_head_img) al_destroy_bitmap(dead_snake_head_img);
    if (snake_body_straight_img) al_destroy_bitmap(snake_body_straight_img);
    if (snake_body_turn_img) al_destroy_bitmap(snake_body_turn_img);
    if (snake_tail_img) al_destroy_bitmap(snake_tail_img);
    if (spike_img) al_destroy_bitmap(spike_img);
    if (stone_img) al_destroy_bitmap(stone_img);
    if (box_img) al_destroy_bitmap(box_img);
    edge_img = nullptr;
    apple_img = nullptr;
    button_img = nullptr;
    end_img = nullptr;
    ground_img = nullptr;
    snake_head_img = nullptr;
    dead_snake_head_img = nullptr;
    snake_body_straight_img = nullptr;
    snake_body_turn_img = nullptr;
    snake_tail_img = nullptr;
    spike_img = nullptr;
    stone_img = nullptr;
    box_img = nullptr;
}

Object* new_Object(Pos pos) {
    Object* object = (Object*) al_calloc(1, sizeof(Object));
    Object_init(object, pos);
    return object;
}
void Object_init(Object* self, Pos pos) {
    // Info
    self->pos = pos;
    self->dir = DIRECTION_UP;
    // Display
    self->Image = nullptr;
    self->viewSize = make_Pos(3, 3);
    // method
    self->draw = Object_draw;
    self->shift = Object_shift;
    self->copy = Object_copy;
    self->deleter = delete_Object;
}
void Object_destroy(Object* self) {
    return;
}
void delete_Object(Object* self) {
    Object_destroy(self);
    al_free(self);
}
Object* Object_copy(Object* self) {
    Object* new_obj = new_Object(self->pos);
    new_obj->dir = self->dir;
    new_obj->Image = self->Image;
    new_obj->viewSize = self->viewSize;
    return new_obj;
}
void Object_draw(Object* self, ShiftWindow* sw, ALLEGRO_BITMAP* backbuffer) {
    if (!SW_isInWindow(sw, self->pos,1)) return;
    Pos ULShift = mul_const(add_const(self->viewSize, -1), -0.5);
    Pos LRShift = mul_const(add_const(self->viewSize,  1), 0.5);
    Pos UL = SW_getPixelPos(sw, add(self->pos, ULShift), backbuffer);
    Pos LR = SW_getPixelPos(sw, add(self->pos, LRShift), backbuffer);
    ALLEGRO_BITMAP* submap = al_create_sub_bitmap(backbuffer, UL.x-2, UL.y-2, LR.x-UL.x+4, LR.y-UL.y+4);
    al_set_target_bitmap(submap);
    if (self->Image) draw_image(self->Image, submap, self->dir, NO_TRANSPARENT);
    else al_clear_to_color(RED);
    al_set_target_bitmap(backbuffer);
}
void Object_shift(Object* self, Pos delta) {
    self->pos = add(self->pos, delta);
}