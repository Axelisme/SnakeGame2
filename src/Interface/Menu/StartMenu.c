
#include "Interface/Menu/StartMenu.h"

const char START_MENU_START_IMAGE_PATH[] = "data/image/menu/game_menu_start.png";
const char START_MENU_LEVEL_IMAGE_PATH[] = "data/image/menu/game_menu_level.png";
const char START_MENU_MUSIC_IMAGE_PATH[] = "data/image/menu/game_menu_music.png";
const char START_MENU_GUIDE_IMAGE_PATH[] = "data/image/menu/game_menu_guide.png";
const char START_MENU_EXIT_IMAGE_PATH[]  = "data/image/menu/game_menu_exit.png";

StartMenu* new_StartMenu() {
    StartMenu* self = (StartMenu*)al_calloc(1, sizeof(StartMenu));
    StartMenu_init(self);
    return self;
}
void StartMenu_init(StartMenu* self) {
    if (self == nullptr) {raise_warn("try to init NULL interface");return;}
    show_msg("StartMenu_init");
    // inherited from Interface
    Interface* Iself = (Interface*)self;
    Interface_init(Iself);
    Iself->type = INTERFACE_START_MENU;
    Iself->background_light_up_step = 10;
    Iself->draw = StartMenu_draw;
    Iself->update = StartMenu_update;
    Iself->event_record = StartMenu_event_record;
    Iself->deleter = delete_StartMenu;
    // Info
    self->menu_state = START_MENU_STATE_START;
    // Image
    self->start_image = nullptr;
    self->level_image = nullptr;
    self->music_image = nullptr;
    self->guide_image = nullptr;
    self->exit_image = nullptr;
    _StartMenu_load_image(self);
}
void StartMenu_destroy(StartMenu* self) {
    show_msg("StartMenu_destroy");
    // inherited from Interface
    Interface* Iself = (Interface*)self;
    Interface_destroy(Iself);
    // free image
    _StartMenu_init_image(self);
}
void delete_StartMenu(Interface* Iself) {
    StartMenu* self = (StartMenu*)Iself;
    StartMenu_destroy(self);
    al_free(self);
}
void StartMenu_draw(Interface* Iself, ALLEGRO_BITMAP* backbuffer) {
    StartMenu* self = (StartMenu*)Iself;
    if (self == nullptr) {raise_warn("try to draw NULL interface");return;}
    if (backbuffer == nullptr) {raise_warn("try to draw interface on NULL backbuffer");return;}
    if (Iself->state == INTERFACE_DIED) return;
    // inherited from Interface
    Interface_draw(Iself, backbuffer);
    // draw image
    ALLEGRO_BITMAP* image = _StartMenu_current_image(self);
    _draw_image(image, backbuffer);
}
INTERFACE_STATE StartMenu_update(Interface* Iself) {
    StartMenu* self = (StartMenu*)Iself;
    // check validility
    if (self == nullptr) {raise_warn("try to update NULL interface");return INTERFACE_DIED;}
    // update by state
    switch (Iself->state) {
        case INTERFACE_INITIALING:
            if (_Interface_update_light(Iself, 1))
                Iself->state = INTERFACE_RUNING;
            break;
        case INTERFACE_STOP:
            Iself->state = INTERFACE_INITIALING;
            break;
        case INTERFACE_RUNING:
            _StartMenu_deal_event(self);
            break;
        case INTERFACE_EXITING:
            if (_Interface_update_light(Iself, -1))
                Iself->state = (Iself->should_kill)? INTERFACE_DIED: INTERFACE_STOP;
            break;
        case INTERFACE_DIED:
            break;
        default:
            raise_err("unknown interface state");
            break;
    }
    return Iself->state;
}
void StartMenu_event_record(Interface* Iself, ALLEGRO_EVENT event) {
    StartMenu* self = (StartMenu*)Iself;
    if (self == nullptr) {raise_warn("try to record event on NULL interface");return;}
    if (Iself->state != INTERFACE_RUNING) return;
    if (event.type != ALLEGRO_EVENT_KEY_DOWN) return;
    switch (event.keyboard.keycode) {
        case ALLEGRO_KEY_UP:
        case ALLEGRO_KEY_DOWN:
        case ALLEGRO_KEY_ENTER:
        case ALLEGRO_KEY_ESCAPE:
            Iself->event = event;
            break;
        default:
            break;
    }
}

void _StartMenu_deal_event(StartMenu* self) {
    Interface* Iself = (Interface*)self;
    if (Iself->event.type == NO_EVENT) return;
    if (Iself->event.type != ALLEGRO_EVENT_KEY_DOWN) return;
    switch (Iself->event.keyboard.keycode) {
        case ALLEGRO_KEY_UP:
            self->menu_state = _StartMenu_prev_state(self);
            break;
        case ALLEGRO_KEY_DOWN:
            self->menu_state = _StartMenu_next_state(self);
            break;
        case ALLEGRO_KEY_ENTER:
            _StartMenu_enter_state(self);
            break;
        case ALLEGRO_KEY_ESCAPE:
            _StartMenu_escape_state(self);
            break;
        default:
            raise_warn("try to deal with unknown event");
            break;
    }
    Iself->event.type = NO_EVENT;
}
void _StartMenu_init_image(StartMenu* self) {
    if (self->start_image) al_destroy_bitmap(self->start_image);
    if (self->level_image) al_destroy_bitmap(self->level_image);
    if (self->music_image) al_destroy_bitmap(self->music_image);
    if (self->guide_image) al_destroy_bitmap(self->guide_image);
    if (self->exit_image)  al_destroy_bitmap(self->exit_image);
    self->start_image = nullptr;
    self->level_image = nullptr;
    self->music_image = nullptr;
    self->guide_image = nullptr;
    self->exit_image  = nullptr;
}
void _StartMenu_load_image(StartMenu* self) {
    show_msg("_StartMenu_load_image");
    _StartMenu_init_image(self);
    self->start_image = al_load_bitmap(START_MENU_START_IMAGE_PATH);
    self->level_image = al_load_bitmap(START_MENU_LEVEL_IMAGE_PATH);
    self->music_image = al_load_bitmap(START_MENU_MUSIC_IMAGE_PATH);
    self->guide_image = al_load_bitmap(START_MENU_GUIDE_IMAGE_PATH);
    self->exit_image  = al_load_bitmap(START_MENU_EXIT_IMAGE_PATH);
    if (!(self->start_image && self->level_image && self->music_image && self->guide_image && self->exit_image))
        raise_warn("failed to load start menu image!");
}
ALLEGRO_BITMAP* _StartMenu_current_image(StartMenu* self) {
    switch (self->menu_state) {
        case START_MENU_STATE_START:
            return self->start_image;
        case START_MENU_STATE_LEVEL:
            return self->level_image;
        case START_MENU_STATE_MUSIC:
            return self->music_image;
        case START_MENU_STATE_GUIDE:
            return self->guide_image;
        case START_MENU_STATE_EXIT:
            return self->exit_image;
        default:
            raise_err("unknown state");
            return nullptr;
    }
}
START_MENU_STATE _StartMenu_next_state(StartMenu* self) {
    if (self->menu_state < START_MENU_STATE_EXIT)
        return self->menu_state = (START_MENU_STATE)(self->menu_state + 1);
    return START_MENU_STATE_EXIT;
}
START_MENU_STATE _StartMenu_prev_state(StartMenu* self) {
    if (self->menu_state > START_MENU_STATE_START)
        return self->menu_state = (START_MENU_STATE)(self->menu_state - 1);
    return START_MENU_STATE_START;
}
void _StartMenu_enter_state(StartMenu* self) {
    Interface* Iself = (Interface*)self;
    if (self->menu_state == START_MENU_STATE_START) return;
    switch (self->menu_state) {
        case START_MENU_STATE_LEVEL:
            Iself->next_interface = INTERFACE_LEVEL_MENU;
            Iself->should_kill = true;
            Iself->state = INTERFACE_EXITING;
            break;
        case START_MENU_STATE_MUSIC:
            show_msg("TODO: mute or unmute music");
            break;
        case START_MENU_STATE_GUIDE:
            Iself->next_interface = INTERFACE_GUIDE;
            Iself->should_kill = false;
            Iself->state = INTERFACE_EXITING;
            break;
        case START_MENU_STATE_EXIT:
            Iself->next_interface = INTERFACE_NONE;
            Iself->should_kill = true;
            Iself->state = INTERFACE_EXITING;
            break;
        default:
            raise_warn("unknown state");
            break;
    }
}
void _StartMenu_escape_state(StartMenu* self) {
    Interface* Iself = (Interface*)self;
    Iself->next_interface = INTERFACE_NONE;
    Iself->should_kill = true;
    Iself->state = INTERFACE_EXITING;
}