#include <iostream>
#include "Conio+.h"
#include "Map.h"
//BlockType
void BlockType::Show() {
    color(fg, bg);
    cout << text;
}

void BlockType::Rever() {
    color(fg, bg);
    cout << text;
}

//Floor
Floor::Floor(BlockType* bt) {
    for (int i=0;i<=40;i++)
        for (int j=0;j<=40;j++)
            Map[i][j]=bt;
}

void Floor::Init() {
    for (int i=1;i<=20;i++) {
        for (int j=1;j<=40;j++)
            Map[j][i]->Show();
        putchar('\n');
    }
}

void Floor::DarkInit() {
    BlockType Dark(BLACK, BLACK, "  ");
    for (int i=1;i<=20;i++) {
        for (int j=1;j<=40;j++)
            if (i<=3&&j<=3)
                Map[j][i]->Show();
            else
                Dark.Show();
        putchar('\n');
    }
}

void Floor::Set(int x, int y, BlockType* bt) {
    Map[x][y]=bt;
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
void Map::Connect(function<bool(Map*)> check, function<void(Map*)> exec) {
    p.push_back(make_pair(check, exec));
}

void Map::Exec() {
    while (true)
        for (int i=0;i<p.size();i++)
            if (p[i].first(this))
                p[i].second(this);
}
