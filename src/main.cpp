#include "RPG.h"

int main(int argc, char* argv[]) {
    srand(time(NULL));
#if defined(linux) || defined(__APPLE__)
#else
    system("mode con lines=40");
    system("mode con cols=81");
#endif
    vector<Any> args;
    for (int i = 0; i < argc; i ++) {
        args.push_back((string)argv[i]);
    }
    return RPGStart(args);
}
