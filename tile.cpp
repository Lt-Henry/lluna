
#include "tile.hpp"

using namespace lluna;

using namespace std;


Tile::Tile() : Tile(0)
{
}

Tile::Tile(uint32_t value) : data(value)
{
}

uint32_t Tile::get_tile()
{
    return data & 0x000000FF;
}

void Tile::set_tile(uint32_t value)
{
    data = data | (value & 0x000000FF);
}

uint32_t Tile::get_status()
{
    return (data & 0x0000FF00) >> 8;
}

void Tile::set_status(uint32_t value)
{
    data = data | ((value & 0x000000FF)<<8);
}


bool Tile::empty()
{
    return (data & 0xE0000000) == 0xE0000000;
}

