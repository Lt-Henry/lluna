
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
