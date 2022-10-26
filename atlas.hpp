// SPDX-FileCopyrightText: 2022 Enrique M.G. <quiqueiii@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <SDL2/SDL.h>

namespace lluna
{
    class Atlas
    {
        public:

            SDL_Texture* texture;

            Atlas(const char* filename);
            ~Atlas();
    };
}
