// SPDX-FileCopyrightText: 2022 Enrique M.G. <quiqueiii@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "level.hpp"

#include <iostream>

using namespace lluna;

using namespace std;

int main (int argc,char* argv[])
{
    clog<<"Welcome to lluna!"<<endl;

    Level level(argv[1]);

    clog<<"\nlevel:"<<argv[1]<<endl;
    clog<<level.width()<<"x"<<level.height()<<endl;
    return 0;
}
