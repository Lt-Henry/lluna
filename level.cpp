// SPDX-FileCopyrightText: 2022 Enrique M.G. <quiqueiii@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "level.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <set>

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

Level::Level(int width,int height,int seed) : _width(width),_height(height)
{
    _data = new int[_width*_height];

    for (int x=0;x<_width;x++) {
        for (int y=0;y<_height;y++) {
            //TODO
        }
    }
}

Level::~Level()
{
    delete [] _data;
}

int Level::get(int x,int y)
{
    if (_width>0 and _height>0 and x>-1 and y>-1 and x<(_width+1) and y<(_height+1)) {
        return _data[x+y*_width];
    }

    return -1;
}
