
#include "Object/Object.h"
#include "Interface/Interface.h"

// Image
static const char edge_img_path[]                = "data/image/edge.png";
static const char apple_img_path[]               = "data/image/apple.png";
static const char button_img_path[]              = "data/image/button.png";
static const char end_img_path[]                 = "data/image/end.png";
static const char ground_img_path[]              = "data/image/ground.png";
static const char snake_head_img_path[]          = "data/image/snakeHead.png";
static const char snake_body_straight_img_path[] = "data/image/snakeBodyStraight.png";
static const char snake_body_turn_img_path[]     = "data/image/snakeBodyTurn.png";
static const char snake_tail_img_path[]          = "data/image/snakeTail.png";
static const char spike_img_path[]               = "data/image/spike.png";
static const char stone_img_path[]               = "data/image/stone.png";
ALLEGRO_BITMAP* edge_img = nullptr;
ALLEGRO_BITMAP* apple_img = nullptr;
ALLEGRO_BITMAP* button_img = nullptr;
ALLEGRO_BITMAP* end_img = nullptr;
ALLEGRO_BITMAP* ground_img = nullptr;
ALLEGRO_BITMAP* snake_head_img = nullptr;
ALLEGRO_BITMAP* snake_body_straight_img = nullptr;
ALLEGRO_BITMAP* snake_body_turn_img = nullptr;
ALLEGRO_BITMAP* snake_tail_img = nullptr;
ALLEGRO_BITMAP* spike_img = nullptr;
ALLEGRO_BITMAP* stone_img = nullptr;

void ObjectClass_init() {
    ObjectClass_destroy();
    edge_img = al_load_bitmap(edge_img_path);
    apple_img = al_load_bitmap(apple_img_path);
    button_img = al_load_bitmap(button_img_path);
    end_img = al_load_bitmap(end_img_path);
    ground_img = al_load_bitmap(ground_img_path);
    snake_head_img = al_load_bitmap(snake_head_img_path);
    snake_body_straight_img = al_load_bitmap(snake_body_straight_img_path);
    snake_body_turn_img = al_load_bitmap(snake_body_turn_img_path);
    snake_tail_img = al_load_bitmap(snake_tail_img_path);
    spike_img = al_load_bitmap(spike_img_path);
    stone_img = al_load_bitmap(stone_img_path);
}
void ObjectClass_destroy() {
    if (edge_img) al_destroy_bitmap(edge_img);
    if (apple_img) al_destroy_bitmap(apple_img);
    if (button_img) al_destroy_bitmap(button_img);
    if (end_img) al_destroy_bitmap(end_img);
    if (ground_img) al_destroy_bitmap(ground_img);
    if (snake_head_img) al_destroy_bitmap(snake_head_img);
    if (snake_body_straight_img) al_destroy_bitmap(snake_body_straight_img);
    if (snake_body_turn_img) al_destroy_bitmap(snake_body_turn_img);
    if (snake_tail_img) al_destroy_bitmap(snake_tail_img);
    if (spike_img) al_destroy_bitmap(spike_img);
    if (stone_img) al_destroy_bitmap(stone_img);
    edge_img = nullptr;
    apple_img = nullptr;
    button_img = nullptr;
    end_img = nullptr;
    ground_img = nullptr;
    snake_head_img = nullptr;
    snake_body_straight_img = nullptr;
    snake_body_turn_img = nullptr;
    snake_tail_img = nullptr;
    spike_img = nullptr;
    stone_img = nullptr;
}

Object* new_Object(Pos pos) {
    Object* object = (Object*) al_calloc(1, sizeof(Object));
    Object_init(object, pos);
    return object;
}
void Object_init(Object* self, Pos pos) {
    show_msg("Object_init");
    self->pos = pos;
    self->dir = DIRECTION_UP;
    self->Image = nullptr;
    self->draw = Object_draw;
    self->shift = Object_shift;
    self->copy = Object_copy;
    self->deleter = delete_Object;
}
void Object_destroy(Object* self) {
    show_msg("Object_destroy");
}
void delete_Object(Object* self) {
    Object_destroy(self);
    al_free(self);
}
Object* Object_copy(Object* self) {
    Object* new_obj = new_Object(self->pos);
    new_obj->dir = self->dir;
    new_obj->Image = self->Image;
    return new_obj;
}
void Object_draw(Object* self, ShiftWindow* sw, ALLEGRO_BITMAP* backbuffer) {
    if (!SW_isInWindow(sw, self->pos,1)) return;
    Pos UL = SW_getPixelPos(sw, add_const(self->pos, -1), backbuffer);
    Pos LR = SW_getPixelPos(sw, add_const(self->pos,  2), backbuffer);
    ALLEGRO_BITMAP* submap = al_create_sub_bitmap(backbuffer, UL.x, UL.y, LR.x-UL.x, LR.y-UL.y);
    al_set_target_bitmap(submap);
    if (self->Image) draw_image(self->Image, submap, self->dir);
    else al_clear_to_color(RED);
    al_set_target_bitmap(backbuffer);
}
void Object_shift(Object* self, Pos delta) {
    self->pos = add(self->pos, delta);
}