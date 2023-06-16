
#include "Game.h"

Game* new_Game() {
    Game* self = (Game*)calloc(1, sizeof(Game));
    Game_init(self);
    return self;
}
void Allegro_init() {
    // Initial Allegro
    show_msg("Initial Allegro");
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

    // Initial sound
    show_msg("Initial sound");
    if(!al_reserve_samples(3))
        raise_warn("can't not initial sound");
}
void Game_init(Game* self) {
    // Info
    strncpy(self->name, "Game", NAME_MAX_LENGTH);
    // allegro
    Allegro_init();
    // game body
    GameWindow_init(&self->window);
    // event queue
    show_msg("Create even queue");
    self->event_queue = al_create_event_queue();
    if(self->event_queue==nullptr) raise_err("can't not create event_queue");
    // timer
    show_msg("Create timer");
    self->update_timer = al_create_timer(1.0 / UPDATE_FPS);
    if(self->update_timer==nullptr) raise_err("can't not create update timer");
    self->render_timer = al_create_timer(1.0 / RENDER_FPS);
    if(self->render_timer==nullptr) raise_err("can't not create render timer");
    // register event source
    show_msg("register event source");
    al_register_event_source(self->event_queue, al_get_display_event_source(self->window.display));
    al_register_event_source(self->event_queue, al_get_keyboard_event_source());
    al_register_event_source(self->event_queue, al_get_mouse_event_source());
    al_register_event_source(self->event_queue, al_get_timer_event_source(self->update_timer));
    al_register_event_source(self->event_queue, al_get_timer_event_source(self->render_timer));
}
void Game_destroy(Game* self) {
    if (self == nullptr) return;
    GameWindow_destroy(&self->window);
    if (self->event_queue) al_destroy_event_queue(self->event_queue);
    if (self->event_queue) al_destroy_timer(self->update_timer);
    if (self->event_queue) al_destroy_timer(self->render_timer);
}
void delete_Game(Game* self) {
    Game_destroy(self);
    free(self);
}
void Game_run(Game* self) {
    // start timer
    al_start_timer(self->update_timer);
    al_start_timer(self->render_timer);
    // game loop
    show_msg("Game loop start");
    bool running_flag = true;
    while (running_flag) {
        // get event
        ALLEGRO_EVENT event;
        ALLEGRO_EVENT next_event;
        al_wait_for_event(self->event_queue, &event);
        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:
                al_peek_next_event(self->event_queue, &next_event);
                if (next_event.timer.source == event.timer.source)
                    al_flush_event_queue(self->event_queue);
                if (event.timer.source == self->update_timer){       // update
                    if (GAMEWINDOW_EXIT == GameWindow_update(&self->window))
                        running_flag = false;
                }
                else if (event.timer.source == self->render_timer)  // draw
                    GameWindow_draw(&self->window);
                break;
            default:
                GameWindow_event_record(&self->window, event);
        }
    }
    show_msg("Game loop end");
    // stop timer
    al_stop_timer(self->update_timer);
    al_stop_timer(self->render_timer);
}