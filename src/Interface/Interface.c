
#include "Interface/Interface.h"

INTERFACE_INFO _default_info() {
    INTERFACE_INFO info;
    info.type = INTERFACE_BASIC;
    info.state = INTERFACE_INITIALING;
    info.child.next_interface = INTERFACE_NONE;
    info.child.level = 0;
    return info;
}
INTERFACE_INFO _fall_back_info() {
    INTERFACE_INFO info;
    info.state = INTERFACE_DIED;
    info.child.next_interface = INTERFACE_NONE;
    return info;
}

Interface* new_Interface() {
    Interface* interface = (Interface*)al_calloc(1,sizeof(Interface));
    Interface_init(interface);
    return interface;
}
void Interface_init(Interface* self) {
    if (self == nullptr) {raise_warn("try to init NULL interface");return;}
    show_msg("init interface");
    // Info
    self->info = _default_info();
    self->should_kill = true;
    // Display
    self->background_light = MIN_LIGHT;
    self->background_light_up_step = BASIC_LIGHT_STEP;
    self->background_light_down_step = BASIC_LIGHT_STEP;
    // event
    self->event.type = NO_EVENT;
    // methods
    self->draw = Interface_draw;
    self->update = Interface_update;
    self->event_record = Interface_event_record;
    self->deleter = Interface_destroy;
}
void Interface_destroy(Interface* self) {
    return;
}
void delete_Interface(Interface* self) {
    Interface_destroy(self);
    al_free(self);
}
void Interface_draw(Interface* self, ALLEGRO_BITMAP* backbuffer) {
    if (self == nullptr) {raise_warn("try to draw NULL interface");return;}
    if (backbuffer == nullptr) {raise_err("try to draw interface on NULL backbuffer");return;}
    if (self->info.state == INTERFACE_DIED) return;
    al_set_target_bitmap(backbuffer);
    al_clear_to_color(al_map_rgb(self->background_light, self->background_light, self->background_light));
}
INTERFACE_INFO Interface_update(Interface* self) {
    if (self == nullptr) {raise_warn("try to update NULL interface");return _fall_back_info();}
    switch (self->info.state) {
        case INTERFACE_INITIALING:
            if (Interface_update_light(self, 1))
                self->info.state = INTERFACE_RUNING;
            break;
        case INTERFACE_RUNING:
            _Interface_deal_event(self);
            break;
        case INTERFACE_EXITING:
            if (Interface_update_light(self, -1))
                self->info.state = (self->should_kill)? INTERFACE_DIED: INTERFACE_STOP;
            break;
        case INTERFACE_STOP:
            self->info.state = INTERFACE_RUNING;
            break;
        case INTERFACE_DIED:
            break;
        default:
            raise_err("unknown interface state");
            break;
    }
    return self->info;
}
void Interface_event_record(Interface* self, ALLEGRO_EVENT event) {
    if (self == nullptr) {raise_warn("try to record event on NULL interface");return;}
    if (self->info.state != INTERFACE_RUNING && self->info.state != INTERFACE_INITIALING) return;
    if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        self->event = event;
}
bool Interface_update_light(Interface* self, int step) {
    if (step > 0) {
        self->background_light += self->background_light_up_step;
        if (self->background_light > MAX_LIGHT) {
            self->background_light = MAX_LIGHT;
            return true;
        }
        else return false;
    } else if (step < 0) {
        self->background_light -= self->background_light_down_step;
        if (self->background_light < MIN_LIGHT) {
            self->background_light = MIN_LIGHT;
            return true;
        }
        else return false;
    }
    return false;
}
void draw_image(ALLEGRO_BITMAP* image, ALLEGRO_BITMAP* backbuffer) {
    if (image == nullptr) {raise_warn("try to draw NULL image");return;}
    al_set_target_bitmap(backbuffer);
    // get screen size
    int screen_w = al_get_bitmap_width(backbuffer);
    int screen_h = al_get_bitmap_height(backbuffer);
    // get image size
    int image_w = al_get_bitmap_width(image);
    int image_h = al_get_bitmap_height(image);
    // draw
    al_draw_scaled_bitmap(image, 0, 0, image_w, image_h, 0, 0, screen_w, screen_h, 0);
}

static void _Interface_deal_event(Interface* self) {
    if (self->event.type != ALLEGRO_EVENT_KEY_DOWN) return;
    if (self->event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
        self->info.state = INTERFACE_EXITING;
        self->info.child.next_interface = INTERFACE_NONE;
        self->should_kill = true;
    }
    self->event.type = NO_EVENT;
}

