// SPDX-FileCopyrightText: 2022 Enrique M.G. <quiqueiii@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "level.hpp"

#include <SDL2/SDL.h>

#include <string>

namespace lluna
{

    class Atlas;
    class Game
    {
        public:

        static Game* create();
        static Game* get();
        static void destroy();

        void init();
        void loop();

        SDL_Renderer* renderer() const
        {
            return _renderer;
        }

        SDL_Window* window() const
        {
            return _window;
        }

        void move_camera(int x,int y);

        private:
        Game();
        ~Game();

        SDL_Renderer* _renderer;
        SDL_Window* _window;

        Level _level[2];
        Atlas* _tiles[2];

        int camx,camy;

        SDL_Point _cam_pos;
        SDL_Point _cam_target;

        static Game* _instance;
    };
}
