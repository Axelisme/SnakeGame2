
#include "GameWindow.h"
#include "SoundEngine.h"
#include "Object/Object.h"
#include "Interface/Interface.h"
#include "Interface/Menu/InMenu.h"
#include "Interface/Menu/StartMenu.h"
#include "Interface/Menu/GuideMenu.h"
#include "Interface/Menu/LevelMenu.h"
#include "Interface/Menu/LevelStart.h"
#include "Interface/Menu/LevelWin.h"
#include "Interface/LevelView.h"

const char GAMEWINDOW_TITLE[] = "SnakeGame2";
const char BACKGROUND_SOUND_PATH[] = "data/music/level_bgm.ogg";
const char GAME_ICON_PATH[] = "data/image/icon.png";
static bool Mute = INIT_MUTE;

GameWindow* new_GameWindow() {
    GameWindow* self = (GameWindow*)al_calloc(1,sizeof(GameWindow));
    GameWindow_init(self);
    return self;
}
void GameWindow_init(GameWindow* self) {
    // state
    self->state = GAMEWINDOW_RUNNING;
    // event
    self->event.type = -1;
    // Create Display
    show_msg("Create Display");
    al_set_new_display_refresh_rate(DISPLAY_FPS);
    self->display = al_create_display(INIT_DISPLAY_WIDTH,INIT_DISPLAY_HEIGHT);
    if (!self->display) raise_err("can't not create display window");
    else {
        al_set_window_position(self->display,0,0);
        al_set_window_title(self->display, GAMEWINDOW_TITLE);
    }
    self->icon = al_load_bitmap(GAME_ICON_PATH);
    if (!self->icon) raise_warn("can't not load icon");
    else al_set_display_icon(self->display, self->icon);
    // initial ObjectClass and InterfaceClass
    ObjectClass_init();
    InterfaceClass_init();
    // initial sample instances
    SE_init();
    // Load background Sound
    show_msg("Load background music");
    self->background_sample = al_load_sample(BACKGROUND_SOUND_PATH);
    if(!self->background_sample) raise_warn("can't not load background music");
    else SE_add_sound(self->background_sample, ALLEGRO_PLAYMODE_LOOP);
    // Interface
    show_msg("Create first interface:");
    self->interface_num = 1;
    CHILD_INFO first_interface_info = {FIRST_INTERFACE, 1};
    Interface* first_interface = _create_Interface(first_interface_info);
    self->interfaces[self->interface_num-1] = first_interface;
}
void GameWindow_destroy(GameWindow* self) {
    // destroy interface
    show_msg("Destroy Interface:");
    for(int i=0;i<self->interface_num;i++) {
        Interface* interface = self->interfaces[i];
        if(interface) interface->deleter(interface);
    }
    // clear sample instances
    show_msg("Clear sample instances");
    SE_destroy();
    // destroy background sound
    show_msg("Destroy background Sound");
    if(self->background_sample) al_destroy_sample(self->background_sample);
    // destroy ObjectClass and InterfaceClass
    InterfaceClass_destroy();
    ObjectClass_destroy();
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
    // update sound engine
    SE_update();
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
            top_interface->deleter(top_interface);
            self->interfaces[self->interface_num-1] = nullptr;
            if (state_info.child.interface_type != INTERFACE_NONE) {
                Interface* next_interface = _create_Interface(state_info.child);
                self->interfaces[self->interface_num-1] = next_interface;
            }
            else self->interface_num--;
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
        case INTERFACE_LEVEL_WIN:
            return (Interface*)new_LevelWin(info.level);
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