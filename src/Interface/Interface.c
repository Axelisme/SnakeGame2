
#include "Interface/Interface.h"

Interface* new_Interface() {
    Interface* interface = (Interface*)al_calloc(1,sizeof(Interface));
    Interface_init(interface);
    return interface;
}
void Interface_init(Interface* self) {
    // Info
    self->type = INTERFACE_BASIC;
    // Display
    self->background_light = MIN_LIGHT;
    self->background_light_up_step = BASIC_LIGHT_STEP;
    self->background_light_down_step = BASIC_LIGHT_STEP;
    // state
    self->state = INTERFACE_INITIALING;
    self->next_interface = INTERFACE_NONE;
    // event
    self->event.type = -1;
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
    if (self->state == INTERFACE_DIED) return;
    al_set_target_bitmap(backbuffer);
    al_clear_to_color(al_map_rgb(self->background_light, self->background_light, self->background_light));
}
INTERFACE_STATE Interface_update(Interface* self) {
    if (self == nullptr) {raise_warn("try to update NULL interface");return INTERFACE_DIED;}
    if (self->state == INTERFACE_DIED) return INTERFACE_DIED;
    switch (self->state) {
        case INTERFACE_INITIALING:
            if (_Interface_update_light(self, 1))
                self->state = INTERFACE_RUNING;
            break;
        case INTERFACE_RUNING:
            _Interface_deal_event(self);
            self->state = INTERFACE_EXITING;
            break;
        case INTERFACE_EXITING:
            if (_Interface_update_light(self, -1))
                self->state = INTERFACE_DIED;
            break;
        case INTERFACE_STOP:
            self->state = INTERFACE_RUNING;
            break;
        case INTERFACE_DIED:
            break;
        default:
            raise_err("unknown interface state");
            break;
    }
    return self->state;
}
void Interface_event_record(Interface* self, ALLEGRO_EVENT event) {
    if (self == nullptr) {raise_warn("try to record event on NULL interface");return;}
    self->event.type = -1;
    switch (self->state) {
        case INTERFACE_INITIALING:
        case INTERFACE_RUNING:
        case INTERFACE_EXITING:
            self->event = event;
            break;
        case INTERFACE_STOP:
        case INTERFACE_DIED:
            raise_warn("try to record event on stop or died interface");
            break;
        default:
            raise_warn("unknown interface state while recording event");
            break;
    }
}

bool _Interface_update_light(Interface* self, int step) {
    if (step > 0) {
        show_msg("interface light up");
        self->background_light += self->background_light_up_step;
        if (self->background_light > MAX_LIGHT) {
            self->background_light = MAX_LIGHT;
            return true;
        }
        else return false;
    } else if (step < 0) {
        show_msg("interface light down");
        self->background_light -= self->background_light_down_step;
        if (self->background_light < MIN_LIGHT) {
            self->background_light = MIN_LIGHT;
            return true;
        }
        else return false;
    }
    return false;
}
static void _Interface_deal_event(Interface* self) {
    if (self->event.type == -1) return;
    switch(self->event.type) {
        case ALLEGRO_EVENT_KEY_DOWN: {
            switch (self->event.keyboard.keycode) {
                case ALLEGRO_KEY_ESCAPE:
                    self->state = INTERFACE_DIED;
                    break;
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }
    self->event.type = -1;
}
void _draw_image(ALLEGRO_BITMAP* image, ALLEGRO_BITMAP* backbuffer) {
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

