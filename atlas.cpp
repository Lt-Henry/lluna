// SPDX-FileCopyrightText: 2022 Enrique M.G. <quiqueiii@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "atlas.hpp"
#include "game.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <fstream>

using namespace lluna;

using namespace std;

Atlas::Atlas(const char* filename, int sprite_w, int sprite_h) : _tw(sprite_w),_th(sprite_h)
{
    _renderer = Game::get()->renderer();
    SDL_Surface* data = IMG_Load(filename);

    if (!data) {
        cerr<<"Failed to load image:"<<filename<<endl;
    }

    _texture = SDL_CreateTextureFromSurface(_renderer,data);

    SDL_FreeSurface(data);

    compute_size();
}

Atlas::~Atlas()
{
    SDL_DestroyTexture(_texture);
}

void Atlas::compute_size()
{
    SDL_QueryTexture(_texture, nullptr, nullptr, &_width, &_height);

    _rows = _height / _th;
    _columns = _width / _tw;
}

void Atlas::draw(int i,int j,SDL_Rect dest)
{
    SDL_Rect source;

    source.x = i * _tw;
    source.y = j * _th;
    source.w = _tw;
    source.h = _th;

    SDL_RenderCopy(_renderer,_texture,&source,&dest);
}
