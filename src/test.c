#include "GameWindow.h"
#include "Level.h"
#include <stdio.h>
#include <wchar.h>
#include <locale.h>

int fmain()
{
    setlocale(LC_ALL, "");
    GameWindow* SnakeGame = new_GameWindow();
    GameWindow_game_load(SnakeGame);
    GameWindow_draw(SnakeGame);
    delete_GameWindow(SnakeGame);
    return 0;
}