#include "RPG.h"

int main(int argc, char* argv[]) {
    srand(time(NULL));
#ifndef linux
    system("mode con lines=40");
    system("mode con cols=81");
#endif
    vector<string> args;
    for (int i = 0; i < argc; i ++) {
        args.push_back((string)argv[i]);
    }
    return RPGStart(args);
}
