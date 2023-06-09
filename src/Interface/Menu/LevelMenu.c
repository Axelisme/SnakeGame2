
#include "Interface/Menu/LevelMenu.h"

const char LEVEL_MENU_LEVEL_1_IMAGE_PATH[] = "data/image/menu/level_in1.png";
const char LEVEL_MENU_LEVEL_2_IMAGE_PATH[] = "data/image/menu/level_in2.png";
const char LEVEL_MENU_LEVEL_3_IMAGE_PATH[] = "data/image/menu/level_in3.png";
const char LEVEL_MENU_LEVEL_4_IMAGE_PATH[] = "data/image/menu/level_in4.png";
const char LEVEL_MENU_EXIT_IMAGE_PATH[]    = "data/image/menu/level_exit.png";

LevelMenu* new_LevelMenu() {
    LevelMenu* self = (LevelMenu*)al_calloc(1,sizeof(LevelMenu));
    LevelMenu_init(self);
    return self;
}
void LevelMenu_init(LevelMenu* self) {
    if (self == nullptr) {raise_warn("try to initial null LevelMenu");return;}
    Interface* Iself = (Interface*)self;
    show_msg("LevelMenu init");
    // Inherited from Interface
    Interface_init(Iself);
    Iself->info.type = INTERFACE_LEVEL_MENU;
    Iself->draw = LevelMenu_draw;
    Iself->update = LevelMenu_update;
    Iself->event_record = LevelMenu_event_record;
    Iself->event_dealer = LevelMenu_deal_event;
    Iself->deleter = delete_LevelMenu;
    // Info
    self->menu_state = LEVEL_MENU_LEVEL_1;
    // Image
    _LevelMenu_init_image(self);
    _LevelMenu_load_image(self);
}
void LevelMenu_destroy(LevelMenu* self) {
    show_msg("LevelMenu destroy");
    // Inherited from Interface
    Interface* Iself = (Interface*)self;
    Interface_destroy(Iself);
    // free image
    _LevelMenu_init_image(self);
}
void delete_LevelMenu(Interface* Iself) {
    LevelMenu* self = (LevelMenu*)Iself;
    LevelMenu_destroy(self);
    al_free(self);
}
void LevelMenu_draw(Interface* Iself, ALLEGRO_BITMAP* backbuffer) {
    LevelMenu* self = (LevelMenu*)Iself;
    if (self == nullptr) {raise_warn("try to draw null LevelMenu");return;}
    if (backbuffer == nullptr) {raise_warn("try to draw LevelMenu on null backbuffer");return;}
    if (Iself->info.state == INTERFACE_DIED) return;
    // Inherited from Interface
    Interface_draw(Iself, backbuffer);
    // draw image
    ALLEGRO_BITMAP* image = _LevelMenu_current_image(self);
    _draw_image(image, backbuffer);
}
INTERFACE_INFO LevelMenu_update(Interface* Iself) {
    LevelMenu* self = (LevelMenu*)Iself;
    // check validility
    if (self == nullptr) {raise_warn("try to update null LevelMenu");return _fall_back_info();}
    // update by state
    switch (Iself->info.state) {
        case INTERFACE_INITIALING:
            if (_Interface_light_up(Iself))
                Iself->info.state = INTERFACE_RUNING;
            break;
        case INTERFACE_STOP:
            Iself->info.state = INTERFACE_INITIALING;
            break;
        case INTERFACE_RUNING:
            Iself->event_dealer(Iself);
            break;
        case INTERFACE_EXITING:
            if (_Interface_light_down(Iself))
                Iself->info.state = (Iself->should_kill) ? INTERFACE_DIED : INTERFACE_STOP;
            break;
        case INTERFACE_DIED:
            break;
        default:
            raise_warn("LevelMenu update: unexpected state");
            break;
    }
    return Iself->info;
}
void LevelMenu_event_record(Interface* Iself, ALLEGRO_EVENT event) {
    LevelMenu* self = (LevelMenu*)Iself;
    if (self == nullptr) {raise_warn("try to record event on null LevelMenu");return;}
    if (Iself->info.state != INTERFACE_RUNING && Iself->info.state != INTERFACE_INITIALING) return;
    if (event.type != ALLEGRO_EVENT_KEY_DOWN) return;
    switch (event.keyboard.keycode) {
        case ALLEGRO_KEY_UP:
        case ALLEGRO_KEY_DOWN:
        case ALLEGRO_KEY_ENTER:
        case ALLEGRO_KEY_SPACE:
        case ALLEGRO_KEY_ESCAPE:
            Iself->event = event;
            break;
        default:
            break;
    }
}
void LevelMenu_deal_event(Interface* Iself) {
    LevelMenu* self = (LevelMenu*)Iself;
    if (Iself->event.type == NO_EVENT) return;
    if (Iself->event.type != ALLEGRO_EVENT_KEY_DOWN) return;
    switch (Iself->event.keyboard.keycode) {
        case ALLEGRO_KEY_UP:
            self->menu_state = _LevelMenu_prev_state(self);
            break;
        case ALLEGRO_KEY_DOWN:
            self->menu_state = _LevelMenu_next_state(self);
            break;
        case ALLEGRO_KEY_SPACE:
        case ALLEGRO_KEY_ENTER:
            _LevelMenu_enter_state(self);
            break;
        case ALLEGRO_KEY_ESCAPE:
            _LevelMenu_escape(self);
            break;
        default:
            raise_warn("try to deal with unknown event");
            break;
    }
    Iself->event.type = NO_EVENT;
}
static void _LevelMenu_init_image(LevelMenu* self) {
    if (self->level_1_image) al_destroy_bitmap(self->level_1_image);
    if (self->level_2_image) al_destroy_bitmap(self->level_2_image);
    if (self->level_3_image) al_destroy_bitmap(self->level_3_image);
    if (self->level_4_image) al_destroy_bitmap(self->level_4_image);
    if (self->level_exit_image) al_destroy_bitmap(self->level_exit_image);
    self->level_1_image = nullptr;
    self->level_2_image = nullptr;
    self->level_3_image = nullptr;
    self->level_4_image = nullptr;
    self->level_exit_image = nullptr;
}
static void _LevelMenu_load_image(LevelMenu* self) {
    self->level_1_image = al_load_bitmap(LEVEL_MENU_LEVEL_1_IMAGE_PATH);
    self->level_2_image = al_load_bitmap(LEVEL_MENU_LEVEL_2_IMAGE_PATH);
    self->level_3_image = al_load_bitmap(LEVEL_MENU_LEVEL_3_IMAGE_PATH);
    self->level_4_image = al_load_bitmap(LEVEL_MENU_LEVEL_4_IMAGE_PATH);
    self->level_exit_image = al_load_bitmap(LEVEL_MENU_EXIT_IMAGE_PATH);
    if (!(self->level_1_image && self->level_2_image && self->level_3_image && self->level_4_image && self->level_exit_image))
        raise_warn("fail to load some level image");
}
static ALLEGRO_BITMAP* _LevelMenu_current_image(LevelMenu* self) {
    switch (self->menu_state) {
        case LEVEL_MENU_LEVEL_1:
            return self->level_1_image;
        case LEVEL_MENU_LEVEL_2:
            return self->level_2_image;
        case LEVEL_MENU_LEVEL_3:
            return self->level_3_image;
        case LEVEL_MENU_LEVEL_4:
            return self->level_4_image;
        case LEVEL_MENU_EXIT:
            return self->level_exit_image;
        default:
            raise_err("LevelMenu current image: unexpected state");
            return nullptr;
    }
}
static LEVEL_MENU_STATE _LevelMenu_next_state(LevelMenu* self) {
    if (self->menu_state < LEVEL_MENU_EXIT)
        return (LEVEL_MENU_STATE)(self->menu_state + 1);
    return LEVEL_MENU_EXIT;
}
static LEVEL_MENU_STATE _LevelMenu_prev_state(LevelMenu* self) {
    if (self->menu_state > LEVEL_MENU_LEVEL_1)
        return (LEVEL_MENU_STATE)(self->menu_state - 1);
    return LEVEL_MENU_LEVEL_1;
}
static void _LevelMenu_enter_state(LevelMenu* self) {
    Interface* Iself = (Interface*)self;
    switch (self->menu_state) {
        case LEVEL_MENU_LEVEL_1:
        case LEVEL_MENU_LEVEL_2:
        case LEVEL_MENU_LEVEL_3:
        case LEVEL_MENU_LEVEL_4:
            // TODO: enter level
            Iself->should_kill = false;
            Iself->info.state = INTERFACE_EXITING;
            Iself->info.child.next_interface = INTERFACE_BASIC;
            break;
        case LEVEL_MENU_EXIT:
            _LevelMenu_escape(self);
            break;
        default:
            raise_warn("LevelMenu enter state: unexpected state");
            break;
    }
}
static void _LevelMenu_escape(LevelMenu* self) {
    Interface* Iself = (Interface*)self;
    Iself->should_kill = true;
    Iself->info.state = INTERFACE_EXITING;
    Iself->info.child.next_interface = INTERFACE_NONE;
}