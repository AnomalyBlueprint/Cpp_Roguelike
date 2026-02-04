#include "Game.h"

int main(int argc, char *argv[])
{
    Game *game = new Game();

    game->Init("Cpp_Roguelike", 800, 600);

    // The loop is now inside Run(), managed by the Accumulator
    game->Run();

    game->Clean();
    delete game;

    return 0;
}