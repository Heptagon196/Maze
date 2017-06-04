#ifndef MAZE_CREATURE_H
#define MAZE_CREATURE_H
#include "Map.h"
typedef function<void(int&, int&)> ActFunc;
class Creature {
    public:
        int x, y;
        int fg, bg;
        string text;
        ActFunc Action;
        void Show();
        Creature(int fg, int bg, string text, ActFunc Action);
};

void CreatureMove(Map *m);
void AddCreature(int fg, int bg, string text, ActFunc Action);
void PopCreature();
#endif
