// rewrite in c

#include "Menu.h"

Menu* new_Menu(GAME_STATE _game_state) {
    Menu* self = (Menu*)malloc(sizeof(Menu));
    Menu_init(self, _game_state);
    return self;
}
void Menu_init(Menu* self, GAME_STATE _game_state) {
    Interface_init((Interface*)self,"","","");
    self->game_state = _game_state;
    self->menu_state = IN_MENU;
    if (self->game_state == GAME_LEVEL)
    {
        self->menu_state = GUIDE;
        self->Guide = al_load_bitmap("data/image/menu/guide_menu.png");
    }
    else {
        self->In_menu = al_load_bitmap("data/image/menu/menu.png");
        self->Start_menu = al_load_bitmap("data/image/menu/game_menu.png");
        self->Start_menu_level = al_load_bitmap("data/image/menu/game_menu_level.png");
        self->Start_menu_music = al_load_bitmap("data/image/menu/game_menu_music.png");
        self->Start_menu_guide = al_load_bitmap("data/image/menu/game_menu_guide.png");
        self->Start_menu_exit = al_load_bitmap("data/image/menu/game_menu_exit.png");
        self->Level_menu = al_load_bitmap("data/image/menu/level_in.png");
        self->Level_menu_1 = al_load_bitmap("data/image/menu/level_in1.png");
        self->Level_menu_2 = al_load_bitmap("data/image/menu/level_in2.png");
        self->Level_menu_3 = al_load_bitmap("data/image/menu/level_in3.png");
        self->Level_menu_4 = al_load_bitmap("data/image/menu/level_in4.png");
        self->Level_menu_exit = al_load_bitmap("data/image/menu/level_exit.png");
        self->Guide = al_load_bitmap("data/image/menu/guide_menu.png");
        if (!(self->In_menu && self->Start_menu && self->Start_menu_level && self->Start_menu_music && self->Start_menu_guide &&
            self->Start_menu_exit && self->Level_menu && self->Level_menu_1 && self->Level_menu_2 && self->Level_menu_3 &&
            self->Level_menu_4 && self->Level_menu_exit && self->Guide))
        {
            raise_warn("menu image missing");
        }
    }
    self->infer.draw = Menu_draw;
}
void Menu_draw(Interface* Iself) {
    Menu* self = (Menu*)Iself;
    al_clear_to_color(al_map_rgb(Iself->light,Iself->light,Iself->light));
    ALLEGRO_BITMAP * img = Menu_ToImg(self, self->menu_state);
    al_draw_scaled_bitmap(img,
                          0,0,
                          al_get_bitmap_width(img),
                          al_get_bitmap_height(img),
                          0,0,
                          DISPLAY_WIDTH,DISPLAY_HEIGHT,
                          0);
    al_flip_display();
}
bool Menu_update(Interface* Iself) {
    Menu* self = (Menu*)Iself;
    switch(self->game_state) {
        case GAME_MENU: {
            break;
        }
        case GAME_LEVEL_PUASE: {
            self->menu_state = GUIDE;
            break;
        }
        default: {
            raise_warn("Unknown game state");
            self->menu_state = IN_MENU;
        }
    }
    return true;
}
GAME_STATE Menu_key_triger(Menu* self, int key) {
    GAME_STATE game_state = self->game_state;
    MENU_STATE menu_state = self->menu_state;
    if (menu_state == IN_MENU)
    {
        Interface_go_down((Interface*)self);
        self->menu_state = START_MENU;
        Interface_go_lift((Interface*)self);
        return GAME_MENU;
    }
    else if (game_state == GAME_LEVEL_PUASE)
    {
        if (key == ALLEGRO_KEY_ENTER || key == ALLEGRO_KEY_P)
        {
            Interface_go_down((Interface*)self);
            self->menu_state = START_MENU;
            Interface_go_lift((Interface*)self);
            return GAME_LEVEL;
        }
        else {return GAME_LEVEL_PUASE;}
    }
    else {
        if(key == ALLEGRO_KEY_DOWN && (menu_state == START_MENU_EXIT || menu_state == LEVEL_MENU_EXIT || menu_state == GUIDE)) {
            show_msg("Key triger : at the bottom of menu");
            return GAME_MENU;
        }
        if(key == ALLEGRO_KEY_DOWN && (menu_state != START_MENU_EXIT && menu_state != LEVEL_MENU_EXIT && menu_state != GUIDE)) {
            show_msg("Key triger : menu select go down");
            self->menu_state = (MENU_STATE)((int)menu_state+1);
            return GAME_MENU;
        }
        if(key == ALLEGRO_KEY_UP && (menu_state == START_MENU || menu_state == LEVEL_MENU || menu_state == GUIDE)) {
            show_msg("Key triger : at the top of menu");
            return GAME_MENU;
        }
        if(key == ALLEGRO_KEY_UP && (menu_state != START_MENU && menu_state != LEVEL_MENU && menu_state != GUIDE)) {
            show_msg("Key triger : menu select go up");
            self->menu_state = (MENU_STATE)((int)menu_state-1);
            return GAME_MENU;
        }
        if(key == ALLEGRO_KEY_ENTER) {
            switch (menu_state)
            {
            case START_MENU_LEVEL:
                Interface_go_down((Interface*)self);
                self->menu_state = LEVEL_MENU;
                Interface_go_lift((Interface*)self);
                return GAME_MENU;
                break;
            case START_MENU_MUSIC:
                GameWindow_Mute = !GameWindow_Mute;
                return GAME_MENU;
                break;
            case START_MENU_GUID:
                Interface_go_down((Interface*)self);
                self->menu_state = GUIDE;
                Interface_go_lift((Interface*)self);
                return GAME_MENU;
                break;
            case START_MENU_EXIT:
                Interface_go_down((Interface*)self);
                return GAME_TERMINATE;
                break;
            case LEVEL_MENU_1:
                GameWindow_want_level = 1;
                return GAME_LEVEL;
                break;
            case LEVEL_MENU_2:
                GameWindow_want_level = 2;
                return GAME_LEVEL;
                break;
            case LEVEL_MENU_3:
                GameWindow_want_level = 3;
                return GAME_LEVEL;
                break;
            case LEVEL_MENU_4:
                GameWindow_want_level = 4;
                return GAME_LEVEL;
                break;
            case LEVEL_MENU_EXIT:
                Interface_go_down((Interface*)self);
                self->menu_state = START_MENU;
                Interface_go_lift((Interface*)self);
                return GAME_MENU;
                break;
            case GUIDE:
                self->menu_state = START_MENU;
                return GAME_MENU;
                break;

            default:
                return GAME_MENU;
                break;
            }
            // show_msg("Key triger : quit game");
            // return GAME_TERMINATE;
        }
        else {
            return GAME_MENU;
        }
    }
    return GAME_MENU;
}
GAME_STATE Menu_mouse_triger(Menu* self, int mouse) {
    return GAME_MENU;
}
ALLEGRO_BITMAP * Menu_ToImg(Menu* self,MENU_STATE stat) {
    switch(stat) {
        case IN_MENU:
            return self->In_menu;
        case START_MENU:
            return self->Start_menu;
        case START_MENU_LEVEL:
            return self->Start_menu_level;
        case START_MENU_MUSIC:
            return self->Start_menu_music;
        case START_MENU_GUID:
            return self->Start_menu_guide;
        case START_MENU_EXIT:
            return self->Start_menu_exit;
        case LEVEL_MENU:
            return self->Level_menu;
        case LEVEL_MENU_1:
            return self->Level_menu_1;
        case LEVEL_MENU_2:
            return self->Level_menu_2;
        case LEVEL_MENU_3:
            return self->Level_menu_3;
        case LEVEL_MENU_4:
            return self->Level_menu_4;
        case LEVEL_MENU_EXIT:
            return self->Level_menu_exit;
        case GUIDE:
            return self->Guide;
        default:
            raise_warn("Unknown menu draw issue");
            return self->In_menu;
    }
}
void Menu_destroy(Menu* self) {
    Interface_destroy((Interface*)self);
    al_destroy_bitmap(self->In_menu);
    al_destroy_bitmap(self->Start_menu);
    al_destroy_bitmap(self->Start_menu_level);
    al_destroy_bitmap(self->Start_menu_music);
    al_destroy_bitmap(self->Start_menu_guide);
    al_destroy_bitmap(self->Start_menu_exit);
    al_destroy_bitmap(self->Level_menu);
    al_destroy_bitmap(self->Level_menu_1);
    al_destroy_bitmap(self->Level_menu_2);
    al_destroy_bitmap(self->Level_menu_3);
    al_destroy_bitmap(self->Level_menu_4);
    al_destroy_bitmap(self->Level_menu_exit);
    al_destroy_bitmap(self->Guide);
}
void delete_Menu(Interface* Iself) {
    Menu* self = (Menu*)Iself;
    Menu_destroy(self);
    free(self);
}

/*
#include "Menu.h"
#include "GameWindow.h"

ALLEGRO_BITMAP * Menu::ToImg(MENU_STATE stat) {
    switch(stat) {
        case IN_MENU:
            return In_menu;
        case START_MENU:
            return Start_menu;
        case START_MENU_LEVEL:
            return Start_menu_level;
        case START_MENU_MUSIC:
            return Start_menu_music;
        case START_MENU_GUID:
            return Start_menu_guide;
        case START_MENU_EXIT:
            return Start_menu_exit;
        case LEVEL_MENU:
            return Level_menu;
        case LEVEL_MENU_1:
            return Level_menu_1;
        case LEVEL_MENU_2:
            return Level_menu_2;
        case LEVEL_MENU_3:
            return Level_menu_3;
        case LEVEL_MENU_4:
            return Level_menu_4;
        case LEVEL_MENU_EXIT:
            return Level_menu_exit;
        case GUIDE:
            return Guide;
        
        default:
            raise_warn("Unknown menu draw issue");
            return In_menu;
    }
}

void Menu::draw() {
    al_clear_to_color(al_map_rgb(light,light,light));
    ALLEGRO_BITMAP * img = ToImg(menu_state);
    al_draw_scaled_bitmap(img,
                          0,0,
                          al_get_bitmap_width(img),
                          al_get_bitmap_height(img),
                          0,0,
                          DISPLAY_WIDTH,DISPLAY_HEIGHT,
                          0);
    al_flip_display();
}

GAME_STATE Menu::key_triger(int key) {
    if (menu_state == IN_MENU)
    {
        go_down();
        menu_state = START_MENU;
        go_lift();
        return GAME_MENU;
    }
    else if (game_state == GAME_LEVEL_PUASE)
    {
        if (key == ALLEGRO_KEY_ENTER || key == ALLEGRO_KEY_P)
        {
            go_down();
            menu_state = START_MENU;
            go_lift();
            return GAME_LEVEL;
        }
        else {return GAME_LEVEL_PUASE;}
    }
    else {
        if(key == ALLEGRO_KEY_DOWN && (menu_state == START_MENU_EXIT || menu_state == LEVEL_MENU_EXIT || menu_state == GUIDE)) {
            show_msg("Key triger : at the bottom of menu");
            return GAME_MENU;
        }
        if(key == ALLEGRO_KEY_DOWN && (menu_state != START_MENU_EXIT && menu_state != LEVEL_MENU_EXIT && menu_state != GUIDE)) {
            show_msg("Key triger : menu select go down");
            menu_state = MENU_STATE(((int)menu_state)+1);
            return GAME_MENU;
        } 
        if(key == ALLEGRO_KEY_UP && (menu_state == START_MENU || menu_state == LEVEL_MENU || menu_state == GUIDE)) {
            show_msg("Key triger : at the top of menu");
            return GAME_MENU;
        } 
        if(key == ALLEGRO_KEY_UP && (menu_state != START_MENU && menu_state != LEVEL_MENU && menu_state != GUIDE)) {
            show_msg("Key triger : menu select go up");
            menu_state = MENU_STATE(((int)menu_state)-1);
            return GAME_MENU;
        } 
        if(key == ALLEGRO_KEY_ENTER) {
            switch (menu_state)
            {
            case START_MENU_LEVEL:
                go_down();
                menu_state = LEVEL_MENU;
                go_lift();
                return GAME_MENU;
                break;
            case START_MENU_MUSIC:
                GameWindow::Mute = !GameWindow::Mute;
                return GAME_MENU;
                break;
            case START_MENU_GUID:
                go_down();
                menu_state = GUIDE;
                go_lift();
                return GAME_MENU;
                break;
            case START_MENU_EXIT:
                go_down();
                return GAME_TERMINATE;
                break;
            case LEVEL_MENU_1:
                GameWindow::want_level = 1;
                return GAME_LEVEL;
                break;
            case LEVEL_MENU_2:
                GameWindow::want_level = 2;
                return GAME_LEVEL;
                break;
            case LEVEL_MENU_3:
                GameWindow::want_level = 3;
                return GAME_LEVEL;
                break;
            case LEVEL_MENU_4:
                GameWindow::want_level = 4;
                return GAME_LEVEL;
                break;
            case LEVEL_MENU_EXIT:
                go_down();
                menu_state = START_MENU;
                go_lift();
                return GAME_MENU;
                break;
            case GUIDE:
                menu_state = START_MENU;
                return GAME_MENU;
                break;

            default:
                return GAME_MENU;
                break;
            }
            // show_msg("Key triger : quit game");
            // return GAME_TERMINATE;
        }
        else {
            return GAME_MENU;
        }
    }
    return GAME_MENU;
}

bool Menu::update() {
    switch(game_state) {
        case GAME_MENU: {
            break;
        }
        case GAME_LEVEL_PUASE: {
            menu_state = GUIDE;
            break;
        }
        default: {
            raise_warn("Unknown game state");
            menu_state = IN_MENU;
        }
    }
    return true;
}

Menu::Menu(GAME_STATE _game_state):Interface("","","") {
    game_state = _game_state;
    if (game_state == GAME_LEVEL)
    {
        menu_state = GUIDE;
        Guide = al_load_bitmap((MENUE_PATH+"/guide_menu.png").c_str());
    }
    else {
        In_menu = al_load_bitmap((MENUE_PATH+"/menu.png").c_str());
        Start_menu = al_load_bitmap((MENUE_PATH+"/game_menu.png").c_str());
        Start_menu_level = al_load_bitmap((MENUE_PATH+"/game_menu_level.png").c_str());
        Start_menu_music = al_load_bitmap((MENUE_PATH+"/game_menu_music.png").c_str());
        Start_menu_guide = al_load_bitmap((MENUE_PATH+"/game_menu_guide.png").c_str());
        Start_menu_exit = al_load_bitmap((MENUE_PATH+"/game_menu_exit.png").c_str());
        Level_menu = al_load_bitmap((MENUE_PATH+"/level_in.png").c_str());
        Level_menu_1 = al_load_bitmap((MENUE_PATH+"/level_in1.png").c_str());
        Level_menu_2 = al_load_bitmap((MENUE_PATH+"/level_in2.png").c_str());
        Level_menu_3 = al_load_bitmap((MENUE_PATH+"/level_in3.png").c_str());
        Level_menu_4 = al_load_bitmap((MENUE_PATH+"/level_in4.png").c_str());
        Level_menu_exit = al_load_bitmap((MENUE_PATH+"/level_exit.png").c_str());
        Guide = al_load_bitmap((MENUE_PATH+"/guide_menu.png").c_str());
        if (!(In_menu && Start_menu && Start_menu_level && Start_menu_music && Start_menu_guide &&
            Start_menu_exit && Level_menu && Level_menu_1 && Level_menu_2 && Level_menu_3 &&
            Level_menu_4 && Level_menu_exit && Guide))
        {
            raise_warn("menu image missing");
        }
    }
}
*/
