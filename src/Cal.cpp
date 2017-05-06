#include <map>
#include <functional>
#include <string>
#include <cstring>
#include <iostream>
#include "Interpreter.h"
#include "Conio+.h"
#include "Map.h"
#include "Cal.h"
#include "InterpreterExt.h"
#if defined(linux) || defined(__APPLE__)
#else
void chdir(const char *str) {
    system(("cd "+(string)str).c_str());
}
#endif

Map Main;
Interpreter exec;
map<int, function<void(Map *m)> > Keys;
map<int, map<int, string> > Events;
BlockType Null(BLACK, WHITE, "  ");
BlockType Wall(BLACK, BLUE, "  ");
BlockType Stair(BLACK, RED, "  ");
BlockType Player(BLACK, YELLOW, "  ");

void Exit(Map *m = NULL) {
    m=&Main;
    unhidecursor();
    clearcolor();
    clear();
    puts("\nPress any key to exit...");
    getch();
    exit(0);
}

void KeyPress(Map *m) {
    if (!kbhit())
        return ;
    char ch=getch();
    int bakx=m->locx, baky=m->locy;
    if (Keys.find(ch)!=Keys.end())
        Keys[ch](m);
    if (m->locx!=bakx || m->locy!=baky) {
        if (m->locx < 1 || m->locx > 40 || m->locy < 1 || m->locy > 20 || m->locz < 0 || m->locz >= m->fl.size() ||
            (m->fl[m->locz].Get(m->locx, m->locy)==&Wall)) {
            m->locx=bakx;
            m->locy=baky;
        }
        if (m->locx!=bakx || m->locy!=baky) {
            m->fl[m->locz].Show(bakx, baky);
            gotoxy(m->locx, m->locy);
            Player.Show();
            string tmp;
            if ((tmp=Events[m->locx][m->locy])!="") {
                stringstream ss;
                ss.str("");
                ss << tmp;
                exec.Exec(ss);
            }
        }
    }
}

#define GetStr(x, y) if (para[x].first==REFER) y=p->Str[para[x].second]; else y=para[x].second.substr(1, para[x].second.length()-2);
#define GetInt(x, y) if (para[x].first==REFER) y=p->Int[para[x].second]; else y=Transfer(para[x].second);

Def(Source) {
    string t;
    GetStr(0, t);
    ifstream fin;
    string com;
    int x, y;
    fin.open(t);
    if (fin)
        while (fin >> x >> y)
            Main.fl[0].Set(x, y, &Wall);
    fin.close();
    return Empty;
}

Def(AddEvent) {
    int a, b;
    GetInt(0, a);
    GetInt(1, b);
    string &s=Events[a][b];
    s="(main ";
    for (int i=2;i<para.size();i++)
        s+=para[i].second+" ";
    s+=")";
    return Empty;
}
BlockType* BlockArray[]={&Null, &Wall};
#define BLOCK_EMPTY 0
#define BLOCK_WALL 1

Def(SetBlock) {
    int a, b, c;
    GetInt(0, a);
    GetInt(1, b);
    GetInt(2, c);
    if (c<2)
        Main.fl[0].Set(a, b, BlockArray[c]);
    Main.fl[0].Show(a, b);
    return Empty;
}

Def(GetBlock) {
    int a, b;
    GetInt(0, a);
    GetInt(1, b);
    BlockType* t=Main.fl[0].Get(a, b);
    return ParaList(1, Pair(INTE, (t==&Wall)?"1":"0"));
}

void Init() {
    Main.fl.push_back(Floor(&Null));
    Keys['w']=[](Map *m){m->locy--;};
    Keys['s']=[](Map *m){m->locy++;};
    Keys['a']=[](Map *m){m->locx--;};
    Keys['d']=[](Map *m){m->locx++;};
    Keys['q']=[](Map *m){Exit();};

    Main.Add(KeyPress);

    ImportExt(exec);
    exec.AddVar("Empty", BLOCK_EMPTY);
    exec.AddVar("Wall", BLOCK_WALL);
    exec.Add("source", Source);
    exec.Add("get", GetBlock);
    exec.Add("set", SetBlock);
    exec.AddPreserved("event", AddEvent);
}

int Cal(int argc, char **argv) {
    hidecursor();
    ifstream fin;
    string MapName;
    fin.open("maze.config");
    fin >> MapName;
    fin.close();
    Init();
    fin.open("maps/"+MapName+"/maze.main");
    if (fin) {
        chdir(("maps/"+MapName).c_str());
        exec.Exec(fin);
        chdir("../../");
    }
    fin.close();
    gotoxy(1, 1);
    Main.fl[0].Init();
    gotoxy(1, 1);
    Player.Show();
    Main.locx=Main.locy=1;
    Main.Exec();
    return 0;
}
