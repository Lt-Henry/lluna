// SPDX-FileCopyrightText: 2022 Enrique M.G. <quiqueiii@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "game.hpp"
#include "atlas.hpp"

#include <SDL2/SDL.h>
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

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    _tiles[0] = new Atlas("tileset.png");
    _tiles[0]->set_sprite_size(64,64);
    //_tiles[1] = new Atlas("L1.png");

    _level[0]= new Level("test.csv");
    //_level[1]= new Level("level0_L1.csv");

    camx = 0;
    camy = 0;

    _cam_pos = {0,0};

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

                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            quit_request = true;
                        break;

                        case SDLK_UP:
                            _cam_target.y = _cam_pos.y - 100;
                        break;

                        case SDLK_DOWN:
                            _cam_target.y = _cam_pos.y + 100;
                        break;

                        case SDLK_RIGHT:
                            _cam_target.x = _cam_pos.x + 100;
                        break;

                        case SDLK_LEFT:
                            _cam_target.x = _cam_pos.x - 100;
                        break;

                        case SDLK_h:
                            move_camera(0,0);
                        break;

                        case SDLK_p:
                            move_camera(900,900);
                        break;

                    }
                break;
            }
        }

        //updating
        int dx =  _cam_target.x - _cam_pos.x;
        int dy =  _cam_target.y - _cam_pos.y;

        _cam_pos.x+=dx;
        _cam_pos.y+=dy;

        //rendering
        SDL_SetRenderDrawColor(_renderer, 0x00, 0x98, 0xdc, 0xff);
        SDL_RenderClear(_renderer);

        for (int i=0;i<40;i++) {
            for (int j=0;j<22;j++) {
                int tx = _cam_pos.x/64;
                int ty = _cam_pos.y/64;
                int l0 = _level[0]->get(tx+i,ty+j);
                SDL_Rect dest;
                dest.x=i*64;
                dest.y=j*64;
                dest.w=64;
                dest.h=64;

                if (l0>-1) {
                    int ax = l0 % 16;
                    int ay = l0 / 16;

                    _tiles[0]->draw(ax,ay,dest);
                }
            }
        }

        SDL_RenderPresent(_renderer);
    }
}

void Game::move_camera(int x,int y)
{
    _cam_target.x = x;
    _cam_target.y = y;
}
