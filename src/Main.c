// rewrite it to c language
#include "GameWindow.h"
#include "stdlib.h"
#include "locale.h"

int main() {
    setlocale(LC_ALL, "");

    GameWindow* SnakeGame = new_GameWindow();

    //SnakeGame->game_play();
    GameWindow_game_play(SnakeGame);

    delete_GameWindow(SnakeGame);
}