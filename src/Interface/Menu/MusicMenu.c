
#include "Interface/Menu/MusicMenu.h"
#include "Utility/Position.h"

static const int music_menu_image_num = 4;
static const char MUSIC_MENU_VOLUME_MUTE_IMAGE_PATH[] = "data/image/menu/music_volume_mute.png";
static const char MUSIC_MENU_EXIT_MUTE_IMAGE_PATH[] = "data/image/menu/music_exit_mute.png";
static const char MUSIC_MENU_VOLUME_IMAGE_PATH[] = "data/image/menu/music_volume.png";
static const char MUSIC_MENU_EXIT_IMAGE_PATH[] = "data/image/menu/music_exit.png";
static const char MUSIC_MENU_CURSOR_IMAGE_PATH[] = "data/image/menu/music_cursor.png";
static const char MUSIC_MENU_CURSOR_DARK_IMAGE_PATH[] = "data/image/menu/music_cursor_dark.png";

static const Pos cursor_left = {0.49, 0.34};
static const Pos cursor_right = {0.49, 0.76};
static const Pos cursor_size = {0.3, 0.3};

MusicMenu* new_MusicMenu() {
    MusicMenu* music_menu = (MusicMenu*)al_calloc(1, sizeof(MusicMenu));
    MusicMenu_init(music_menu);
    return music_menu;
}
void MusicMenu_init(MusicMenu *self) {
    if (self == nullptr) {
        raise_warn("try to init a nullptr MusicMenu"); return;}
    Interface* Iself = (Interface*) self;
    Interface_init(Iself);
    Iself->info.type = INTERFACE_MUSIC_MENU;
    Iself->draw = MusicMenu_draw;
    Iself->event_record = MusicMenu_event_record;
    Iself->event_dealer = MusicMenu_deal_event;
    Iself->deleter = delete_MusicMenu;
    // Info
    self->Mute = SE_get_mute();
    self->menu_state = MUSIC_MENU_VOLUME;
    // Image
    self->volume_image = al_load_bitmap(MUSIC_MENU_VOLUME_IMAGE_PATH);
    self->exit_image = al_load_bitmap(MUSIC_MENU_EXIT_IMAGE_PATH);
    self->volume_mute_image = al_load_bitmap(MUSIC_MENU_VOLUME_MUTE_IMAGE_PATH);
    self->exit_mute_image = al_load_bitmap(MUSIC_MENU_EXIT_MUTE_IMAGE_PATH);
    self->music_cursor_image = al_load_bitmap(MUSIC_MENU_CURSOR_IMAGE_PATH);
    self->music_cursor_image_dark = al_load_bitmap(MUSIC_MENU_CURSOR_DARK_IMAGE_PATH);
    if (!self->volume_image) raise_warn("fail to load volume image");
    if (!self->exit_image) raise_warn("fail to load exit image");
    if (!self->volume_mute_image) raise_warn("fail to load volume mute image");
    if (!self->exit_mute_image) raise_warn("fail to load exit mute image");
    if (!self->music_cursor_image) raise_warn("fail to load music cursor image");
    if (!self->music_cursor_image_dark) raise_warn("fail to load music cursor dark image");
}
void MusicMenu_destroy(MusicMenu* self) {
    if (self == nullptr) {raise_warn("try to delete a nullptr MusicMenu");return;}
    // free image
    if (self->volume_image) al_destroy_bitmap(self->volume_image);
    if (self->exit_image) al_destroy_bitmap(self->exit_image);
    if (self->volume_mute_image) al_destroy_bitmap(self->volume_mute_image);
    if (self->exit_mute_image) al_destroy_bitmap(self->exit_mute_image);
    if (self->music_cursor_image) al_destroy_bitmap(self->music_cursor_image);
    if (self->music_cursor_image_dark) al_destroy_bitmap(self->music_cursor_image_dark);
    self->volume_image = nullptr;
    self->exit_image = nullptr;
    self->volume_mute_image = nullptr;
    self->exit_mute_image = nullptr;
    self->music_cursor_image = nullptr;
    self->music_cursor_image_dark = nullptr;
    // inherited from Interface
    Interface* Iself = (Interface*)self;
    Interface_destroy(Iself);
}
void delete_MusicMenu(Interface* Iself) {
    MusicMenu* self = (MusicMenu*)Iself;
    MusicMenu_destroy(self);
    al_free(self);
}
void MusicMenu_draw(Interface* Iself, ALLEGRO_BITMAP* backbuffer) {
    MusicMenu* self = (MusicMenu*)Iself;
    Interface_draw(Iself, backbuffer);
    ALLEGRO_BITMAP* image = nullptr;
    if (self->Mute && self->menu_state == MUSIC_MENU_VOLUME)
        image = self->volume_mute_image;
    else if (self->Mute && self->menu_state == MUSIC_MENU_EXIT)
        image = self->exit_mute_image;
    else if (!self->Mute && self->menu_state == MUSIC_MENU_VOLUME)
        image = self->volume_image;
    else if (!self->Mute && self->menu_state == MUSIC_MENU_EXIT)
        image = self->exit_image;
    draw_image(image, backbuffer, DIRECTION_UP, NO_TRANSPARENT);
    _MusicMenu_draw_cursor(self, backbuffer);
}
void MusicMenu_event_record(Interface* Iself, ALLEGRO_EVENT event) {
    MusicMenu* self = (MusicMenu*)Iself;
    if (self == nullptr) {raise_warn("try to record event on NULL interface");return;}
    if (Iself->info.state != INTERFACE_RUNNING && Iself->info.state != INTERFACE_INITIALING) return;
    if (event.type != ALLEGRO_EVENT_KEY_DOWN) return;
    switch (event.keyboard.keycode) {
        case ALLEGRO_KEY_UP:
        case ALLEGRO_KEY_DOWN:
        case ALLEGRO_KEY_LEFT:
        case ALLEGRO_KEY_RIGHT:
        case ALLEGRO_KEY_ENTER:
        case ALLEGRO_KEY_SPACE:
        case ALLEGRO_KEY_ESCAPE:
            Iself->event = event;
            break;
        default:
            break;
    }
}
void MusicMenu_deal_event(Interface* Iself) {
    MusicMenu* self = (MusicMenu*)Iself;
    self->Mute = SE_get_mute();
    if (Iself->event.type == NO_EVENT) return;
    switch (Iself->event.keyboard.keycode) {
        case ALLEGRO_KEY_UP:
            self->menu_state = MUSIC_MENU_VOLUME;
            break;
        case ALLEGRO_KEY_DOWN:
            self->menu_state = MUSIC_MENU_EXIT;
            break;
        case ALLEGRO_KEY_LEFT:
            if (self->menu_state == MUSIC_MENU_VOLUME)
                SE_set_volume(SE_get_volume() - 0.1);
            break;
        case ALLEGRO_KEY_RIGHT:
            if (self->menu_state == MUSIC_MENU_VOLUME)
                SE_set_volume(SE_get_volume() + 0.1);
            break;
        case ALLEGRO_KEY_SPACE:
        case ALLEGRO_KEY_ENTER:
            _MusicMenu_enter_state(self);
            break;
        case ALLEGRO_KEY_ESCAPE:
            Interface_set_kill(Iself, INTERFACE_NONE);
            break;
        default:
            raise_warn("try to deal with unknown event");
            break;
    }
    Iself->event.type = NO_EVENT;
}

static ALLEGRO_BITMAP* _MusicMenu_current_image(MusicMenu* self) {
    MultiMenu* Mself = (MultiMenu*) self;
    return Mself->images[self->menu_state];
}
static void _MusicMenu_enter_state(MusicMenu* self) {
    switch (self->menu_state) {
        case MUSIC_MENU_VOLUME:
            SE_toggle_mute();
            break;
        case MUSIC_MENU_EXIT:
            Interface_set_kill((Interface*)self, INTERFACE_NONE);
            break;
        default:
            break;
    }
}
static void _MusicMenu_draw_cursor(MusicMenu* self, ALLEGRO_BITMAP* backbuffer) {
    float volume = SE_get_volume();
    Pos cursor_pos = add(mul_const(sub(cursor_right, cursor_left), volume), cursor_left);
    Pos cursor_UL = sub(cursor_pos, mul_const(cursor_size, 0.5));
    Pos cursor_UL_pixel = POS_TO_PIXEL(cursor_UL, backbuffer);
    Pos cursor_size_pixel = POS_TO_PIXEL(cursor_size, backbuffer);
    ALLEGRO_BITMAP* sub_buffer = al_create_sub_bitmap(backbuffer, cursor_UL_pixel.x, cursor_UL_pixel.y, cursor_size_pixel.x, cursor_size_pixel.y);
    ALLEGRO_BITMAP* image = (self->menu_state == MUSIC_MENU_VOLUME) ? self->music_cursor_image : self->music_cursor_image_dark;
    draw_image(image, sub_buffer, DIRECTION_UP, NO_TRANSPARENT);
}