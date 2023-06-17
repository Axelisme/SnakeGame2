
#include "Interface/Interface.h"
#include "Utility/Direction.h"

INTERFACE_INFO _default_info() {
    INTERFACE_INFO info;
    info.type = INTERFACE_BASIC;
    info.state = INTERFACE_INITIALING;
    info.child.interface_type = INTERFACE_NONE;
    info.child.level = 0;
    return info;
}
INTERFACE_INFO _fall_back_info() {
    INTERFACE_INFO info;
    info.state = INTERFACE_DIED;
    info.child.interface_type = INTERFACE_NONE;
    return info;
}

Interface* new_Interface() {
    Interface* interface = (Interface*)al_calloc(1,sizeof(Interface));
    Interface_init(interface);
    return interface;
}
void Interface_init(Interface* self) {
    if (self == nullptr) {
        raise_warn("try to init NULL interface");
        return;
    }
    show_msg("Interface_init");
    // Info
    self->info = _default_info();
    self->should_kill = true;
    // Display
    self->background_color = WRITE;
    self->background_light = MIN_LIGHT;
    self->background_light_max = MAX_LIGHT;
    self->background_light_min = MIN_LIGHT;
    self->background_light_up_step = BASIC_LIGHT_STEP;
    self->background_light_down_step = BASIC_LIGHT_STEP;
    // event
    self->event.type = NO_EVENT;
    // methods
    self->draw = Interface_draw;
    self->update = Interface_update;
    self->event_record = Interface_event_record;
    self->event_dealer = Interface_deal_event;
    self->deleter = Interface_destroy;
}
void Interface_destroy(Interface* self) {
    show_msg("Interface_destroy");
}
void delete_Interface(Interface* self) {
    Interface_destroy(self);
    al_free(self);
}
void Interface_draw(Interface* self, ALLEGRO_BITMAP* backbuffer) {
    if (self == nullptr) {
        raise_warn("try to draw NULL interface");return;}
    if (backbuffer == nullptr) {
        raise_err("try to draw interface on NULL backbuffer");return;}
    if (self->info.state == INTERFACE_DIED) return;
    al_set_target_bitmap(backbuffer);
    unsigned char r, g, b; al_unmap_rgb(self->background_color, &r, &g, &b);
    r *= self->background_light/255.0;
    g *= self->background_light/255.0;
    b *= self->background_light/255.0;
    al_clear_to_color(al_map_rgb(r, g, b));
}
INTERFACE_INFO Interface_update(Interface* self) {
    if (self == nullptr) {raise_warn("try to update NULL interface");return _fall_back_info();}
    switch (self->info.state) {
        case INTERFACE_INITIALING:
            if (Interface_light_up(self))
                self->info.state = INTERFACE_RUNNING;
            break;
        case INTERFACE_RUNNING:
            self->event_dealer(self);
            break;
        case INTERFACE_EXITING:
            if (Interface_light_down(self))
                self->info.state = (self->should_kill)? INTERFACE_DIED: INTERFACE_STOP;
            break;
        case INTERFACE_STOP:
            self->info.state = INTERFACE_INITIALING;
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
    if (self->info.state != INTERFACE_RUNNING && self->info.state != INTERFACE_INITIALING) return;
    if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        self->event = event;
}
void Interface_deal_event(Interface* self) {
    if (self->event.type != ALLEGRO_EVENT_KEY_DOWN) return;
    Interface_set_kill(self, INTERFACE_NONE);
    self->event.type = NO_EVENT;
}
void Interface_set_kill(Interface* self, INTERFACE_TYPE next_type) {
    self->info.state = INTERFACE_EXITING;
    self->info.child.interface_type = next_type;
    self->should_kill = true;
}
void Interface_set_stop(Interface* self, INTERFACE_TYPE next_type) {
    self->info.state = INTERFACE_EXITING;
    self->info.child.interface_type = next_type;
    self->should_kill = false;
}
bool Interface_light_up(Interface* self) {
    if (self == nullptr) {
        raise_warn("try to light up NULL interface");return true;}
    self->background_light += self->background_light_up_step;
    if (self->background_light > self->background_light_max) {
        self->background_light = self->background_light_max;
        return true;
    }
    else return false;
}
bool Interface_light_down(Interface* self) {
    if (self == nullptr) {
        raise_warn("try to light down NULL interface");return true;}
    self->background_light -= self->background_light_down_step;
    if (self->background_light < self->background_light_min) {
        self->background_light = self->background_light_min;
        return true;
    }
    else return false;
}
void draw_image(ALLEGRO_BITMAP* image, ALLEGRO_BITMAP* backbuffer, Direction direction, unsigned char opacity) {
    if (image == nullptr) {
        raise_warn("try to draw NULL image");return;}
    al_set_target_bitmap(backbuffer);
    // get screen size
    const float screen_w = al_get_bitmap_width(backbuffer);
    const float screen_h = al_get_bitmap_height(backbuffer);
    // get image size
    const float image_w = al_get_bitmap_width(image);
    const float image_h = al_get_bitmap_height(image);
    // get image center position
    const float img_center_x = image_w / 2;
    const float img_center_y = image_h / 2;
    // get screen center position
    const float screen_center_x = screen_w / 2;
    const float screen_center_y = screen_h / 2;
    // get scale
    const float scale_w = (float)screen_w / image_w;
    const float scale_h = (float)screen_h / image_h;
    const float scale_x = (direction == DIRECTION_UP || direction == DIRECTION_DOWN)? scale_w: scale_h;
    const float scale_y = (direction == DIRECTION_UP || direction == DIRECTION_DOWN)? scale_h: scale_w;
    // get angle
    const float angle = Direction_to_angle(direction);
    // get opacity
    const ALLEGRO_COLOR mask = al_map_rgba_f(1, 1, 1, opacity/255.0);
    // draw
    al_draw_tinted_scaled_rotated_bitmap(image, mask, img_center_x, img_center_y, screen_center_x, screen_center_y, scale_x, scale_y, angle, 0);
}


