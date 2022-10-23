// SPDX-FileCopyrightText: 2022 Enrique M.G. <quiqueiii@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <memory>

namespace lluna
{
    class Level
    {
        protected:

        std::shared_ptr<int[]> data;
        int _width;
        int _height;

        public:

        Level(const char* filename);

        int get(int x,int y);

        const int width() const { return _width; }
        const int height() const { return _height; }
    };
}
