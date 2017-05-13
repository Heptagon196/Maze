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

Map Main;
Interpreter exec;
map<int, function<void(Map *m)> > Keys;
map<int, map<int, string> > Events;
BlockType Null(BLACK, WHITE, "  ", true, 0);
BlockType Wall(BLACK, BLUE, "  ", false, 1);
BlockType Stair(BLACK, RED, "  ", true, 2);
BlockType Player(BLACK, YELLOW, "  ");
map<string, BlockType> UserType;
string MapName;
vector<BlockType*> BlockArray={&Null, &Wall, &Stair};
map<int, string> UserKeys;

int print, horizon=2;
// 0 for Overview mode
// 1 for Explore mode
// 2 for Torch mode
#define PRINT_OVERVIEW 0
#define PRINT_EXPLORE 1
#define PRINT_TORCH 2

void Exit(Map *m = NULL) {
    m=&Main;
    unhidecursor();
    clearcolor();
    clear();
    puts("\nPress any key to exit...");
    getch();
    exit(0);
}

BlockType Dark(BLACK, BLACK, "  ");
void KeyPress(Map *m) {
    if (!kbhit())
        return ;
    char ch=getch();
    int bakx=m->locx, baky=m->locy;
    string tmp;
    if (Keys.find(ch)!=Keys.end())
        Keys[ch](m);
    if ((tmp=UserKeys[ch])!="") {
        stringstream ss;
        ss.str("");
        ss << tmp;
        exec.Exec(ss);
    }
    if (m->locx!=bakx || m->locy!=baky) {
        if (m->locx < 1 || m->locx > 40 || m->locy < 1 || m->locy > 20 || m->locz < 0 || m->locz >= m->fl.size() ||
            (!m->fl[m->locz].Get(m->locx, m->locy)->crossable)) {
            m->locx=bakx;
            m->locy=baky;
        }
        if (m->locx!=bakx || m->locy!=baky) {
            if (print == PRINT_OVERVIEW) {
                m->fl[m->locz].Show(bakx, baky);
                gotoxy(m->locx, m->locy);
                Player.Show();
            } else if (print == PRINT_EXPLORE) {
                m->fl[m->locz].Show(bakx, baky);
                gotoxy(m->locx, m->locy);
                for (int i=m->locx-horizon;i<=m->locx+horizon;i++)
                    for (int j=m->locy-horizon;j<=m->locy+horizon;j++)
                        if (!(i<1||i>40||j<1||j>20))
                            if (!m->fl[m->locz].explored[i][j]) {
                                m->fl[m->locz].explored[i][j]=1;
                                m->fl[m->locz].Show(i, j);
                            }
                gotoxy(m->locx, m->locy);
                Player.Show();
            } else if (print == PRINT_TORCH) {
                m->fl[m->locz].Show(bakx, baky);
                for (int i=min(bakx, m->locx)-horizon;i<=max(bakx, m->locx)+horizon;i++)
                    for (int j=min(baky, m->locy)-horizon;j<=max(baky, m->locy)+horizon;j++)
                        if (!(i<1||i>40||j<1||j>20)) {
                            if ( (i>=m->locx-horizon && i<=m->locx+horizon && (! (i>=bakx-horizon && i<=bakx+horizon))) ||
                                (j>=m->locy-horizon && j<=m->locy+horizon && (! (j>=baky-horizon && j<=baky+horizon))) )
                                m->fl[m->locz].Show(i, j);
                            else if (! (i>=m->locx-horizon && i<=m->locx+horizon && j>=m->locy-horizon && j<=m->locy+horizon)){
                                gotoxy(i, j);
                                Dark.Show();
                            }
                        }
                gotoxy(m->locx, m->locy);
                Player.Show();
            }
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
    int i;
    string t;
    GetInt(0, i);
    GetStr(1, t);
    ifstream fin;
    string com;
    int x, y;
    if (t[0]!='.')
        fin.open("maps/"+MapName+"/"+t);
    else
        fin.open(t);
    if (fin)
        while (fin >> x >> y)
            Main.fl[0].Set(x, y, BlockArray[i]);
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

Def(SetBlock) {
    if (para.size()==3) {
        int a, b, c;
        GetInt(0, a);
        GetInt(1, b);
        GetInt(2, c);
        if (c<BlockArray.size())
            Main.fl[0].Set(a, b, BlockArray[c]);
        Main.fl[0].Show(a, b);
        return Empty;
    } else
        return Source(p, para);
}

Def(GetBlock) {
    int a, b;
    GetInt(0, a);
    GetInt(1, b);
    BlockType* t=Main.fl[0].Get(a, b);
    return ParaList(1, Pair(INTE, Transfer(t->id)));
}

Def(Crossable) {
    int a, b;
    GetInt(0, a);
    GetInt(1, b);
    BlockType* t=Main.fl[0].Get(a, b);
    return ParaList(1, Pair(INTE, Transfer(t->crossable)));
}

Def(DefBlock) {
    int fg, bg, cross;
    string ch, name;
    GetInt(1, fg);
    GetInt(2, bg);
    GetStr(3, ch);
    GetInt(4, cross);
    exec.AddVar(para[0].second, BlockArray.size());
    UserType[name]=BlockType(fg, bg, ch, cross, BlockArray.size());
    BlockArray.push_back(&UserType[name]);
    return Empty;
}

Def(AddKey) {
    string ch;
    GetStr(0, ch);
    string tar="(main ";
    for (int i=1;i<para.size();i++)
        tar+=para[i].second+" ";
    tar+=")";
    for (int i=0;i<ch.length();i++)
        UserKeys[ch[i]]=tar;
    return Empty;
}

Def(GetX) {
    return ParaList(1, Pair(INTE, Transfer(Main.locx)));
}

Def(GetY) {
    return ParaList(1, Pair(INTE, Transfer(Main.locy)));
}

Def(ChangeMap) {
    string s;
    GetStr(0, s);
    MapName=s;
    return Empty;
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
    exec.AddVar("Empty", 0);
    exec.AddVar("Wall", 1);
    exec.AddVar("Stair", 2);
    exec.Add("get", GetBlock);
    exec.Add("set", SetBlock);
    exec.Add("def", DefBlock);
    exec.Add("can-cross", Crossable);
    exec.Add("locx", GetX);
    exec.Add("locy", GetY);
    exec.Add("changemap", ChangeMap);
    exec.AddPreserved("event", AddEvent);
    exec.AddPreserved("key", AddKey);
    exec.AddVar("method", -32000);
    exec.AddVar("horizon", -32000);
}

int Cal(int argc, char **argv) {
    hidecursor();
    ifstream fin;
    fin.open("config.lsp");
    MapName="Default";
    if (fin) {
        Interpreter config;
        ImportExt(config);
        config.AddVar("method", -32000);
        config.AddVar("horizon", -32000);
        config.type["MapName"]=STR;
        config.Exec(fin);
        if (config.Str["MapName"]!="")
            MapName=config.Str["MapName"];
        if (config.Int["method"]!=-32000)
            print=config.Int["method"];
        if (config.Int["horizon"]!=-32000)
            horizon=config.Int["horizon"];
    }
    fin.close();
    Init();
    fin.open("maps/"+MapName+"/main.lsp");
    if (fin)
        exec.Exec(fin);
    fin.close();
    if (exec.Int["method"]!=-32000)
        print=exec.Int["method"];
    if (exec.Int["horizon"]!=-32000)
        horizon=exec.Int["horizon"];
    if (print == PRINT_EXPLORE || print == PRINT_TORCH)
        for (int i=1;i<=horizon+1;i++)
            for (int j=1;j<=horizon+1;j++)
                Main.fl[0].explored[i][j]=1;
    else if (print == PRINT_OVERVIEW)
        for (int i=1;i<=20;i++)
            for (int j=1;j<=40;j++)
                Main.fl[0].explored[i][j]=1;
    gotoxy(1, 1);
    Main.fl[0].Init();
    gotoxy(1, 1);
    Player.Show();
    Main.locx=Main.locy=1;
    Main.Exec();
    return 0;
}
