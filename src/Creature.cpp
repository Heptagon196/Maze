#include "Creature.h"
#include <cstdio>

vector<Creature> c;
void Creature::Show() {
    color(fg, bg);
    gotoxy(x, y);
    cout << text << endl;
}

Creature::Creature(int fg, int bg, string text, ActFunc Action)
    : fg(fg), bg(bg), text(text), Action(Action), x(1), y(1) {}

void CreatureMove(Map *m) {
    BlockType Player(BLACK, YELLOW, "  ");
    for (int i=0;i<c.size();i++) {
        int bakx=c[i].x,
            baky=c[i].y;
        c[i].Action(c[i].x, c[i].y);
        if (c[i].x !=bakx || c[i].y != baky) {
            if (m->fl[m->locz].explored[bakx][baky])
                m->fl[m->locz].Show(bakx, baky);
            else {
                gotoxy(bakx, baky);
                Dark.Show();
            }
            if (bakx == m->locx && baky == m->locy) {
                gotoxy(bakx, baky);
                Player.Show();
            }
            c[i].Show();
        }
    }
}

void AddCreature(int fg, int bg, string text, function<void(int&, int&)> Action) {
    c.push_back(Creature(fg, bg, text, Action));
}

void PopCreature() {
    if (c.size())
        c.pop_back();
}
