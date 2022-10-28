// SPDX-FileCopyrightText: 2022 Enrique M.G. <quiqueiii@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "level.hpp"
#include "game.hpp"

#include <iostream>

using namespace lluna;

using namespace std;

int main (int argc,char* argv[])
{
    clog<<"Welcome to lluna!"<<endl;

    Game::game* Game::create();
    game->loop();

    Game::destroy();

    return 0;
}
