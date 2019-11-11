#ifndef MAZEMAP_H
#define MAZEMAP_H
#include <vector>
#include <functional>
#include <iostream>
#include <cstring>
#if defined(linux) || defined(__APPLE__)
#include <unistd.h>
#define Sleep(x) usleep(x * 1000)
#else
#include <windows.h>
#endif
#include "ConioPlus.h"
#include "Gos.h"
using namespace std;

class BlockType {
    private:
        int fg, bg;
        string text;
    public:
        int crossable = 1;
        int id;
        BlockType();
        BlockType(int fg, int bg, string text);
        BlockType(int fg, int bg, string text, int crossable);
        BlockType(int fg, int bg, string text, int crossable, int id);
        void Show() const;
        void Rever() const;
};

static const BlockType Dark(BLACK, BLACK, "  ");

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

        int timePerFrame = 20;
        unsigned long long framecnt = 0;

        Floor& operator [] (int i);
        Floor& cur();

        vector<function<void()> > p;
        void Add(function<void()> func);
        void Exec();
};
#endif
