#include "Game.h"
#include "locale.h"

int main() {
    setlocale(LC_ALL, "");

    Game* SnakeGame = new_Game();

    Game_run(SnakeGame);

    delete_Game(SnakeGame);
}