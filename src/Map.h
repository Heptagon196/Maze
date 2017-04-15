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
        BlockType(int fg, int bg, string text): fg(fg), bg(bg), text(text) {}
        void Show();
        void Rever();
};

class Floor {
    private:
        BlockType* Map[45][45];
    public:
        void Init();
        void DarkInit();
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

        vector<pair<function<bool(Map*)>, function<void(Map*)> > > p;
        void Connect(function<bool(Map*)> check, function<void(Map*)> exec);
        Floor& operator [](int i) {
            return fl[i];
        }
        void Exec();
};
#endif
