
#include "GameWindow.h"
#include "Interface/Interface.h"
#include "Interface/Menu/InMenu.h"
#include "Interface/Menu/StartMenu.h"
#include "Interface/Menu/GuideMenu.h"
#include "Interface/Menu/LevelMenu.h"
#include "Interface/Menu/LevelStart.h"
#include "Interface/Level.h"

const char GAMEWINDOW_TITLE[] = "SnakeGame2";
const char BACKGROUND_SOUND_PATH[] = "data/music/level_bgm.ogg";
const char GAME_ICON_PATH[] = "data/image/icon.png";
static bool Mute = true;

GameWindow* new_GameWindow() {
    GameWindow* self = (GameWindow*)al_calloc(1,sizeof(GameWindow));
    GameWindow_init(self);
    return self;
}
void GameWindow_init(GameWindow* self) {
    // display
    self->display = nullptr;
    // Icon
    self->icon = nullptr;
    // sound
    self->background_sample = nullptr;
    self->background_music = nullptr;
    self->Mute = GameWindow_get_mute();
    // state
    self->state = GAMEWINDOW_RUNNING;
    // event
    self->event.type = -1;
    // Interface
    self->interface_num = 0;
    // load game window
    _GameWindow_load(self);
    // play background music
    al_play_sample_instance(self->background_music);
}
void GameWindow_destroy(GameWindow* self) {
    // destroy interface
    show_msg("Destroy Interface:");
    for(int i=0;i<self->interface_num;i++) {
        Interface* interface = self->interfaces[i];
        if(interface) interface->deleter(interface);
    }
    // destroy sound
    show_msg("Destroy Sound");
    if(self->background_music) {
        al_stop_sample_instance(self->background_music);
        al_destroy_sample_instance(self->background_music);
    }
    if(self->background_sample) al_destroy_sample(self->background_sample);
    // destroy display
    show_msg("Destroy Display");
    if (self->display) al_destroy_display(self->display);
    if (self->icon) al_destroy_bitmap(self->icon);
}
void delete_GameWindow(GameWindow* self) {
    GameWindow_destroy(self);
    al_free(self);
}
void GameWindow_draw(GameWindow* self) {
    // draw the top interface
    if (self->state == GAMEWINDOW_EXIT || self->interface_num == 0) return;
    Interface* top_interface = self->interfaces[self->interface_num-1];
    if (top_interface == nullptr) {raise_err("top interface is nullptr");return;}
    top_interface->draw(top_interface, al_get_backbuffer(self->display));
    // display the interface
    al_flip_display();
}
GAMEWINDOW_STATE GameWindow_update(GameWindow* self) {
    // exit if status is GAME_EXIT
    if (self->state==GAMEWINDOW_EXIT) return GAMEWINDOW_EXIT;
    // deal with the game window level key event
    _GameWindow_deal_event(self);
    // mute or unmute the background music
    self->Mute = GameWindow_get_mute();
    if (self->Mute) al_stop_sample_instance(self->background_music);
    else al_play_sample_instance(self->background_music);
    // update the top interface
    if (self->interface_num==0) return GAMEWINDOW_EXIT;
    Interface* top_interface = self->interfaces[self->interface_num-1];
    INTERFACE_INFO state_info = top_interface->update(top_interface);
    // update the interface stack
    switch (state_info.state) {
        case INTERFACE_STOP: { // add a new interface base on the infomation from top interface
            if (self->interface_num==INTERFACE_MAX_NUM) {raise_err("interface stack overflow");return GAMEWINDOW_EXIT;}
            if (state_info.child.interface_type == INTERFACE_NONE) {raise_err("interface stop but give none new interface, ignore");return GAMEWINDOW_RUNNING;}
            Interface* next_interface = _create_Interface(state_info.child);
            self->interfaces[self->interface_num++] = next_interface;
            break;
        }
        case INTERFACE_DIED: { // delete the top interface and return to the previous interface if next interface is INTERFACE_NONE
            self->interfaces[self->interface_num-1] = nullptr;
            if (state_info.child.interface_type != INTERFACE_NONE) {
                Interface* next_interface = _create_Interface(state_info.child);
                self->interfaces[self->interface_num-1] = next_interface;
            }
            else self->interface_num--;
            top_interface->deleter(top_interface);
            break;
        }
    }
    // return GAME_EXIT if there is no interface
    if (self->interface_num==0) return GAMEWINDOW_EXIT;
    return GAMEWINDOW_RUNNING;
}
void GameWindow_event_record(GameWindow* self, ALLEGRO_EVENT event) {
    self->event.type = -1;
    switch(event.type) {
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            show_msg("Detect display close");
            self->event = event;
            break;
        case ALLEGRO_EVENT_DISPLAY_RESIZE:
            show_msg("Detect display resize");
            self->event = event;
            break;
        default: {
            Interface* top_interface = self->interfaces[self->interface_num-1];
            top_interface->event_record(top_interface, event);
            break;
        }
    }
}
void GameWindow_set_mute(bool mute) {Mute = mute;}
bool GameWindow_get_mute() {return Mute;}
void GameWindow_toggle_mute() {Mute = !Mute;}

static void _GameWindow_load(GameWindow* self) {
    // Create Display
    show_msg("Create Display");
    al_set_new_display_refresh_rate(DISPLAY_FPS);
    self->display = al_create_display(INIT_DISPLAY_WIDTH,INIT_DISPLAY_HEIGHT);
    if(self->display==nullptr) raise_err("can't not create display window");
    self->icon = al_load_bitmap(GAME_ICON_PATH);
    al_set_window_position(self->display,0,0);
    al_set_display_icon(self->display, self->icon);
    al_set_window_title(self->display, GAMEWINDOW_TITLE);
    // Create Sound
    show_msg("Create background music");
    self->background_sample = al_load_sample(BACKGROUND_SOUND_PATH);
    if(self->background_sample==nullptr) raise_err("can't not load background music");
    self->background_music = al_create_sample_instance(self->background_sample);
    al_set_sample_instance_playmode(self->background_music, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(self->background_music, al_get_default_mixer());
    // Create interface
    show_msg("Create first interface:");
    CHILD_INFO first_interface_info = {FIRST_INTERFACE, 1};
    Interface* first_interface = _create_Interface(first_interface_info);
    self->interfaces[self->interface_num++] = first_interface;
}
static Interface* _create_Interface(CHILD_INFO info) {
    switch (info.interface_type) {
        case INTERFACE_IN_MENU:
            return (Interface*)new_InMenu();
            break;
        case INTERFACE_START_MENU:
            return (Interface*)new_StartMenu();
            break;
        case INTERFACE_GUIDE_MENU:
            return (Interface*)new_GuideMenu();
            break;
        case INTERFACE_LEVEL_MENU:
            return (Interface*)new_LevelMenu();
            break;
        case INTERFACE_LEVEL_START:
            return (Interface*)new_LevelStart(info.level);
            break;
        case INTERFACE_LEVEL:
            return (Interface*)new_Level(info.level);
            break;
        case INTERFACE_NONE:
            raise_warn("can't create INTERFACE_NONE");
            return nullptr;
            break;
        case INTERFACE_SINGLE:
            raise_warn("create inner interface: INTERFACE_SINGLE");
            return (Interface*)new_SingleMenu("");
            break;
        case INTERFACE_MULTI:
            raise_warn("create inner interface: INTERFACE_MULTI");
            return (Interface*)new_MultiMenu(nullptr,0);
            break;
        case INTERFACE_BASIC:
            raise_warn("create inner interface: INTERFACE_BASIC");
            return new_Interface();
        default:
            raise_err("unknown interface type");
            return nullptr;
    }
}
static void _GameWindow_deal_event(GameWindow* self) {
    if (self->event.type == -1) return;
    switch(self->event.type) {
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            self->state = GAMEWINDOW_EXIT;
            break;
        case ALLEGRO_EVENT_DISPLAY_RESIZE:
            al_acknowledge_resize(self->display);
            break;
        default:
            break;
    }
    self->event.type = -1;
}