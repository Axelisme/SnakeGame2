#include "global.h"
#include "Game.h"
#include "Entity/Object.h"
#include "Entity/Entity.h"

int main() {
    Allegro_init();
    al_set_new_display_refresh_rate(DISPLAY_FPS);
    ALLEGRO_DISPLAY *display = al_create_display(INIT_DISPLAY_WIDTH,INIT_DISPLAY_HEIGHT);
    al_set_window_position(display,0,0);
    ALLEGRO_BITMAP *buffer = al_get_backbuffer(display);
    al_set_target_bitmap(buffer);
    ShiftWindow sw;
    ShiftWindow_init(&sw,make_Pos(30,30),buffer);
    ShiftWindow_resize(&sw,make_Pos(9,9),make_Pos(21,21));
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));

    ObjectClass_init();
    Object obj1, obj2;
    Object_init(&obj1,make_Pos(9,9));
    obj1.Image = al_load_bitmap("data/image/apple.png");
    Object_init(&obj2,make_Pos(19,19));
    Entity entity;
    Entity_init(&entity);
    entity.addObject(&entity,&obj1);
    entity.addObject(&entity,&obj2);
    for (int i=0;i<10;i++) {
        al_clear_to_color(WRITE);
        entity.draw(&entity,&sw);
        al_flip_display();
        al_rest(0.5);
        entity.shift(&entity,DIRECTION_DOWN);
    }

    ALLEGRO_EVENT event;
    while(event.type != ALLEGRO_EVENT_DISPLAY_CLOSE)
        al_wait_for_event(event_queue, &event);

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    ShiftWindow_destroy(&sw);
    Entity_destroy(&entity);
    Object_destroy(&obj1);
    Object_destroy(&obj2);
    ObjectClass_destroy();
}