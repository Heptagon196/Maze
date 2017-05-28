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
#include "Bag.h"

Map Main;
Interpreter exec;
map<int, function<void(Map *m)> > Keys;
typedef string SingleEvent[45][45];
map<int, SingleEvent> Events;
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
    char ch;
    gotoxy(1, 21);
    color(BLACK, WHITE);
    puts("Are you sure you want to exit? [Y/n]");
    ch=getch();
    if (ch=='Y' || ch=='y') {
        clearcolor();
        clear();
        unhidecursor();
        exit(0);
    } else {
        gotoxy(1, 21);
        color(BLACK, WHITE);
        for (int i=0;i<80;i++)
            putchar(' ');
    }
}

BlockType Dark(BLACK, BLACK, "  ");
void LocationMoved(int bakx, int baky, Map *m) {
    string tmp;
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
            if (bakx>=m->locx-2 && bakx<=m->locx+2 && baky>=m->locy-2 && baky<=m->locy+2)
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
            if (bakx>=m->locx-2 && bakx<=m->locx+2 && baky>=m->locy-2 && baky<=m->locy+2)
                m->fl[m->locz].Show(bakx, baky);
            for (int i=min(bakx, m->locx)-horizon;i<=max(bakx, m->locx)+horizon;i++)
                for (int j=min(baky, m->locy)-horizon;j<=max(baky, m->locy)+horizon;j++)
                    if (!(i<1||i>40||j<1||j>20)) {
                        if ( (i>=m->locx-horizon && i<=m->locx+horizon && j>=m->locy-horizon && j<=m->locy+horizon) &&
                            ((! (i>=bakx-horizon && i<=bakx+horizon && j>=baky-horizon && j<=baky+horizon))) ) {
                            m->fl[m->locz].Show(i, j);
                        } else if (! (i>=m->locx-horizon && i<=m->locx+horizon && j>=m->locy-horizon && j<=m->locy+horizon)){
                            gotoxy(i, j);
                            Dark.Show();
                        }
                    }
            gotoxy(m->locx, m->locy);
            Player.Show();
        }
        if ((tmp=Events[m->locz][m->locx][m->locy])!="") {
            stringstream ss;
            ss.str("");
            ss << tmp;
            exec.Exec(ss);
        }
    }
}

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
    if (m->locx!=bakx || m->locy!=baky)
        LocationMoved(bakx, baky, m);
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
            Main.fl[Main.locz].Set(x, y, BlockArray[i]);
    fin.close();
    return Empty;
}

bool isNumber(string s) {
    for (int i=0;i<s.length();i++)
        if (!isdigit(s[i]))
            return false;
    return true;
}

Def(AddEvent) {
    int a, b, c;
    GetInt(0, a);
    GetInt(1, b);
    int i=2;
    if (isNumber(para[2].second) || (p->type.find(para[2].second)!=p->type.end() && p->type[para[2].second]==INTE)) {
        GetInt(2, c);
        i++;
    } else
        c=Main.locz;
    string &s=Events[c][a][b];
    s="(main ";
    for (;i<para.size();i++)
        s+=para[i].second+" ";
    s+=")";
    return Empty;
}

Def(SetBlock) {
    if (para.size()==3) {
        int a, b, c;
        GetInt(0, c);
        GetInt(1, a);
        GetInt(2, b);
        if (c<BlockArray.size())
            Main.fl[Main.locz].Set(a, b, BlockArray[c]);
        Main.fl[Main.locz].Show(a, b);
        return Empty;
    } else
        return Source(p, para);
}

Def(GetBlock) {
    int a, b;
    GetInt(0, a);
    GetInt(1, b);
    BlockType* t=Main.fl[Main.locz].Get(a, b);
    return ParaList(1, Pair(INTE, Transfer(t->id)));
}

Def(Crossable) {
    int a, b;
    GetInt(0, a);
    GetInt(1, b);
    BlockType* t=Main.fl[Main.locz].Get(a, b);
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

Def(GetZ) {
    return ParaList(1, Pair(INTE, Transfer(Main.locz)));
}

Def(ChangeMap) {
    string s;
    GetStr(0, s);
    MapName=s;
    return Empty;
}

Def(Refresh) {
    gotoxy(1, 1);
    Main.fl[Main.locz].Init();
    gotoxy(Main.locx, Main.locy);
    Player.Show();
    return Empty;
}

Def(SetLocation) {
    int x=Main.locx, y=Main.locy;
    GetInt(0, Main.locx);
    GetInt(1, Main.locy);
    if (para.size()==3) {
        GetInt(2, Main.locz);
        gotoxy(1, 1);
        Main.fl[Main.locz].Init();
    }
    LocationMoved(x, y, &Main);
    return Empty;
}

Def(ResetMap) {
    memset(Main.fl[Main.locz].explored, 0, sizeof(Main.fl[Main.locz].explored));
    Main.fl[Main.locz]=Floor(&Null);
    if (print == PRINT_EXPLORE || print == PRINT_TORCH)
        for (int i=1;i<=horizon+1;i++)
            for (int j=1;j<=horizon+1;j++)
                Main.fl[Main.locz].explored[i][j]=1;
    else if (print == PRINT_OVERVIEW)
        for (int i=1;i<=40;i++)
            for (int j=1;j<=20;j++)
                Main.fl[Main.locz].explored[i][j]=1;
    return Empty;
}

Def(AddFloor) {
    Main.fl.push_back(Floor(&Null));
    Main.locz++;
    if (print == PRINT_EXPLORE || print == PRINT_TORCH)
        for (int i=1;i<=horizon+1;i++)
            for (int j=1;j<=horizon+1;j++)
                Main.fl[Main.locz].explored[i][j]=1;
    else if (print == PRINT_OVERVIEW)
        for (int i=1;i<=40;i++)
            for (int j=1;j<=20;j++)
                Main.fl[Main.locz].explored[i][j]=1;
    return Empty;
}

Def(SetMethod) {
    int tmp;
    GetInt(0, tmp);
    print=tmp;
    return Empty;
}

Def(SetHorizon) {
    int tmp;
    GetInt(0, tmp);
    horizon=tmp;
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
    ImportBag(exec);
    exec.AddVar("Empty", 0);
    exec.AddVar("Wall", 1);
    exec.AddVar("Stair", 2);
    exec.Add("map.get", GetBlock);
    exec.Add("map.set", SetBlock);
    exec.Add("map.def", DefBlock);
    exec.Add("map.can-cross", Crossable);
    exec.Add("map.x", GetX);
    exec.Add("map.y", GetY);
    exec.Add("map.z", GetZ);
    exec.Add("map.change", ChangeMap);
    exec.Add("map.refresh", Refresh);
    exec.Add("map.reset", ResetMap);
    exec.Add("map.setloc", SetLocation);
    exec.Add("map.horizon", SetHorizon);
    exec.Add("map.level", SetMethod);
    exec.Add("floor.add", AddFloor);
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
        config.type["MapName"]=STR;
        config.Exec(fin);
        if (config.Str["MapName"]!="")
            MapName=config.Str["MapName"];
    }
    fin.close();
    Init();
    fin.open("maps/"+MapName+"/main.lsp");
    if (fin)
        exec.Exec(fin);
    fin.close();
    if (print == PRINT_EXPLORE || print == PRINT_TORCH)
        for (int i=1;i<=horizon+1;i++)
            for (int j=1;j<=horizon+1;j++)
                Main.fl[Main.locz].explored[i][j]=1;
    else if (print == PRINT_OVERVIEW)
        for (int i=1;i<=40;i++)
            for (int j=1;j<=20;j++)
                Main.fl[Main.locz].explored[i][j]=1;
    gotoxy(1, 1);
    Main.fl[Main.locz].Init();
    gotoxy(1, 1);
    Player.Show();
    Main.locx=Main.locy=1;
    Main.Exec();
    return 0;
}
