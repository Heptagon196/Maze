#include "Map.h"
//BlockType

BlockType::BlockType() {}
BlockType::BlockType(int fg, int bg, string text): fg(fg), bg(bg), text(text) {}
BlockType::BlockType(int fg, int bg, string text, int crossable): fg(fg), bg(bg), text(text), crossable(crossable) {}
BlockType::BlockType(int fg, int bg, string text, int crossable, int id): fg(fg), bg(bg), text(text), crossable(crossable), id(id) {}

void BlockType::Show() const {
    color(fg, bg);
    cout << text;
}

void BlockType::Rever() const {
    color(fg, bg);
    cout << text;
}

//Floor
Floor::Floor(BlockType* bt) {
    memset(explored, 0, sizeof(explored));
    for (int i = 0; i <= 40; i ++)  {
        for (int j = 0; j <= 40; j ++) {
            Map[i][j] = bt;
        }
    }
}

void Floor::Init() {
    for (int i = 1; i <= 20; i ++) {
        for (int j = 1; j <= 40; j ++) {
            if (explored[j][i]) {
                Map[j][i]->Show();
            } else {
                Dark.Show();
            }
        }
        putchar('\n');
    }
}

void Floor::Set(int x, int y, BlockType* bt) {
    Map[x][y] = bt;
}

BlockType* Floor::Get(int x, int y) {
    return Map[x][y];
}

void Floor::Show(int x, int y) {
    gotoxy(x, y);
    Map[x][y]->Show();
}

void Floor::Rever(int x, int y) {
    gotoxy(x, y);
    Map[x][y]->Rever();
}

//Map
void Map::Add(function<void()> func) {
    p.push_back(func);
}

void Map::Exec() {
    while (true) {
        for (int i = 0; i < p.size(); i ++) {
            p[i]();
        }
        framecnt ++;
        gotoxy(1, 23);
        Sleep(timePerFrame);
        gotoxy(1, 23);
        color(BLACK, WHITE);
        cout << framecnt << endl;
        //puts(" ");
    }
}

Floor& Map::operator [] (int i) {
    return fl[i];
}

Floor& Map::cur() {
    return fl[locz];
}
