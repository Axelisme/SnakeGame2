// rewrite in c

#include "Level.h"
#include <stdio.h>
#include <wchar.h>

Level* new_Level(int level_idx) {
    Level* level = (Level*)malloc(sizeof(Level));
    Level_init(level, level_idx);
    return level;
}
void Level_init(Level *self, int level_idx) {
    Interface_init((Interface*)self, "data/music/level_bgm.ogg", "data/image/background.png", "data/image/fog.png");
    show_msg("Create level begin");
    self->level_idx = level_idx;
    self->Gravity = DOWN;
    self->level_stat = KEEP;
    self->Fix_Background_image = nullptr;
    self->Ground_image = nullptr;
    self->Stone_image = nullptr;
    self->Apple_image = nullptr;
    self->Snake_head_image = nullptr;
    self->Snake_body_straight_image = nullptr;
    self->Snake_body_turn_image = nullptr;
    self->Snake_tail_image = nullptr;
    self->End_point_image = nullptr;
    self->Buttom_image = nullptr;
    self->Spike_image = nullptr;
    self->Spike_down_image = nullptr;
    Map_init(&self->map,0,0,AIR);
    Map_init(&self->ground_map,0,0,AIR);
    Map_init(&self->ob_map,0,0,AIR);
    Map_init(&self->snake_map,0,0,AIR);
    ObjectQueue_init(&self->object);
    self->infer.draw = Level_draw;
    Level_load_level(self,level_idx);
    show_msg("Create level done");
}
void Level_destory(Interface *Iself) {
    show_msg("level destroy begin");
    if (Iself == nullptr) return;
    Interface_destroy(Iself);
    Level *self = (Level*)Iself;
    al_destroy_bitmap(self->Ground_image);
    al_destroy_bitmap(self->Stone_image);
    al_destroy_bitmap(self->Apple_image);
    al_destroy_bitmap(self->Snake_head_image);
    al_destroy_bitmap(self->Snake_body_straight_image);
    al_destroy_bitmap(self->Snake_body_turn_image);
    al_destroy_bitmap(self->End_point_image);
    al_destroy_bitmap(self->Buttom_image);
    al_destroy_bitmap(self->Spike_image);
    al_destroy_bitmap(self->Snake_tail_image);
    self->Ground_image = nullptr;
    self->Stone_image = nullptr;
    self->Apple_image = nullptr;
    self->Snake_head_image = nullptr;
    self->Snake_body_straight_image = nullptr;
    self->Snake_body_turn_image = nullptr;
    self->Snake_tail_image = nullptr;
    self->End_point_image = nullptr;
    self->Buttom_image = nullptr;
    self->Spike_image = nullptr;

    Snake_destory(&self->snake);
    Map_destory(&self->map);
    Map_destory(&self->ground_map);
    Map_destory(&self->ob_map);
    Map_destory(&self->snake_map);
    ObjectQueue_destroy(&self->object);
    show_msg("level destroy done");
}
void delete_Level(Interface *Iself) {
    Level_destory(Iself);
    free(Iself);
}
void Level_draw(Interface *Iself) {
    Level *self = (Level*)Iself;
    al_clear_to_color(GRAY);

    Iself->window_center = self->snake.head;
    Interface_draw(Iself);

    const pos_t x = Iself->window_center.x - Iself->window_width/2;
    const pos_t y = Iself->window_center.y - Iself->window_height/2;
    ObjectQueue *object = &self->object;
    Node* node = object->head;
    while (node != nullptr) {
        Object *o = node->obj;
        o->window_x = x;
        o->window_y = y;
        o->draw(o);
        node = next(node);
    }
    Snake_draw(&self->snake);

    draw_map(self);

}
bool Level_update(Interface *Iself) {
    Level *self = (Level*)Iself;
    Snake *snake = &self->snake;
    Map *ground_map = &self->ground_map;
    Map *ob_map = &self->ob_map;
    Map *snake_map = &self->snake_map;
    Map *map = &self->map;
    ObjectQueue *objects = &self->object;
    bool draw = false;
    // object state set
    Node* Onode = objects->head;
    while (Onode != nullptr) {
        Object * obj = Onode->obj;
        switch(obj->type) {
            case STONE: {
                Stone *stone = (Stone*)obj;
                Pos _pos = obj->pos;
                if(*is(_pos,ground_map) == EDGE) {
                    *is(_pos,ob_map) = AIR;
                    *is(_pos,map) = EDGE;
                    obj->deleter(obj);
                    Node *temp = next(Onode);
                    erase(objects, Onode);
                    Onode = temp;
                    draw = true;
                }
                else if(CanMove(self,_pos,STONE,self->Gravity)) {
                    obj->move_dirc = self->Gravity;
                    draw = true;
                }
                break;
            }
            case BUTTON: {
                Button *button = (Button*)obj;
                OBJ_TYPE T = *is(obj->pos,snake_map);
                if(T!=BODY&&T!=HEAD || button->triger) break;
                switch(button->effect) {
                    case SHORTEN: {
                        Snake_Shorten(snake);
                        break;
                    }
                    case TRIGER_SPIKE: {
                        for (size_t i = 0; i < button->Effect_len; i++) {
                            Pos pos = button->Effect_pos[i];
                            OBJ_TYPE* typ = is(pos,ground_map);
                            *typ = (*typ!=SPIKE)? SPIKE:AIR;
                        }
                        break;
                    }
                    case CH_GRAVITY: {
                        self->Gravity = (self->Gravity==DOWN)? UP:DOWN;
                    }
                    case NOEFFECT: break;
                    default: {
                        raise_warn("Unknown buttom effect");
                    }
                }
                button->triger = true;
                obj->deleter(obj);
                Node *temp = next(Onode);
                erase(objects, Onode);
                Onode = temp;
                draw = true;
                break;
            }
            default: raise_warn("Unknown object type");
        }
        if (Onode == nullptr) break;
        Onode = next(Onode);
    }

    // snake fall dectection
    snake->isFall = true;
    Node *BN = snake->body->head;
    while(BN != nullptr) {
        Object *obj = BN->obj;
        Pos pos = obj->pos;
        if(*is(pos,ground_map)==SPIKE) {
            snake->isDied = true;
            snake->isFall = false;
            break;
        }
        const Pos nextp = add(pos,DIRC_TO_POS(self->Gravity));
        if(*is(pos,ground_map)==EDGE) {
            snake->isFall = false;
            snake->isDied = true;
            break;
        }
        if(snake->isFall && !CanMove(self,pos,obj->type,self->Gravity) && *is(nextp,snake_map)!=BODY && *is(nextp,snake_map)!=HEAD){
            snake->isFall = false;
        }
        BN = next(BN);
    }

    // snake state set
    Pos nextp = Snake_Next_Pos(snake);
    if(snake->isDied) {
        show_msg("Snake died");
    }
    else if(snake->isFall) {  //falling
        show_msg("Snake fall");
        snake->move_direction = self->Gravity;
        draw = true;
    }
    else if(snake->move_direction==NONE) {}
    else if(*is(nextp,map)==APPLE) {  //eat apple
        show_msg("Snake eat apple");
        snake->can_eat_apple = true;
        *is(nextp,map) = AIR;
        *is(nextp,ground_map) = AIR;
        draw = true;
    }
    else if(CanMove(self,snake->head,HEAD,snake->move_direction)) {  //move
        show_msg("Snake move");
        if(*is(nextp,map)==STONE) {
            Node * ON = objects->head;
            while(ON != nullptr) {
                Object *obj = ON->obj;
                if(equal(obj->pos, nextp))
                    obj->move_dirc = snake->move_direction;
                ON = next(ON);
            }
        }
        draw = true;
    }
    else {
        show_msg("Snake can't move because obstacle");
        snake->move_direction = NONE;
        draw = true;
    }

    // delete all object to temp map
    BN = snake->body->head;
    while (BN != nullptr) {
        Object *obj = BN->obj;
        Pos pos = obj->pos;
        *is(pos,snake_map) = AIR;
        *is(pos,map) = AIR;
        BN = next(BN);
    }
    Node *ON = objects->head;
    while (ON != nullptr) {
        Object *obj = ON->obj;
        Pos pos = obj->pos;
        *is(pos,ob_map) = AIR;
        *is(pos,map) = AIR;
        ON = next(ON);
    }

    // update snake
    Snake_update(snake);

    // update object
    ON = objects->head;
    while (ON != nullptr) {
        Object *obj = ON->obj;
        obj->update(obj);
        ON = next(ON);
    }

    // load all object to temp map
    BN = snake->body->head;
    while (BN != nullptr) {
        Object *obj = BN->obj;
        Pos pos = obj->pos;
        *is(pos,snake_map) = obj->type;
        *is(pos,map) = obj->type;
        BN = next(BN);
    }
    ON = objects->head;
    while (ON != nullptr) {
        Object *obj = ON->obj;
        Pos pos = obj->pos;
        *is(pos,ob_map) = obj->type;
        *is(pos,map) = obj->type;
        ON = next(ON);
    }

    if(snake->isDied==true) {   // if is died
        self->level_stat = RESTART;
        Interface_stop_sound(Iself);
        return false;
    }
    else if(*is(snake->head,ground_map)==END){  // if reach end
        self->level_stat = NEXT;
        Interface_stop_sound(Iself);
        return false;
    }

    if(draw && Show) print_map(self);

    return true;
}
bool Level_load_level(Level *self, int _level_idx) {
    show_msg("Load level begin");

    show_msg("Set level id");
    self->level_idx = _level_idx;

    // backbackground image
    self->Fix_Background_image = al_load_bitmap("data/image/fix_background.png");
    if(self->Fix_Background_image==nullptr) {
        raise_warn("Load fix background fail");
    }

    show_msg("Load image begin");

    self->Ground_image = al_load_bitmap("data/image/ground.png");
    self->Stone_image = al_load_bitmap("data/image/stone.png");
    self->Apple_image = al_load_bitmap("data/image/apple.png");
    self->Snake_head_image = al_load_bitmap("data/image/snakeHead.png");
    self->Snake_body_straight_image = al_load_bitmap("data/image/snakeBody_straight.png");
    self->Snake_body_turn_image = al_load_bitmap("data/image/snakeBody_turn.png");
    self->Snake_tail_image = al_load_bitmap("data/image/snakeTail.png");
    self->End_point_image = al_load_bitmap("data/image/end.png");
    self->Buttom_image = al_load_bitmap("data/image/buttom.png");
    self->Spike_image = al_load_bitmap("data/image/spike.png");
    self->Spike_down_image = al_load_bitmap("data/image/spike_down.png");

    if((self->Ground_image &&
        self->Stone_image &&
        self->Apple_image &&
        self->Snake_head_image &&
        self->Snake_body_straight_image &&
        self->Snake_body_turn_image &&
        self->Snake_tail_image &&
        self->End_point_image &&
        self->Buttom_image &&
        self->Spike_image) == false) {
        raise_warn("Some image load fail");
    }
    show_msg("Load image done");

    show_msg("Load level file begin");
    char tmp[MAX_PATH_LEN];
    sprintf(tmp, "data/level/L%d", self->level_idx);
    show_msg("load level");
    show_msg(tmp);
    FILE *fin = fopen(tmp, "r");
    if(fin == nullptr) {
        sprintf(tmp, "Level%d loaded fail, no Level%d's data.", self->level_idx,self->level_idx);
        raise_err(tmp);
        return false;
    }
    int t = 6;
    char data_type[20], path[MAX_PATH_LEN];
    int m = 0, n = 0;
    while (t--)
    {
        fscanf(fin, "%s", data_type);
        switch(data_type[0])
        {
            case '1': {  //background
                show_msg("load background");
                //fin >> m;
                fscanf(fin, "%d", &m);
                while (m--)
                {
                    //fin >> path;
                    fscanf(fin, "%s", path);
                }
                break;
            }
            case '2': {  //music
                show_msg("load music");
                //fin >> m;
                fscanf(fin, "%d", &m);
                while (m--)
                {
                    //fin >> path;
                    fscanf(fin, "%s", path);
                }
                break;
            }
            case '3': {  //snake
                show_msg("load snake initial");
                //fin >> m;
                fscanf(fin, "%d", &m);
                //vector <Pos> snake_position_vector(m);  //snake_posotion_vector {{y1,x1},{y2,y2}}
                Pos snake_position_vector[m];
                for (int i = 0; i < m; i++)
                {
                    //fin >> snake_position_vector[i].first >> snake_position_vector[i].second;
                    fscanf(fin, "%lf %lf", &snake_position_vector[i].y, &snake_position_vector[i].x);
                }
                Snake_init(&self->snake,snake_position_vector,m,self->Snake_head_image,
                                                        self->Snake_body_straight_image,
                                                        self->Snake_body_turn_image,
                                                        self->Snake_tail_image);
                break;
            }
            case '4': {  //stone
                show_msg("load stone");
                //fin >> m;
                fscanf(fin, "%d", &m);
                Pos stone_position;  //stone_posotion {y1,x1}
                for (int i = 0; i < m; i++)
                {
                    //fin >> stone_position.first >> stone_position.second;
                    fscanf(fin, "%lf %lf", &stone_position.y, &stone_position.x);
                    Object * temp = (Object*)new_Stone(stone_position,self->Stone_image);
                    //object.emplace_back(temp);
                    push_back(&self->object, temp);
                }
                break;
            }
            case '5': {  //button
                show_msg("load button");
                //fin >> m;
                fscanf(fin, "%d", &m);
                Pos button_position;  //button_posotion {y1,x1}
                for (int i = 0; i < m; i++)
                {
                    //fin >> button_position.first >> button_position.second >> path;
                    fscanf(fin, "%lf %lf %s", &button_position.y, &button_position.x, path);
                    Object * newOb = nullptr;
                    switch (path[1]){
                        case 'h':{   //Short
                            newOb = (Object*)new_Button(button_position,self->Buttom_image,SHORTEN,nullptr,0);
                            //object.emplace_back(newOb);
                            push_back(&self->object, newOb);
                            break;}
                        case 'p':{   //Spike
                            //std::vector<Pos> Spike;
                            Pos Spike[100];
                            Pos spike_position;  //spike_posotion {y1,x1}
                            //fin >> n;
                            fscanf(fin, "%d", &n);
                            size_t i = n;
                            while (i--)
                            {
                                //fin >> spike_position.first >> spike_position.second;
                                fscanf(fin, "%lf %lf", &spike_position.y, &spike_position.x);
                                //Spike.emplace_back(spike_position);
                                Spike[i] = spike_position;
                            }
                            //object.emplace_back(new Button(button_position,Buttom_image,TRIGER_SPIKE,Spike));
                            newOb = (Object*)new_Button(button_position,self->Buttom_image,TRIGER_SPIKE,Spike,n);
                            push_back(&self->object, newOb);
                            break;}
                        case 'r':{   //Grave
                            //object.emplace_back(new Button(button_position,Buttom_image,CH_GRAVITY,std::vector<Pos>(0)));
                            newOb = (Object*)new_Button(button_position,self->Buttom_image,CH_GRAVITY,nullptr,0);
                            push_back(&self->object, newOb);
                            break;}
                        default: {
                            raise_warn("Can not load unknow type buttom");
                            break;
                        }
                    }
                }
                break;
            }
            case '6': {  //map
                show_msg("load map");
                //fin >> m >> n;
                fscanf(fin, "%d %d", &m, &n);
                self->mapw = n;
                self->maph = m;
                //map.resize(m,std::vector<OBJ_TYPE>(n,AIR));
                Map_resize(&self->map, m, n, AIR);
                Map_resize(&self->ground_map, m, n, AIR);
                Map_resize(&self->ob_map, m, n, AIR);
                Map_resize(&self->snake_map, m, n, AIR);
                int element;
                for (int i = 0; i < m; i++)
                {
                    for (int j = 0; j < n; j++)
                    {
                        //fin >> element;
                        fscanf(fin, "%d", &element);
                        //OBJ_TYPE typ = static_cast<OBJ_TYPE>(element);
                        OBJ_TYPE typ = (OBJ_TYPE)element;
                        switch(typ) {
                            case EDGE:
                            case AIR:
                            case GROUND:
                            case END:
                            case APPLE:
                            case SPIKE: {
                                self->ground_map.map[i][j] = typ;
                                self->map.map[i][j] = typ;
                                break;
                            }
                            case HEAD:
                            case BODY: {
                                self->snake_map.map[i][j] = typ;
                                self->map.map[i][j] = typ;
                            }
                            case BUTTON:
                            case STONE: {
                                self->ob_map.map[i][j] = typ;
                                self->map.map[i][j] = typ;
                                break;
                            }
                            default: {
                                raise_warn("Unknown type when load level");
                                self->map.map[i][j] = typ;
                                break;
                            }
                        }
                    }
                }
                break;
            }

            default: {
                sprintf(tmp, "Level%d loaded fail, Level%d's data wrong.", self->level_idx,self->level_idx);
                raise_err(tmp);
                fclose(fin);
                return false;
            }
        }
    }
    show_msg("load level done");
    fclose(fin);
    print_map(self);
    return true;
}
void Level_level_reset(Level *self, int level_idx) {
    show_msg("Level reset begin");
    Level_destory((Interface*)self);
    self->infer.key_lock = false;
    Level_load_level(self, level_idx);
    show_msg("Level reset done");
}
void print_map(Level *self) {
    show_msg("print map begin");
    for (int i = 0; i < self->map.y_size; i++)
    {
        for (int j = 0; j < self->map.x_size; j++)
        {
            switch(self->map.map[i][j])
            {
                case AIR:
                    //cout << "  ";
                    wprintf(L"  ");
                    break;
                case STONE:
                    //cout << "░░";
                    wprintf(L"░░");
                    break;
                case GROUND:
                    //cout << "██";
                    wprintf(L"██");
                    break;
                case END:
                    //cout << " ⊠";
                    wprintf(L" ⊠");
                    break;
                case APPLE:
                    //cout << " ⋄";
                    wprintf(L" ⋄");
                    break;
                case EDGE:
                    //cout << "▞▞";
                    wprintf(L"▞▞");
                    break;
                case SPIKE:
                case SPIKE_DOWN:
                    //cout << "||";
                    wprintf(L"||");
                    break;
                case BUTTON:
                    //cout << " ▂";
                    wprintf(L" ▂");
                    break;
                case BODY:
                    //cout << "▒▒";
                    wprintf(L"▒▒");
                    break;
                case HEAD:
                    //cout << "▓▓";
                    wprintf(L"▓▓");
                    break;
                default:
                    //cout << "**";
                    wprintf(L"**");
                    break;
            }
        }
        wprintf(L"\n");
    }
    show_msg("print map done");
}
void print_map_id(Level *self) {
    show_msg("print map begin");
    //setlocale(LC_ALL, "");
    //for (auto i:map)
    for (int i = 0; i < self->map.y_size; i++)
    {
        //for (auto j:i)
        for (int j = 0; j < self->map.x_size; j++)
        {
            wprintf(L"%d ", (int)self->map.map[i][j]);
        }
        wprintf(L"\n");
    }
    show_msg("print map done");
}
GAME_STATE Level_key_triger(Level *self, int key) {
    switch(key) {
        case ALLEGRO_KEY_P: {
            show_msg("Key triger : switch to game menu");
            return GAME_LEVEL_PUASE;
        }
        case ALLEGRO_KEY_N: {
            show_msg("Key triger : jump to next level");
            self->level_stat = NEXT;
            return GAME_LEVEL;
        }
        case ALLEGRO_KEY_Q: {
            show_msg("Key triger : quit game");
            Interface_stop_sound((Interface*)self);
            return GAME_MENU;
        }
        case ALLEGRO_KEY_R:
            show_msg("Key triger : reset level");
            self->level_stat = RESTART;
            return GAME_LEVEL;
        case ALLEGRO_KEY_UP:
        case ALLEGRO_KEY_DOWN:
        case ALLEGRO_KEY_LEFT:
        case ALLEGRO_KEY_RIGHT: {
            show_msg("Key triger : move");
            self->snake.move_direction = KEY_TO_DIRC(key);
            return GAME_LEVEL;
        }
        default: {
            show_msg("Unknown key in level");
            return GAME_LEVEL;
        }
    }
}
DIRCTION KEY_TO_DIRC(int key) {
    switch(key) {
        case ALLEGRO_KEY_LEFT:  return LEFT;
        case ALLEGRO_KEY_RIGHT: return RIGHT;
        case ALLEGRO_KEY_UP:    return UP;
        case ALLEGRO_KEY_DOWN:  return DOWN;
        default :               {
            raise_warn("Unknown key");
            return NONE;
        }
    }
}
bool CanMove(Level *self, Pos now, OBJ_TYPE type, DIRCTION dirc) {
    if(dirc == NONE) return false;
    const Pos dP = DIRC_TO_POS(dirc);
    const Pos next = add(now,dP);
    const Pos next_next = add(next,dP);
    OBJ_TYPE NT = *is(next,&self->map);
    OBJ_TYPE NNT = *is(next_next,&self->map);

    switch(NT) {
        case HEAD:
        case BODY:
        case GROUND:
        case APPLE: return false;
        case BUTTON:
        case SPIKE:
        case EDGE:
        case AIR:
        case END:   return true;
        case STONE: {
            if(NNT==AIR || NNT==EDGE || NNT == END) return true;
            else return false;
        }
        default:{
            raise_warn("unknown type in map");
            return false;
        }
    }
}
void draw_map(Level *self) {
    Interface * infer = (Interface*)self;
    pos_t window_x = infer->window_center.x - infer->window_width/2;
    pos_t window_y = infer->window_center.y - infer->window_height/2;
    for(int i=0;i<self->map.y_size;++i) {
        for(int j=0;j<self->map.x_size;++j){
            if(j<window_x || j>window_x+infer->window_width || i<window_y || i>window_y+infer->window_height) continue;
            ALLEGRO_BITMAP * img;
            switch(self->ground_map.map[i][j]) {
                case AIR :
                case EDGE : continue;
                case GROUND: {
                    img = self->Ground_image;
                    break;
                }
                case END: {
                    img = self->End_point_image;
                    break;
                }
                case APPLE: {
                    img = self->Apple_image;
                    break;
                }
                case SPIKE:{
                    if (self->ground_map.map[i+1][j] == GROUND)
                    {
                        img = self->Spike_image;
                    }
                    else
                    {
                        img = self->Spike_down_image;
                    }
                    break;
                }
                default: {
                    raise_warn("wrong object type while write map");
                    img = nullptr;
                }
            }
            if(img==nullptr) {
                raise_warn("try to draw null object in ground map"); 
                return;
            }

            al_draw_scaled_bitmap(img,
                                  0,0,
                                  al_get_bitmap_width(img) ,al_get_bitmap_height(img),
                                  CHUNK_WIDTH *(j -window_x - OBJECT_IMAGE_SIZE/2),
                                  CHUNK_HEIGHT*(i -window_y - OBJECT_IMAGE_SIZE/2),
                                  OBJECT_IMAGE_SIZE*CHUNK_WIDTH,OBJECT_IMAGE_SIZE*CHUNK_HEIGHT,
                                  0
                                 );
        }
    }
}

/*
#include "Level.h"
#include "GameWindow.h"
#include <iostream>
#include <fstream>
#include <string>

void Level::draw_map() {
    float window_x = window_center.second - window_width/2;
    float window_y = window_center.first - window_height/2;
    for(int i=0;i<map.size();++i) {
        for(int j=0;j<map[0].size();++j){
            if(j<window_x || j>window_x+window_width || i<window_y || i>window_y+window_height) continue;
            ALLEGRO_BITMAP * img;
            switch(ground_map[i][j]) {
                case AIR :
                case EDGE : continue;
                case GROUND: {
                    img = Ground_image;
                    break;
                }
                case END: {
                    img = End_point_image;
                    break;
                }
                case APPLE: {
                    img = Apple_image;
                    break;
                }
                case SPIKE:{
                    if (ground_map[i+1][j] == GROUND)
                    {
                        img = Spike_image;
                    }
                    else
                    {
                        img = Spike_down_image;
                    }
                    break;
                }
                default: {
                    raise_warn("wrong object type while write map");
                    img = nullptr;
                }
            }
            if(img==nullptr) {
                raise_warn("try to draw null object in ground map"); 
                return;
            }

            al_draw_scaled_bitmap(img,
                                  0,0,
                                  al_get_bitmap_width(img) ,al_get_bitmap_height(img),
                                  CHUNK_WIDTH *(j -window_x - OBJECT_IMAGE_SIZE/2),
                                  CHUNK_HEIGHT*(i -window_y - OBJECT_IMAGE_SIZE/2),
                                  OBJECT_IMAGE_SIZE*CHUNK_WIDTH,OBJECT_IMAGE_SIZE*CHUNK_HEIGHT,
                                  0
                                 );
        }
    }
}

void Level::draw() {
    al_clear_to_color(GRAY);

    window_center = snake->head;
    Interface::draw();

    const double x = window_center.second - window_width/2;
    const double y = window_center.first - window_height/2;
    for(auto &o:object) {
        o->window_x = x;
        o->window_y = y;
        o->draw();
    }
    snake->draw();

    draw_map();
}

bool Level::CanMove(Pos now,OBJ_TYPE T,DIRCTION dirc) {
    if(dirc == NONE) return false;
    const Pos dP = DIRC_TO_POS(dirc);
    const Pos next = {now.first+dP.first,now.second+dP.second};
    const Pos next_next = {next.first+dP.first,next.second+dP.second};
    OBJ_TYPE NT = is(next,map);
    OBJ_TYPE NNT = is(next_next,map);

    switch(NT) {
        case HEAD:
        case BODY:
        case GROUND: 
        case APPLE: return false;
        case BUTTON:
        case SPIKE:
        case EDGE: 
        case AIR:
        case END:   return true;
        case STONE: {
            if(NNT==AIR || NNT==EDGE || NNT == END) return true;
            else return false;
        }
        default:{
            raise_warn("unknown type in map");
            return false;
        }
    }
}

bool Level::update() {

    bool draw = false;
    // object state set
    for(auto it=object.begin();it!=object.end();++it) {
        Object * ob = *it;
        switch(ob->type) {
            case STONE: {
                Pos _pos = ob->getPos();
                if(is(_pos,ground_map)==EDGE) {
                    is(_pos,ob_map) = AIR;
                    is(_pos,map) = EDGE;
                    delete ob;
                    object.erase(it++);
                    draw = true;
                }
                else if(CanMove(ob->getPos(),STONE,Gravity)) {
                    ob->move_dirc = Gravity;
                    draw = true;
                }
                break;
            }
            case BUTTON: {
                OBJ_TYPE T = is(ob->getPos(),snake_map);
                if(T!=BODY&&T!=HEAD || ob->triger) break;
                switch(ob->effect) {
                    case SHORTEN: {
                        snake->Shorten();
                        break;
                    }
                    case TRIGER_SPIKE: {
                        for(auto &p:ob->Effect_pos) {
                            OBJ_TYPE & typ = is(p,ground_map);
                            typ = (typ!=SPIKE)? SPIKE:AIR;
                        }
                        break;
                    }
                    case CH_GRAVITY: {
                        Gravity = (Gravity==DOWN)? UP:DOWN;
                    }
                    case NOEFFECT: break;
                    default: {
                        raise_warn("Unknown buttom effect");
                    }
                }
                ob->triger = true;
                delete ob;
                object.erase(it++);
                draw = true;
                break;
            }
            default: raise_warn("Unknown object type");
        }
    }

    // snake fall dectection
    snake->isFall = true;
    for(auto &b:snake->body) {
        Pos pos = b->getPos();
        if(is(pos,ground_map)==SPIKE) {
            snake->isDied = true;
            snake->isFall = false;
            break;
        }
        const Pos dP = DIRC_TO_POS(Gravity);
        const Pos next = {pos.first+dP.first,pos.second+dP.second};
        if(is(pos,ground_map)==EDGE) {
            snake->isFall = false;
            snake->isDied = true;
            break;
        }
        if(snake->isFall && !CanMove(pos,b->type,Gravity) && is(next,snake_map)!=BODY && is(next,snake_map)!=HEAD){
            snake->isFall = false;
        }
    }

    // snake state set
    Pos next = snake->Next_Pos();
    if(snake->isDied) {
        show_msg("Snake died");
    }
    else if(snake->isFall) {  //falling
        show_msg("Snake fall");
        snake->move_direction = Gravity;
        draw = true;
    }
    else if(snake->move_direction==NONE) {}
    else if(is(next,map)==APPLE) {  //eat apple
        show_msg("Snake eat apple");
        snake->can_eat_apple = true;
        is(next,map) = AIR;
        is(next,ground_map) = AIR;
        draw = true;
    }
    else if(CanMove(snake->head,HEAD,snake->move_direction)) {  //move
        show_msg("Snake move");
        if(is(next,map)==STONE) {
            for(auto &ob:object) {
                if(ob->getPos() == next) ob->move_dirc = snake->move_direction;
            }
        }
        draw = true;
    }
    else {
        show_msg("Snake can't move because obstacle");
        snake->move_direction = NONE;
        draw = true;
    }

    // delete all object to temp map
    for(const auto &b:snake->body) {
        is(b->getPos(),snake_map) = AIR;
        is(b->getPos(),map) = AIR;
    }
    for(const auto &o:object) {
        is(o->getPos(),ob_map) = AIR;
        is(o->getPos(),map) = AIR;
    }

    // update snake
    snake->update();
    
    // update object
    for(auto &o:object) {
        o->update();
    }

    // load all object to temp map
    for(const auto &b:snake->body) {
        is(b->getPos(),snake_map) = b->type;
        is(b->getPos(),map) = b->type;
    }
    for(const auto &o:object) {
        is(o->getPos(),ob_map) = o->type;
        is(o->getPos(),map) = o->type;
    }

    if(snake->isDied==true) {   // if is died
        level_stat = RESTART;
        stop_sound();
        return false;
    }
    else if(is(snake->head,ground_map)==END){  // if reach end
        level_stat = NEXT;
        stop_sound();
        return false;
    }

    if(draw && Show) print_map();

    return true;
}

inline DIRCTION Level::KEY_TO_DIRC(int key) {
    switch(key) {
        case ALLEGRO_KEY_LEFT:  return LEFT;
        case ALLEGRO_KEY_RIGHT: return RIGHT;
        case ALLEGRO_KEY_UP:    return UP;
        case ALLEGRO_KEY_DOWN:  return DOWN;
        default :               {
            raise_warn("Unknown key");
            return NONE;
        }
    }    
}

// process trigered by key
GAME_STATE Level::key_triger(int key) {
    switch(key) {
        case ALLEGRO_KEY_P: {
            show_msg("Key triger : switch to game menu");
            return GAME_LEVEL_PUASE;
        }
        case ALLEGRO_KEY_N: {
            show_msg("Key triger : jump to next level");
            level_stat = NEXT;
            return GAME_LEVEL;
        }
        case ALLEGRO_KEY_Q: {
            show_msg("Key triger : quit game");
            stop_sound();
            return GAME_MENU;
        }
        case ALLEGRO_KEY_R:
            show_msg("Key triger : reset level");
            level_stat = RESTART;
            return GAME_LEVEL;
        case ALLEGRO_KEY_UP:
        case ALLEGRO_KEY_DOWN:
        case ALLEGRO_KEY_LEFT:
        case ALLEGRO_KEY_RIGHT: {
            show_msg("Key triger : move");
            snake->move_direction = KEY_TO_DIRC(key);
            return GAME_LEVEL; 
        }
        default: {
            show_msg("Unknown key in level");
            return GAME_LEVEL;
        }
    }
}

// load level
bool Level::load_level(int _level_idx) 
{
    show_msg("Load level begin");

    show_msg("Set level id");
    level_idx = _level_idx;

    // backbackground image
    Fix_Background_image = al_load_bitmap((IMAGE_PATH+"/fix_background.png").c_str());
    if(Fix_Background_image==nullptr) {
        raise_warn("Load fix background fail");
    }

    show_msg("Load image begin");
    Ground_image = al_load_bitmap((IMAGE_PATH+"/ground.png").c_str());
    Stone_image = al_load_bitmap((IMAGE_PATH+"/stone.png").c_str());
    Apple_image = al_load_bitmap((IMAGE_PATH+"/apple.png").c_str());
    Snake_head_image = al_load_bitmap((IMAGE_PATH+"/snakeHead.png").c_str());
    Snake_body_straight_image = al_load_bitmap((IMAGE_PATH+"/snakeBody_straight.png").c_str());
    Snake_body_turn_image = al_load_bitmap((IMAGE_PATH+"/snakeBody_turn.png").c_str());
    Snake_tail_image = al_load_bitmap((IMAGE_PATH+"/snakeTail.png").c_str());
    End_point_image = al_load_bitmap((IMAGE_PATH+"/end.png").c_str());
    Buttom_image = al_load_bitmap((IMAGE_PATH+"/buttom.png").c_str());
    Spike_image = al_load_bitmap((IMAGE_PATH+"/spike.png").c_str());
    Spike_down_image = al_load_bitmap((IMAGE_PATH+"/spike_down.png").c_str());
    
    if((Ground_image && 
        Stone_image && 
        Apple_image && 
        Snake_head_image && 
        Snake_body_straight_image && 
        Snake_body_turn_image &&
        Snake_tail_image &&
        End_point_image &&
        Buttom_image && 
        Spike_image) == false) {
        raise_warn("Some image load fail");
    }
    show_msg("Load image done");

    show_msg("Load level file begin");
    char tmp[100];
    sprintf(tmp, "/L%d", level_idx);
    show_msg(std::string("load level: ")+tmp);
    ifstream fin(LEVEL_PATH+tmp);
    if(!fin)
    {
        sprintf(tmp, "Level%d loaded fail, no Level%d's data.", level_idx,level_idx);
        raise_err(tmp);
        fin.close();
        return false;
    }
    int t = 6;
    char data_type[20], path[100];
    int m = 0, n = 0;
    while (t--)
    {
        fin >> data_type;
        switch(data_type[0])
        {
            case '1': {  //background
                show_msg("load background");
                fin >> m;
                while (m--)
                {
                    fin >> path;
                }
                break;
            }
            case '2': {  //music
                show_msg("load music");
                fin >> m;
                while (m--)
                {
                    fin >> path;
                }
                break;
            }
            case '3': {  //snake
                show_msg("load snake initial");
                fin >> m;
                vector <Pos> snake_position_vector(m);  //snake_posotion_vector {{y1,x1},{y2,y2}}
                for (int i = 0; i < m; i++)
                {
                    fin >> snake_position_vector[i].first >> snake_position_vector[i].second;
                }
                snake = new Snake(snake_position_vector,Snake_head_image,
                                                        Snake_body_straight_image,
                                                        Snake_body_turn_image,
                                                        Snake_tail_image);
                break;
            }
            case '4': {  //stone
                show_msg("load stone");
                fin >> m;
                Pos stone_position;  //stone_posotion {y1,x1}
                for (int i = 0; i < m; i++)
                {
                    fin >> stone_position.first >> stone_position.second;
                    Object * temp = new Stone(stone_position,Stone_image);
                    object.emplace_back(temp);
                }
                break;
            }
            case '5': {  //button
                show_msg("load button");
                fin >> m;
                Pos button_position;  //button_posotion {y1,x1}
                for (int i = 0; i < m; i++)
                {
                    fin >> button_position.first >> button_position.second >> path;
                    Object * newOb = nullptr;
                    switch (path[1]){
                        case 'h':{   //Short
                            newOb = new Button(button_position,Buttom_image,SHORTEN,std::vector<Pos>(0));
                            object.emplace_back(newOb);
                            break;}
                        case 'p':{   //Spike
                            std::vector<Pos> Spike;
                            Pos spike_position;  //spike_posotion {y1,x1}
                            fin >> n;
                            while (n--)
                            {
                                fin >> spike_position.first >> spike_position.second;
                                Spike.emplace_back(spike_position);
                            }
                            object.emplace_back(new Button(button_position,Buttom_image,TRIGER_SPIKE,Spike));
                            break;}
                        case 'r':{   //Grave
                            object.emplace_back(new Button(button_position,Buttom_image,CH_GRAVITY,std::vector<Pos>(0)));
                            break;}
                        default: {
                            raise_warn("Can not load unknow type buttom");
                            break;
                        }
                    }
                }
                break;
            }
            case '6': {  //map
                show_msg("load map");
                fin >> m >> n;
                mapw = n;
                maph = m;
                map.resize(m,std::vector<OBJ_TYPE>(n,AIR));
                ground_map = map;
                ob_map = map;
                snake_map = map;
                int element;
                for (int i = 0; i < m; i++)
                {
                    for (int j = 0; j < n; j++)
                    {
                        fin >> element;
                        OBJ_TYPE typ = static_cast<OBJ_TYPE>(element);
                        switch(typ) {
                            case EDGE:
                            case AIR:
                            case GROUND:
                            case END:
                            case APPLE:
                            case SPIKE: {
                                ground_map[i][j] = typ;
                                map[i][j] = typ;
                                break;
                            }
                            case HEAD:
                            case BODY: {
                                snake_map[i][j] = typ;
                                map[i][j] = typ;
                            }
                            case BUTTON:
                            case STONE: {
                                ob_map[i][j] = typ;
                                map[i][j] = typ;
                                break;
                            }
                            default: {
                                raise_warn("Unknown type when load level");
                                map[i][j] = typ;
                                break;
                            }
                        }
                    }
                }
                break;
            }

            default: {
                sprintf(tmp, "Level%d loaded fail, Level%d's data wrong.", level_idx,level_idx);
                raise_err(tmp);
                fin.clear();
                fin.close();
                return false;
            }
        }
    }
    show_msg("load level done");
    fin.clear();
    fin.close();
    print_map();
    return true;
}

// print level map
void Level::print_map()
{
    show_msg("print map begin");
    for (auto i:map)
    {
        for (auto j:i)
        {
            switch(j)
            {
                case AIR:
                    cout << "  ";
                    break;
                case STONE:
                    cout << "░░";
                    break;
                case GROUND:
                    cout << "██";
                    break;
                case END:
                    cout << " ⊠";
                    break;
                case APPLE:
                    cout << " ⋄";
                    break;
                case EDGE:
                    cout << "▞▞";
                    break;
                case SPIKE:
                case SPIKE_DOWN:
                    cout << "||";
                    break;
                case BUTTON:
                    cout << " ▂";
                    break;
                case BODY:
                    cout << "▒▒";
                    break;
                case HEAD:
                    cout << "▓▓";
                    break;
                default:
                    cout << "**";
                    break;
            }
        }
        cout << endl;
    }
    show_msg("print map done");
};

// reset
void Level::level_reset(int idx) {
    show_msg("Level reset begin");
    destroy_level();
    key_lock = false;
    load_level(idx);
    show_msg("Level reset done");
}

void Level::destroy_level() {
    show_msg("level destroy begin");
    Interface::~Interface();
    
    al_destroy_bitmap(Ground_image);
    al_destroy_bitmap(Stone_image);
    al_destroy_bitmap(Apple_image);
    al_destroy_bitmap(Snake_head_image);
    al_destroy_bitmap(Snake_body_straight_image);
    al_destroy_bitmap(Snake_body_turn_image);
    al_destroy_bitmap(End_point_image);
    al_destroy_bitmap(Buttom_image);
    al_destroy_bitmap(Spike_image);
    al_destroy_bitmap(Snake_tail_image);
    Ground_image = nullptr;
    Stone_image = nullptr;
    Apple_image = nullptr;
    Snake_head_image = nullptr;
    Snake_body_straight_image = nullptr;
    Snake_body_turn_image = nullptr;
    Snake_tail_image = nullptr;
    End_point_image = nullptr;
    Buttom_image = nullptr;
    Spike_image = nullptr;

    key_lock = false;
    int key_lock_count = 0;

    delete snake;
    for(auto &o:object) {
        delete o;
        o = nullptr;
    }
    show_msg("level destroy done");
}

// constructor and deletor
Level::Level(int i):
       Interface(MUSIC_PATH+"/level_bgm.ogg",IMAGE_PATH+"/background.png",IMAGE_PATH+"/fog.png") 
{
    show_msg("Create level begin");
    key_lock = false;
    level_stat = KEEP;
    load_level(i);
    show_msg("Create level done");
}

Level::~Level() {
    show_msg("Delete level begin");
    destroy_level();
    show_msg("Delete level done");
}
*/