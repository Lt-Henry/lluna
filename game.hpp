// SPDX-FileCopyrightText: 2022 Enrique M.G. <quiqueiii@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "SDL_render.h"
#include <SDL2/SDL.h>

#include <string>

namespace lluna
{
    class Game
    {
        public:

        static Game* create();
        static Game* get();
        static void destroy();

        void loop();

        SDL_Renderer* renderer() const
        {
            return _renderer;
        }

        SDL_Window* window() const
        {
            return _window;
        }

        private:
        Game();
        ~Game();

        SDL_Renderer* _renderer;
        SDL_Window* _window;

        static Game* _instance;
    };
}
