// SPDX-FileCopyrightText: 2024 Enrique M.G. <quiqueiii@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstdint>

namespace lluna
{
    class Tile
    {
        public:
        
        uint32_t data;
        
        Tile();
        Tile(uint32_t value);
        
        uint32_t get_tile();
        void set_tile(uint32_t value);
        
        uint32_t get_status();
        void set_status(uint32_t value);
        
        bool empty();
        
    };
}
