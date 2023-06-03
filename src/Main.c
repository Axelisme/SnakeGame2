// rewrite it to c language
#include "GameWindow.h"
#include "stdlib.h"

int main() {
    GameWindow* SnakeGame = new_GameWindow();

    //SnakeGame->game_play();
    GameWindow_game_play(SnakeGame);

    delete_GameWindow(SnakeGame);
}