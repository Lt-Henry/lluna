// SPDX-FileCopyrightText: 2022 Enrique M.G. <quiqueiii@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <SDL2/SDL.h>

namespace lluna
{
    class Atlas
    {
        public:

        Atlas(const char* filename);
        ~Atlas();

        void set_sprite_size(int w,int h)
        {
            _tw = w;
            _th = h;
        }

        void draw(int i,int j,SDL_Rect dest);

        private:

        int _tw;
        int _th;
        SDL_Texture* _texture;
        SDL_Renderer* _renderer;

    };
}
