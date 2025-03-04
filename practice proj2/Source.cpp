#include <iostream>
#include"Game.h"
#include<vector>

using namespace sf;

//Not really much to comment here... fairly simple//
int main()
{

    Game game;


    ///Game Loop///
    while (game.getWindow()->isOpen())
    {
        game.update();
        game.render();
    }

    return 0;
}