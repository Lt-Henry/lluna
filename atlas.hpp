// SPDX-FileCopyrightText: 2022 Enrique M.G. <quiqueiii@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <SDL2/SDL.h>

namespace lluna
{
    class Atlas
    {
        public:

        Atlas(const char* filename, int sprite_w=32,int sprite_h=32);
        ~Atlas();

        void set_sprite_size(int w,int h)
        {
            _tw = w;
            _th = h;
            compute_size();
        }

        void draw(int i,int j,SDL_Rect dest);

        int width() const
        {
            return _width;
        }

        int height() const
        {
            return _height;
        }

        int rows() const
        {
            return _rows;
        }

        int columns() const
        {
            return _columns;
        }

        int sprite_width() const
        {
            return _tw;
        }

        int sprite_height() const
        {
            return _th;
        }

        private:

        void compute_size();

        int _tw;
        int _th;
        int _width;
        int _height;
        int _rows;
        int _columns;

        SDL_Texture* _texture;
        SDL_Renderer* _renderer;

    };
}
