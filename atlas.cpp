// SPDX-FileCopyrightText: 2022 Enrique M.G. <quiqueiii@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "atlas.hpp"

#include <SDL2/SDL_image.h>

#include <iostream>
#include <fstream>

using namespace lluna;

using namespace std;

class StaticLoader
{
    StaticLoader()
    {
        if (IMG_Init() == 0) {
            cerr<<"Failed to init SDL_Image"<<endl;
        }
    }

    ~StaticLoader()
    {
        IMG_Quit();
    }
};

StaticLoader loader;

Atlas::Atlas(const char* filename)
{
    //SDL_Renderer* renderer = Game::get()->renderer;
    SDL_Surface* data = IMG_Load(filename);

    if (!data) {
        cerr<<"Failed to load image:"<<filename<<endl;
    }

    //this->texture = SDL_CreateTextureFromSurface(renderer,data);

    SDL_FreeSurface(data);
}

Atlas::~Atlas()
{
    SDL_DestroyTexture(texture);
}
