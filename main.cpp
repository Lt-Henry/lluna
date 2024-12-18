// SPDX-FileCopyrightText: 2022 Enrique M.G. <quiqueiii@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "game.hpp"

#include <iostream>

using namespace lluna;

using namespace std;

int main (int argc,char* argv[])
{
    clog<<"Welcome to lluna!"<<endl;

    Game* game = Game::create();
    game->init(argc,argv);
    game->loop();

    Game::destroy();

    return 0;
}
