// SPDX-FileCopyrightText: 2022 Enrique M.G. <quiqueiii@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <memory>

namespace lluna
{
    class Level
    {
        protected:

        int _width;
        int _height;
        int* _data;

        public:

        Level():_width(0),_height(0),_data(0){};
        Level(const char* filename);
        virtual ~Level();

        int get(int x,int y);

        const int width() const { return _width; }
        const int height() const { return _height; }
    };
}
