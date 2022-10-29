// SPDX-FileCopyrightText: 2022 Enrique M.G. <quiqueiii@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "game.hpp"
#include "atlas.hpp"

#include <SDL2/SDL_image.h>

#include <iostream>

using namespace lluna;

using namespace std;

Game* Game::_instance = nullptr;

Game* Game::create()
{
    //ToDo: watch for created instances
    Game::_instance = new Game();
    return Game::_instance;
}

Game* Game::get()
{
    return Game::_instance;
}

void Game::destroy()
{
    delete Game::_instance;
    Game::_instance = nullptr;
}

Game::Game()
{

}

Game::~Game()
{
}

void Game::init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cerr<<"Failed to init SDL"<<endl;
    }

    // yeah, logic is inverted for IMG_Init
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        cerr<<"Failed to init SDL_Image"<<endl;
    }

    _window = SDL_CreateWindow("Lluna",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280,720, SDL_WINDOW_SHOWN);

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);

    _tiles[0] = new Atlas("L0.png");
    _tiles[1] = new Atlas("L1.png");
}

void Game::loop()
{
    bool quit_request = false;

    while (!quit_request) {

        //event handling
        SDL_Event event;

        while(SDL_PollEvent(&event)) {

            switch (event.type) {
                case SDL_QUIT:
                    clog<<"quit requested"<<endl;
                    quit_request = true;
                break;

                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        quit_request = true;
                    }
                break;
            }
        }

        //updating

        //rendering
        SDL_SetRenderDrawColor(_renderer, 0x00, 0x98, 0xdc, 0xff);
        SDL_RenderClear(_renderer);
        SDL_RenderPresent(_renderer);
    }
}
