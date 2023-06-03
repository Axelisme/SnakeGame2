// rewrite in c

#include "GameWindow.h"
#include "global.h"

GameWindow* new_GameWindow() {
    GameWindow* self = (GameWindow*)malloc(sizeof(GameWindow));
    GameWindow_init(self);
    return self;
}
void GameWindow_init(GameWindow* self) {
    self->width = DISPLAY_WIDTH;
    self->height = DISPLAY_HEIGHT;
    self->state = GAME_MENU;
    self->menu = nullptr;
    self->level = nullptr;
    GameWindow_Mute = false;
    GameWindow_want_level = FIRST_LEVEL;
    // Initial Allegro
    show_msg("Initial Allegor");
    if (! al_init()) raise_err("allegro initial fail");

    // Initial Allegro addon
    show_msg("Initial Allegro addon");
    al_init_primitives_addon();
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon(); // initialize the ttf (True Type Font) addon
    al_init_image_addon(); // initialize the image addon
    al_init_acodec_addon(); // initialize acodec addon

    // install device
    show_msg("Install device");
    al_install_keyboard(); // install keyboard event
    al_install_mouse();    // install mouse event
    al_install_audio();    // install audio event

    // first state is game menu
    self->state = FIRST_STATE;

    // Create Display
    show_msg("Create Display");
    self->display = al_create_display(self->width,self->height);
    if(self->display==nullptr) raise_err("can't not create display window");
    al_set_window_position(self->display,0,0);

    // Create event queue
    show_msg("Create even queue");
    self->event_queue = al_create_event_queue();
    if(self->event_queue==nullptr) raise_err("can't not create event_queue");

    // Create timer
    show_msg("Create timer");
    self->timer = al_create_timer(1.0 / DISPLAY_FPS);
    if(self->timer==nullptr) raise_err("can't not create timer");

    // Create sound
    show_msg("Create sound");
    if(!al_reserve_samples(3)) raise_warn("can't not initial sound");

    // register event source
    show_msg("register event source");
    al_register_event_source(self->event_queue, al_get_display_event_source(self->display));
    al_register_event_source(self->event_queue, al_get_keyboard_event_source());
    al_register_event_source(self->event_queue, al_get_mouse_event_source());
    al_register_event_source(self->event_queue, al_get_timer_event_source(self->timer));
}
void GameWindow_draw(GameWindow* self) {
    if(GameWindow_Mute) Interface_stop_sound((Interface*)self->level);
    else Interface_start_sound((Interface*)self->level);
    switch(self->state) {
        case GAME_MENU:
        case GAME_LEVEL_PUASE: {
            self->menu->infer.draw((Interface*)self->menu);
            break;
        }
        case GAME_LEVEL: {
            self->level->infer.draw((Interface*)self->level);
            break;
        }
        case GAME_TERMINATE: return;
    }
    al_flip_display();
}
bool GameWindow_update(GameWindow* self) {
    // menu or level update
    if(GameWindow_want_level != self->level->level_idx) {
        //delete level;
        delete_Level((Interface*)self->level);
        if(GameWindow_want_level>LEVEL_NUM){
            self->state = GAME_MENU;
            GameWindow_want_level = FIRST_LEVEL;
        }
        else self->level = new_Level(GameWindow_want_level);
    }
    switch(self->state) {
        case GAME_MENU: {
            self->menu->game_state = self->state;
            Menu_update((Interface*)self->menu);
            return true;
        }
        case GAME_LEVEL: {
            switch(self->level->level_stat)
            {
                case KEEP:{
                    Level_update((Interface*)self->level);
                    break;
                }
                case STOP: {
                    self->state = GAME_MENU;
                    break;
                }
                case NEXT:{
                    int level_idx = self->level->level_idx;
                    if(level_idx == LEVEL_NUM) {
                        GameWindow_want_level = 1;
                        self->state = GAME_MENU;
                        return false;
                    }
                    else{
                        //delete level;
                        delete_Level((Interface*)self->level);
                        GameWindow_want_level = level_idx+1;
                        self->level = new_Level(level_idx+1);
                        return true;
                    }
                }
                case RESTART:{
                    int level_idx = self->level->level_idx;
                    //delete level;
                    delete_Level((Interface*)self->level);
                    self->level = new_Level(level_idx);
                    return true;
                }
            }
            return true;
        }
        case GAME_LEVEL_PUASE: {
            self->menu->game_state = self->state;
            //self->menu->update();
            Menu_update((Interface*)self->menu);
        }
        case GAME_TERMINATE: return false;
        default : {
            raise_err("unknown game state");
            self->state = GAME_TERMINATE;
            return false;
        }
    }
}
void GameWindow_game_load(GameWindow* self) {
    show_msg("Game Loading...");

    // create level
    show_msg("Create level");
    self->level = new_Level(FIRST_LEVEL);

    // create menu
    show_msg("Create menu");
    self->menu = new_Menu(self->state);

    show_msg("Game Load finish");
}
void GameWindow_game_play(GameWindow* self) {
    // load data
    GameWindow_game_load(self);

    // start game
    GameWindow_game_begin(self);

    // process event until terminate
    show_msg("Start to process event");
    pos_t t_now = 0.0;
    pos_t t_pre = 0.0;
    while(self->state != GAME_TERMINATE) {
        if(!al_is_event_queue_empty(self->event_queue))
            GameWindow_game_process(self);
        else {
            t_now=al_get_time();
            if(t_now-t_pre>=10/DISPLAY_FPS) {
                t_pre = t_now;
            }
            else sleep(0);
        }
    }
}
void GameWindow_game_reset(GameWindow* self) {
    show_msg("Game reset begin");

    //self->level->level_reset(FIRST_LEVEL);
    Level_level_reset(self->level, FIRST_LEVEL);
    self->state = FIRST_STATE;

    show_msg("Game reset down");
}
void GameWindow_game_destroy(GameWindow* self) {
    show_msg("Game destroy begin");

    if (self->display) al_destroy_display(self->display);               // delete display
    if (self->event_queue) al_destroy_event_queue(self->event_queue);   // delete event_queue
    if (self->timer) al_destroy_timer(self->timer);                     // delete timer

    //delete level;
    delete_Level((Interface*)self->level);
    //delete menu;
    delete_Menu((Interface*)self->menu);

    show_msg("Game destroy done");
}
void GameWindow_game_begin(GameWindow* self) {
    show_msg("Game begining");

    // set timer start
    show_msg("set timer start");
    al_start_timer(self->timer);

    show_msg("Game begin finish");
}
void GameWindow_game_process(GameWindow* self) {
    // acquire event
    al_wait_for_event(self->event_queue, &self->event);

    // process for different event
    switch(self->event.type) {
        case ALLEGRO_EVENT_TIMER: {                  // meet time update
            //update();
            GameWindow_update(self);
            //draw();
            GameWindow_draw(self);
            break;
        }
        case ALLEGRO_EVENT_DISPLAY_CLOSE: {          // if close the window
            show_msg("Detect display close, game terminate");
            self->state = GAME_TERMINATE;
            return;
        }
        case ALLEGRO_EVENT_KEY_DOWN: {               // if pulse a key
            if(self->level->infer.key_lock || self->menu->infer.key_lock) break;
            show_msg("Detect key down");
            // process for different state
            if(self->state==GAME_LEVEL) {
                //self->state = self->level->key_triger(event.keyboard.keycode);
                self->state = Level_key_triger(self->level, self->event.keyboard.keycode);
            }
            else {
                self->menu->game_state = self->state;
                //self->state = self->menu->key_triger(self->event.keyboard.keycode);
                self->state = Menu_key_triger(self->menu, self->event.keyboard.keycode);
            }
            break;
        }
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: {      // if click
            show_msg("Detect click down");
            // process for different state
            if(self->state==GAME_MENU)
                    //self->state = self->menu->mouse_triger(self->event.mouse.button);
                    self->state = Menu_mouse_triger(self->menu, self->event.mouse.button);
            break;
        }
    }
}
GameWindow* delete_GameWindow(GameWindow* self) {
    GameWindow_game_destroy(self);
    free(self);
}

/*
#include "GameWindow.h"
#include <iostream>
#include <unistd.h>
#include <sched.h>

using namespace std;

int GameWindow::want_level = FIRST_LEVEL;

void GameWindow::game_load() {
    show_msg("Game Loading...");

    // create level
    show_msg("Create level");
    level = new Level(FIRST_LEVEL);

    // create menu
    show_msg("Create menu");
    menu = new Menu(state);

    show_msg("Game Load finish");
}

void GameWindow::game_play() {
    // load data
    game_load();

    // start game
    game_begin();

    // process event until terminate
    show_msg("Start to process event");
    double t_now=0.0;
    double t_pre=0.0;
    while(state != GAME_TERMINATE) {
        if(!al_is_event_queue_empty(event_queue))
            game_process();
        else {
            t_now=al_get_time();
            if(t_now-t_pre>=10/FPS) {
                t_pre = t_now;
            }
            else sleep(0);
        }
    }

}

void GameWindow::game_begin() {
    show_msg("Game begining");

    // set timer start
    show_msg("set timer start");
    al_start_timer(timer);

    show_msg("Game begin finish");
}

void GameWindow::game_process() {
    // acquire event
    al_wait_for_event(event_queue, &event);

    // process for different event
    switch(event.type) {
        case ALLEGRO_EVENT_TIMER: {                  // meet time update
            update();
            draw();
            break;
        }
        case ALLEGRO_EVENT_DISPLAY_CLOSE: {          // if close the window
            show_msg("Detect display close, game terminate");
            state = GAME_TERMINATE;
            return;
        }
        case ALLEGRO_EVENT_KEY_DOWN: {               // if pulse a key
            if(level->key_lock || menu->key_lock) break;
            show_msg("Detect key down");
            // process for different state
            if(state==GAME_LEVEL)
                state = level->key_triger(event.keyboard.keycode);
            else {
                menu->game_state = state;
                state = menu->key_triger(event.keyboard.keycode);
            }
            break;
        }
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: {      // if click
            show_msg("Detect click down");
            // process for different state
            if(state==GAME_MENU)
                    state = menu->mouse_triger(event.mouse.button);
            break;
        }
    }
}

bool GameWindow::update() {
    // menu or level update
    if(GameWindow::want_level!=level->getID()) {
        delete level;
        if(want_level>LEVEL_NUM){
            state = GAME_MENU;
            want_level = FIRST_LEVEL;
        }
        else level = new Level(want_level);
    }
    switch(state) {
        case GAME_MENU: {
            menu->game_state = state;
            menu->update();
            return true;
        }
        case GAME_LEVEL: {
            switch(level->level_stat)
            {
                case KEEP:{
                    level->update();
                    break;
                }
                case STOP: {
                    state = GAME_MENU;
                    break;
                }
                case NEXT:{
                    int level_idx = level->getID();
                    if(level_idx == LEVEL_NUM) {
                        GameWindow::want_level = 1;
                        state = GAME_MENU;
                        return false;
                    }
                    else{
                        delete level;
                        GameWindow::want_level = level_idx+1;
                        level = new Level(level_idx+1);
                        return true;
                    }
                }
                case RESTART:{
                    int level_idx = level->getID();
                    delete level;
                    level = new Level(level_idx);
                    return true;
                }
            }
            return true;
        }
        case GAME_LEVEL_PUASE: {
            menu->game_state = state;
            menu->update();
        }
        case GAME_TERMINATE: return false;
        default : {
            raise_err("unknown game state");
            state = GAME_TERMINATE;
            return false;
        }
    }
}

void GameWindow::draw() {
    if(GameWindow::Mute) level->stop_sound();
    else level->start_sound();
    switch(state) {
        case GAME_MENU:
        case GAME_LEVEL_PUASE: {
            menu->draw();
            break;
        }
        case GAME_LEVEL: {
            level->draw();
            break;
        }
        case GAME_TERMINATE: return;
    }
    al_flip_display();
}

GameWindow::GameWindow() {
    // Initial Allegro
    show_msg("Initial Allegor");
    if (! al_init()) raise_err("allegro initial fail");

    // Initial Allegro addon
    show_msg("Initial Allegro addon");
    al_init_primitives_addon();
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon(); // initialize the ttf (True Type Font) addon
    al_init_image_addon(); // initialize the image addon
    al_init_acodec_addon(); // initialize acodec addon

    // install device
    show_msg("Install device");
    al_install_keyboard(); // install keyboard event
    al_install_mouse();    // install mouse event
    al_install_audio();    // install audio event

    // first state is game menu
    state = FIRST_STATE;

    // Create Display
    show_msg("Create Display");
    display = al_create_display(width,height);
    if(display==nullptr) raise_err("can't not create display window");
    al_set_window_position(display,0,0);

    // Create event queue
    show_msg("Create even queue");
    event_queue = al_create_event_queue();
    if(event_queue==nullptr) raise_err("can't not create event_queue");

    // Create timer
    show_msg("Create timer");
    timer = al_create_timer(1.0 / FPS);
    if(timer==nullptr) raise_err("can't not create timer");

    // Create sound
    show_msg("Create sound");
    if(!al_reserve_samples(3)) raise_warn("can't not initial sound");

    // register event source
    show_msg("register event source");
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
}

void GameWindow::game_reset() {
    show_msg("Game reset begin");

    level->level_reset(FIRST_LEVEL);
    state = FIRST_STATE;

    show_msg("Game reset down");
}

void GameWindow::game_destroy() {
    show_msg("Game destroy begin");

    al_destroy_display(display);            // delete display
    al_destroy_event_queue(event_queue);    // delete event_queue
    al_destroy_timer(timer);                // delete timer

    delete level;
    delete menu;

    show_msg("Game destroy done");
}

GameWindow::~GameWindow() {
    show_msg("Game delete begin");

    game_destroy();

    show_msg("Game delete done");
}
*/
