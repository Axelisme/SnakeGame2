// TODO: rewrite in c

#include "Snake.h"
#include "global.h"

Snake* new_Snake(Pos Poss[], int len,
                 ALLEGRO_BITMAP * img_head,
                 ALLEGRO_BITMAP * img_body_straight,
                 ALLEGRO_BITMAP * img_body_turn,
                 ALLEGRO_BITMAP * img_tail) {
    Snake *snake = (Snake*)malloc(sizeof(Snake));
    Snake_init(snake,Poss,len,
               img_head,
               img_body_straight,
               img_body_turn,
               img_tail);
    return snake;
}

void Snake_init(Snake *snake, Pos Poss[], int len,
                ALLEGRO_BITMAP * img_head,
                ALLEGRO_BITMAP * img_body_straight,
                ALLEGRO_BITMAP * img_body_turn,
                ALLEGRO_BITMAP * img_tail) {
    show_msg("Create snake begin");
    snake->body = new_ObjectQueue();
    // Snake Info
    snake->move_direction = NONE;
    snake->isFall = false;
    snake->can_eat_apple = false;
    snake->isDied = false;
    // Display
    snake->window_width = CHUNK_W_NUM;
    snake->window_height = CHUNK_H_NUM;
    snake->Image_head = img_head;
    snake->Image_body_straight = img_body_straight;
    snake->Image_body_turn = img_body_turn;
    snake->Image_tail = img_tail;
    // load snake bodys
    DIRCTION from = UP;
    DIRCTION to = POS_TO_DIRC(sub(Poss[1],Poss[0]));
    push_back(snake->body,(Object*)new_Body(Poss[0],BODY,snake->Image_head,
                                              snake->Image_tail,
                                              snake->Image_body_turn,
                                              to,to));
    for(int i=1;i<len-1;++i) {
        from = to;
        to = POS_TO_DIRC(sub(Poss[i+1],Poss[i]));
        push_back(snake->body,(Object*)new_Body(Poss[i],BODY,snake->Image_head,
                                                 snake->Image_body_straight,
                                                 snake->Image_body_turn,
                                                 from,to));
    }
    snake->head = Poss[len-1];
    snake->heading = to;
    push_back(snake->body,(Object*)new_Body(snake->head,HEAD,snake->Image_head,
                                             snake->Image_body_straight,
                                             snake->Image_body_turn,
                                             to,to));
    show_msg("Create Snake done");
}

void Snake_draw(Snake *snake) {
    ObjectQueue *body = snake->body;
    Node* BN = body->head;
    while(BN != NULL) {
        Object* obj = BN->obj;
        obj->window_x = snake->head.x - snake->window_width/2;
        obj->window_y = snake->head.y - snake->window_height/2;
        obj->draw(obj);
        BN = next(BN);
    }
}

bool Snake_update(Snake *snake) {
    if(snake->isDied) return false;
    else if(snake->isFall) {
        show_msg("Snake fall begin");
        Snake_Move_all(snake);
        snake->isFall = false;
        snake->move_direction = NONE;
        show_msg("Snake fall done");
        return true;
    }
    else if(snake->can_eat_apple) {
        show_msg("Snake eat apple begin");
        Snake_Move_extend(snake);
        snake->can_eat_apple = false;
        snake->move_direction = NONE;
        show_msg("Snake eat apple done");
        return true;
    }
    else if(snake->move_direction!=NONE) {
        show_msg("Snake move begin");
        Snake_Move_forward(snake);
        snake->move_direction = NONE;
        show_msg("Snake move done");
        return true;
    }
    else return false;
}

Pos Snake_Next_Pos(Snake *snake) {
    Pos dP = DIRC_TO_POS(snake->move_direction);
    return add(snake->head,dP);
}

void Snake_Move_all(Snake *snake) {
    ObjectQueue *body = snake->body;
    Node* BN = body->head;
    Object* obj = NULL;
    while(BN != NULL) {
        obj = BN->obj;
        obj->move(obj, snake->move_direction);
        BN = next(BN);
    }
    snake->head = obj->pos;
}

void Snake_Move_forward(Snake *snake) {
    Snake_Move_extend(snake);
    Snake_Shorten(snake);
}

void Snake_Move_extend(Snake *snake) {
    Pos next_pos = Snake_Next_Pos(snake);
    ObjectQueue *body = snake->body;
    Body *origin_head = (Body*) body->tail->obj;

    origin_head->obj.type = BODY;
    origin_head->to_dirc = snake->move_direction;
    snake->head = next_pos;
    snake->heading = snake->move_direction;

    push_back(body,(Object*)new_Body(next_pos,
                                        HEAD,
                                        snake->Image_head,
                                        snake->Image_body_straight,
                                        snake->Image_body_turn,
                                        snake->move_direction,
                                        snake->move_direction));
}

void Snake_Shorten(Snake *snake) {
    show_msg("snake shorten begin");
    Object* obj = pop_front(snake->body);
    obj->deleter(obj);
    if(isempty(snake->body)) {
        snake->isDied = true;
        return;
    }
    Body * NewTail = (Body*)front(snake->body);
    NewTail->image_body_straight = snake->Image_tail;
    NewTail->from_dirc = NewTail->to_dirc;
    show_msg("snake shorten end");
}

void Snake_destory(Snake *snake) {
    show_msg("destroy snake begin");
    if (snake->body == NULL) return;
    while(!isempty(snake->body)) {
        Object* obj = pop_front(snake->body);
        obj->deleter(obj);
    }
    delete_ObjectQueue(snake->body);
    snake->body = NULL;
    show_msg("destroy snake done");
}

void delete_Snake(Snake *snake) {
    Snake_destory(snake);
    free(snake);
}

/*
#include "Snake.h"

void Snake::draw() {
    for(auto &b:body) {
        b->window_x = head.second - window_width/2;
        b->window_y = head.first - window_height/2;
        b->draw();
    }
}

void Snake::Move_all() {
    for(auto &b:body){
        b->move_dirc = move_direction;
        b->update();
    }
    head = body.back()->getPos();
    move_direction = NONE;
}

void Snake::Move_forward() {
    Move_extend();
    Shorten();
}

void Snake::Move_extend() {
    Pos next_pos = Next_Pos();
    Body *origin_head = body.back();

    origin_head->type = BODY;
    origin_head->to_dirc = move_direction;
    head = next_pos;
    heading = move_direction;

    body.push_back(new Body(next_pos,
                            HEAD,
                            Image_head,
                            Image_body_straight,
                            Image_body_turn,
                            move_direction,
                            move_direction));
    move_direction = NONE;
}

void Snake::Shorten() {
    show_msg("snake shorten begin");
    delete body.front();
    body.pop_front();
    if(body.empty()) {
        isDied = true;
        return;
    }
    Body * NewTail = body.front();
    NewTail->image_body_straight = Image_tail;
    NewTail->from_dirc = NewTail->to_dirc;
    show_msg("snake shorten end");
}

bool Snake::update() {
    if(isDied) return false;
    else if(isFall) {
        show_msg("Snake fall begin");
        Move_all();
        isFall = false;
        show_msg("Snake fall done");
        return true;
    }
    else if(can_eat_apple) {
        show_msg("Snake eat apple begin");
        Move_extend();
        can_eat_apple = false;
        show_msg("Snake eat apple done");
        return true;
    }
    else if(move_direction!=NONE) {
        show_msg("Snake move begin");
        Move_forward();
        show_msg("Snake move done");
        return true;
    }
    else return false;
}

Pos Snake::Next_Pos() {
    Pos dP = DIRC_TO_POS(move_direction);
    return std::make_pair(head.first+dP.first,head.second+dP.second);
}

Snake::Snake(std::vector<Pos>& Poss,ALLEGRO_BITMAP * img_head,
                                    ALLEGRO_BITMAP * img_body_straight,
                                    ALLEGRO_BITMAP * img_body_turn,
                                    ALLEGRO_BITMAP * img_tail) {
    show_msg("Create snake begin");
    Image_head = img_head;
    Image_body_straight = img_body_straight;
    Image_body_turn = img_body_turn;
    Image_tail = img_tail;

    move_direction = NONE;
    isFall = false;
    can_eat_apple = false;
    isDied = false;

    DIRCTION from = UP;
    DIRCTION to = POS_TO_DIRC(Poss[0],Poss[1]);
    body.push_back(new Body(Poss.front(),BODY,Image_head,
                                              Image_tail,
                                              Image_body_turn,
                                              to,to));
    for(int i=1;i<Poss.size()-1;++i) {
        from = to;
        to = POS_TO_DIRC(Poss[i],Poss[i+1]);
        body.push_back(new Body(Poss[i],BODY,Image_head,
                                             Image_body_straight,
                                             Image_body_turn,
                                             from,to));
    }
    head = Poss.back();
    heading = to;
    body.push_back(new Body(head,HEAD,Image_head,
                                      Image_body_straight,
                                      Image_body_turn,
                                      to,to));

    show_msg("Create Snake done");
}

Snake::~Snake() {
    show_msg("Delete snake begin");
    for(auto &b:body) {
        delete b;
        b = nullptr;
    }
    show_msg("Delete snake done");
}
*/