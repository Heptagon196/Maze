#ifndef MAZEMAP_H
#define MAZEMAP_H
#include "Conio+.h"
#include <vector>
#include <functional>
class BlockType {
    private:
        int fg, bg;
        string text;
    public:
        int crossable = 1;
        int id;
        BlockType() {}
        BlockType(int fg, int bg, string text): fg(fg), bg(bg), text(text) {}
        BlockType(int fg, int bg, string text, int crossable): fg(fg), bg(bg), text(text), crossable(crossable) {}
        BlockType(int fg, int bg, string text, int crossable, int id): fg(fg), bg(bg), text(text), crossable(crossable), id(id) {}
        void Show();
        void Rever();
};
static BlockType Dark(BLACK, BLACK, "  ");

class Floor {
    private:
        BlockType* Map[45][45];
    public:
        bool explored[45][45];
        void Init();
        Floor(BlockType* bt);
        void Set(int x, int y, BlockType* bt);
        BlockType* Get(int x, int y);
        void Show(int x, int y);
        void Rever(int x, int y);
};

class Map {
    public:
        vector<Floor> fl;
        int locx, locy, locz;

        vector<function<void(Map*)> > p;
        void Add(function<void(Map*)> func);
        Floor& operator [](int i) {
            return fl[i];
        }
        void Exec();
};
#endif
