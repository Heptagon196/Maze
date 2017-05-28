#include "Conio+.h"
#include "Map.h"
#include "Cal.h"
#include <fstream>
#include <map>

int main(int argc, char **argv) {
#ifndef linux
    system("mode con lines=40");
    system("mode con cols=80");
#endif
    return Cal(argc, argv);
}
