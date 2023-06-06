
#include "GameWindow.h"
#include "Interface/Interface.h"
#include "Interface/Menu/InMenu.h"
#include <string.h>

const char BACKGROUND_SOUND_PATH[] = "data/music/level_bgm.ogg";

GameWindow* new_GameWindow() {
    GameWindow* self = (GameWindow*)al_calloc(1,sizeof(GameWindow));
    GameWindow_init(self);
    return self;
}
void GameWindow_init(GameWindow* self) {
    // display
    self->display = nullptr;
    // sound
    self->background_sample = nullptr;
    self->background_music = nullptr;
    self->Mute = false;
    // state
    self->state = GAMEWINDOW_RUNING;
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
    show_msg("Destroy Interface");
    for(int i=0;i<self->interface_num;i++) {
        Interface* interface = self->interfaces[i];
        if(interface!=nullptr) interface->deleter(interface);
    }
    // destroy sound
    show_msg("Destroy Sound");
    if(self->background_music!=nullptr) {
        al_stop_sample_instance(self->background_music);
        al_destroy_sample_instance(self->background_music);
    }
    if(self->background_sample!=nullptr) al_destroy_sample(self->background_sample);
    // destroy display
    show_msg("Destroy Display");
    if(self->display!=nullptr) al_destroy_display(self->display);
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
    // deal with the game window level key event
    _GameWindow_deal_event(self);
    // exit if status is GAME_EXIT
    if (self->state==GAMEWINDOW_EXIT) return GAMEWINDOW_EXIT;
    // update the top interface
    if (self->interface_num==0) return GAMEWINDOW_EXIT;
    Interface* top_interface = self->interfaces[self->interface_num-1];
    INTERFACE_STATE state = top_interface->update(top_interface);
    // update the interface stack
    switch (state) {
        case INTERFACE_STOP: { // add a new interface base on the infomation from top interface
            if (self->interface_num==INTERFACE_MAX_NUM) {raise_err("interface stack overflow");return GAMEWINDOW_EXIT;}
            if (top_interface->next_interface == INTERFACE_NONE) {raise_err("interface stop but next interface is INTERFACE_NONE");return GAMEWINDOW_EXIT;}
            Interface* next_interface = _create_Interface(top_interface->next_interface);
            self->interfaces[self->interface_num++] = next_interface;
            break;
        }
        case INTERFACE_DIED: { // delete the top interface and return to the previous interface if next interface is INTERFACE_NONE
            self->interfaces[self->interface_num-1] = nullptr;
            if (top_interface->next_interface != INTERFACE_NONE) {
                Interface* next_interface = _create_Interface(top_interface->next_interface);
                self->interfaces[self->interface_num-1] = next_interface;
            }
            else self->interface_num--;
            top_interface->deleter(top_interface);
            break;
        }
    }
    // return GAME_EXIT if there is no interface
    if (self->interface_num==0) return GAMEWINDOW_EXIT;
    return GAMEWINDOW_RUNING;
}
void GameWindow_event_record(GameWindow* self, ALLEGRO_EVENT event) {
    self->event.type = -1;
    switch(event.type) {
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            self->event = event;
            break;
        case ALLEGRO_EVENT_KEY_DOWN: {
            Interface* top_interface = self->interfaces[self->interface_num-1];
            top_interface->event_record(top_interface, event);
            break;
        }
        default:
            break;
    }
}

void _GameWindow_load(GameWindow* self) {
    // Create Display
    show_msg("Create Display");
    al_set_new_display_refresh_rate(DISPLAY_FPS);
    self->display = al_create_display(INIT_DISPLAY_WIDTH,INIT_DISPLAY_HEIGHT);
    if(self->display==nullptr) raise_err("can't not create display window");
    al_set_window_position(self->display,0,0);
    // Create Sound
    show_msg("Create background music");
    self->background_sample = al_load_sample(BACKGROUND_SOUND_PATH);
    if(self->background_sample==nullptr) raise_err("can't not load background music");
    self->background_music = al_create_sample_instance(self->background_sample);
    al_set_sample_instance_playmode(self->background_music, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(self->background_music, al_get_default_mixer());
    // Create interface
    show_msg("Create In interface");
    Interface* first_interface = _create_Interface(FIRST_INTERFACE);
    self->interfaces[self->interface_num++] = first_interface;
}
Interface* _create_Interface(INTERFACE_TYPE type) {
    // TODO
    switch (type) {
        case INTERFACE_IN_MENU:
            return (Interface*)new_InMenu();
            break;
        case INTERFACE_NONE:
            raise_err("can't create INTERFACE_NONE");
        case INTERFACE_START_MENU:
        case INTERFACE_LEVEL_MENU:
        case INTERFACE_LEVEL:
        case INTERFACE_GUIDE:
        case INTERFACE_BASIC:
            return new_Interface();
        default:
            raise_err("unknown interface type");
            return nullptr;
    }
}
void _GameWindow_deal_event(GameWindow* self) {
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