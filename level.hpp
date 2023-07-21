// SPDX-FileCopyrightText: 2022 Enrique M.G. <quiqueiii@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <memory>

namespace lluna
{
    enum Tiles
    {
        Empty = -1,
        Dirt = 0,
        Grass,

        Rock = 16,
        RockIron,
        RockCoal,

        Log = 32,
        Leafs,
        Leafs2,

        Sand = 48,
        Cactus,

        Water = 64,
        Water2,

        Lava = 80,
        lava2,

        Bedrock = 96,

        Wall = 112,

        Pumpking = 128,
        PumpkingHalloween,
        Watermelon,

        TNT = 144
    };

    class Level
    {
        protected:

        int _width;
        int _height;
        int* _data;

        public:

        Level():_width(0),_height(0),_data(nullptr){};
        Level(int width,int height,int seed=0);
        Level(const char* filename);
        virtual ~Level();

        int get(int x,int y);
        void put(int x,int y,int value);

        const int width() const { return _width; }
        const int height() const { return _height; }
    };
}
