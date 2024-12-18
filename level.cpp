// SPDX-FileCopyrightText: 2022 Enrique M.G. <quiqueiii@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "level.hpp"
#include "noise.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <random>

using namespace lluna;

using namespace std;

Level::Level(const char* filename)
{
    fstream file;

    file.open(filename, std::fstream::in);

    if (file.is_open()) {

        string tmp;
        vector<int> columns;
        vector< vector<int> >rows;

        char c;
        while (file.get(c)) {

            if (c==',') {
                int v = std::stoi(tmp);
                tmp="";

                columns.push_back(v);
            }
            else {
                if (c=='\n') {
                    int v = std::stoi(tmp);
                    tmp="";
                    columns.push_back(v);
                    rows.push_back(columns);
                    columns.clear();
                }
                else {
                    if ( (c>='0' and c<='9') or c=='-') {
                        tmp+=c;
                    }
                    else {
                        cerr<<"Unexpected character:"<<c<<endl;
                    }
                }
            }
        }

        set<int> sizes;
        for (size_t i=0;i<rows.size();i++) {
            //clog<<"row "<<i<<" size "<<rows[i].size()<<endl;
            sizes.insert(rows[i].size());
        }

        if (sizes.size()>1) {
            cerr<<"Rows are not same size"<<endl;
        }

        _height = rows.size();
        _width = *sizes.begin();
        clog<<"size "<<_width<<"x"<<_height<<endl;

        _data = new int[_width*_height];

        for (size_t j=0;j<rows.size();j++) {
            for (size_t i=0;i<rows[j].size();i++) {
                //clog<<"accesing row "<<j<<" "<<i<<endl;
                //clog<<rows[j][i]<<endl;
                _data[i+j*_width] = rows[j][i];
                //clog<<data[i+j*_width]<<endl;

            }
        }

    }

    file.close();

    int fx = _width;
    int fy = _height;
    int lx = 0;
    int ly = 0;

    for (int i=0;i<_width;i++) {
        for (int j=0;j<_height;j++) {
            int l = _data[i+j*_width];
            if (l>-1) {
                //clog<<"l "<<l<<" "<<i<<" "<<j<<endl;
                if (i<fx) {
                    fx=i;
                }
                if (j<fy) {
                    fy=j;
                }
                if (i>lx) {
                    lx=i;
                }
                if (j>ly) {
                    ly=j;
                }
            }
        }
    }

    clog<<"level size:"<<_width<<"x"<<_height<<endl;
    clog<<"level box:"<<fx<<"x"<<fy<<" -- "<<lx<<"x"<<ly<<endl;
}
/*
Level::Level(int width,int height,int seed) : _width(width),_height(height)
{
    _data = new int[_width*_height];

    for (size_t n=0;n<(_width*_height);n++) {
        _data[n] = -1;
    }

    int h = 64;
    int dy = 0;
    default_random_engine re;
    uniform_int_distribution<int> id(0,100);

    for (int x=0;x<_width;x++) {
        int rnd = id(re);

        if (rnd>50) {
            dy++;
        }
        else {
            dy--;
        }

        if (dy>4) {
            h++;
        }
        if (dy<0) {
            h--;
        }

        for (int y=0;y<_height;y++) {
            if (y>=h) {
                size_t center = x+y*_width;
                size_t top = x+(y-1)*_width;
                int tile = Rock;

                if (y==h) {
                    tile = Grass;

                    if (id(re) > 90) {
                        _data[top] = Watermelon;
                    }
                }

                if (y>h and y<(h+4)) {
                    tile = Dirt;
                }

                if (tile == Rock) {
                    if (id(re)>98) {
                        tile = RockCoal;
                    }

                }

                _data[center] = tile;
            }
        }
    }
}
*/

Level::Level(int width,int height,int seed) : _width(width),_height(height)
{

    default_random_engine re;
    uniform_int_distribution<int> id(0,100);

    lluna::set_seed(seed);

    _data = new int[_width*_height];

    for (size_t n=0;n<(_width*_height);n++) {
        _data[n] = -1;
    }

    for (int x=0;x<_width;x++) {
        int first_layer = 1 + id(re)/20;
        int first = 0;

        float fb = lluna::perlin(x,1,0.06f,2.0f);
        int biome = fb*10;
        //clog<<"biome "<<biome<<" "<<fb<<endl;

        int l1 = Grass;
        int l2 = Dirt;

        switch (biome) {
            case 0:
            case 1:
            case 2:
                l1 = Snow;
                l2 = Dirt;
                break;
            case 3:
                l1 = Water;
                l2 = Water;
                break;
            case 4:
                l1 = Sand;
                l2 = Sand;
                break;
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:

            default:
                break;
        }

        for (int y=6;y<(_height-1);y++) {

            float value = lluna::perlin(x,y,0.1f,2);
            //clog<<i<<" "<<j<<" "<<value<<endl;

            if (value>0.5) {
                if (first < first_layer) {
                    if (first == 0) {
                        put(x,y,l1);

                        if (biome == 2 and id(re)>80) {
                            put(x,y-1,Pumpking);
                        }

                        if (biome == 4 and id(re)>80) {
                            put(x,y-1,Cactus);
                            put(x,y-2,Cactus);
                            put(x,y-3,Cactus);
                        }

                        if (biome == 5 and id(re)>80) {
                            put(x,y-1,Log);
                            put(x,y-2,Log);
                            put(x,y-3,Log);

                            for (int tx=-1;tx<2;tx++) {
                                for (int ty=-1;ty<2;ty++) {
                                    put(x+tx,y-4+ty,Leafs);
                                }
                            }
                        }
                    }
                    else {
                        put(x,y,l2);
                    }
                    first++;
                }
                else {
                    put(x,y,Rock);
                }
            }
        }
    }

    for (int x=0;x<_width;x++) {
        size_t center = x+(_height-1)*_width;
        _data[center] = Bedrock;
    }
}

Level::~Level()
{
    delete [] _data;
}

void Level::save(const char* filename)
{
    fstream file;

    file.open(filename, std::fstream::out);

    for (int j=0;j<_height;j++) {
        for (int i=0;i<_width-1;i++) {
            file<<_data[i+j*_width]<<",";
        }
        file<<_data[_width-1+j*_width]<<"\n";

    }

    file.close();
}

int Level::get(int x,int y)
{
    if (_width>0 and _height>0 and x>-1 and y>-1 and x<(_width) and y<(_height)) {
        return _data[x+y*_width];
    }

    return -1;
}

void Level::put(int x,int y,int value)
{
    if (_width>0 and _height>0 and x>-1 and y>-1 and x<(_width) and y<(_height)) {
        _data[x+y*_width] = value;
    }
}
