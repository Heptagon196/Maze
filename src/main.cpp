#include "Conio+.h"
#include "Map.h"
#include <fstream>
#include <map>
#include <thread>

int Key_Up, Key_Down, Key_Left, Key_Right, Key_Quit;
BlockType Empty(BLACK, WHITE, "  ");
BlockType Wall(BLACK, BLUE, "  ");
BlockType Stair(BLACK, RED, "  ");
BlockType Player(BLACK, YELLOW, "  ");

bool dark=false;
void SaveMap(Map* m);
void Exit(Map *m = NULL) {
    unhidecursor();
    if (!dark)
        SaveMap(m);
    exit(0);
}

void ClearText() {
    double x=pro_time();
    while (pro_time()-x<10) ;
    gotoxy(1, 21);
    color(BLACK, WHITE);
    for (int i=0;i<80;i++)
        putchar(' ');
}

void DoNothing() {
    gotoxy(1, 21);
    color(BLACK, WHITE);
    for (int i=0;i<80;i++)
        putchar(' ');
}

map<int, function<void(Map *m)> > Keys;
void Config() {
    fstream fin;
    fin.open("maze.conf");
    fin >> Key_Up >> Key_Down >> Key_Left >> Key_Right >> Key_Quit;
    if (Key_Up==0 || Key_Down==0 || Key_Left==0 || Key_Right==0 || Key_Quit==0) {
        Key_Up='w';
        Key_Down='s';
        Key_Left='a';
        Key_Right='d';
        Key_Quit='q';
    }
    Keys[Key_Up]=[](Map *m){m->locy--;};
    Keys[Key_Down]=[](Map *m){m->locy++;};
    Keys[Key_Left]=[](Map *m){m->locx--;};
    Keys[Key_Right]=[](Map *m){m->locx++;};
    Keys[Key_Quit]=[](Map *m){Exit();};
    fin.close();
}

//Input
bool Kbhit(Map* m) {
    return kbhit();
}

void Read(Map* m) {
    int ch=getch();
    if (Keys.find(ch)!=Keys.end())
        Keys[ch](m);
}
//Output
int bakx=1, baky=1, bakz=0;
bool LocationChanged(Map* m) {
    if (m->locx!=bakx || m->locy!=baky) {
        if (m->locx < 1 || m->locx > 40 || m->locy < 1 || m->locy > 20 || m->locz < 0 || m->locz >= m->fl.size() ||
            (m->fl[m->locz].Get(m->locx, m->locy)==&Wall && dark)) {
            m->locx=bakx;
            m->locy=baky;
            m->locz=bakz;
            return false;
        }
        return true;
    }
    return false;
}

BlockType Dark(WHITE, BLACK, "  ");
map<int, map<int, map<int, function<void(Map*)>>>> events;
map<int, map<int, map<int, string>>> msg;
void Msg(Map *m);
void RefreshChar(Map *m) {
    if (events.find(m->locx)!=events.end() &&
        events[m->locx].find(m->locy)!=events[m->locx].end() &&
        events[m->locx][m->locy].find(m->locz)!=events[m->locx][m->locy].end())
        events[m->locx][m->locy][m->locz](m);
    if (msg.find(m->locx)!=msg.end() &&
        msg[m->locx].find(m->locy)!=msg[m->locx].end() &&
        msg[m->locx][m->locy].find(m->locz)!=msg[m->locx][m->locy].end())
        Msg(m);
    m->fl[m->locz].Show(bakx, baky);
    bakx=m->locx;
    baky=m->locy;
    bakz=m->locz;
    if (dark)
        for (int i=bakx-3;i<=bakx+3;i++)
            for (int j=baky-3;j<=baky+3;j++) {
                if (i<1 || i>40 || j<1 || j>20)
                    continue;
                if (i==bakx-3 || i==bakx+3 || j==baky-3 || j==baky+3) {
                    gotoxy(i, j);
                    Dark.Show();
                }
                else
                    m->fl[m->locz].Show(i, j);
            }
    gotoxy(m->locx, m->locy);
    Player.Show();
}
void Msg(Map *m) {
    gotoxy(1, 21);
    color(BLACK, WHITE);
    for (int i=0;i<80;i++)
        putchar(' ');
    gotoxy(1, 21);
    cout << msg[m->locx][m->locy][m->locz];
}

struct Door {
    int x, y, z;
    Door(int x, int y, int z):x(x), y(y), z(z){}
    Door():x(0), y(0), z(0) {}
    bool operator != (Door d) {
        return x!=d.x || y!=d.y || z!=d.z;
    }
};
map<int, map<int, map<int, Door>>> door;
void UseKey(Map *m) {
    Door d=door[m->locx][m->locy][m->locz];
    if (m->fl[d.z].Get(d.x, d.y)==&Empty)
        m->fl[d.z].Set(d.x, d.y, &Wall);
    else
        m->fl[d.z].Set(d.x, d.y, &Empty);
    if (d.z==m->locz)
        m->fl[d.z].Show(d.x, d.y);
}

map<int, map<int, map<int, int>>> exits;
void ReadMap(Map* m) {
    fstream fin;
    string com, para;
    int x, y;
    fin.open("maze.map");
    while (fin >> com) {
        if (com=="Wall") {
            fin >> x >> y;
            m->fl[0].Set(x, y, &Wall);
        } else if (com=="Exit") {
            fin >> x >> y;
            events[x][y][0]=Exit;
            exits[x][y][0]=1;
        } else if (com=="Msg") {
            fin >> x >> y;
            getline(fin, para, '\n');
            msg[x][y][0]=para;
        } else if (com=="Key") {
            int x2, y2;
            fin >> x >> y >> x2 >> y2;
            door[x][y][0]=Door(x2, y2, 0);
            events[x][y][0]=UseKey;
        }
    }
    fin.close();
}

Map* bakup;
void SaveMap(Map* m) {
    m=bakup;
    fstream fout;
    fout.open("maze.map");
    for (int i=1;i<=20;i++)
        for (int j=1;j<=40;j++) {
            if (m->fl[0].Get(i, j)==&Wall)
                fout << "Wall " << i << ' ' << j << endl;
            if (exits[i][j][0])
                fout << "Exit " << i << ' ' << j << endl;
            if (msg[i][j][0]!="")
                fout << "Msg " << i << ' ' << j << ' ' << msg[i][j][0] << endl;
            if (door[i][j][0]!=Door(0, 0, 0))
                fout << "Key " << i << ' ' << j << ' ' << door[i][j][0].x << ' ' << door[i][j][0].y << endl;
        }
    fout.close();
}

int main(int argc, char **argv) {
#ifndef linux
    system("mode con lines=40");
    system("mode con cols=90");
#endif
    dark=(argc==1);
    gotoxy(1, 1);
    hidecursor();
    Map m;
    bakup=&m;
    Config();
    m.locx=m.locy=1;
    m.locz=0;
    m.fl.push_back(Floor(&Empty));
    ReadMap(&m);
    if (dark)
        m.fl[0].DarkInit();
    else
        m.fl[0].Init();
    RefreshChar(&m);
    m.Connect(Kbhit, Read);
    m.Connect(LocationChanged, RefreshChar);
    if (dark)
        m.Exec();
    else {
        Keys['r']=[](Map* m) {
            gotoxy(1, 21);
            printf("BlockType(0: empty 1: wall 2: leave message 3: door control): ");
            int num;
            scanf("%d", &num);
            if (num==0)
                m->fl[m->locz].Set(m->locx, m->locy, &Empty);
            else if (num==1)
                m->fl[m->locz].Set(m->locx, m->locy, &Wall);
            else if (num==2) {
                gotoxy(1, 21);
                for (int i=0;i<80;i++)
                    putchar(' ');
                gotoxy(1, 21);
                printf("Input a string: ");
                string s;
                cin >> s;
                msg[m->locx][m->locy][m->locz]=s;
            } else if (num==3) {
                gotoxy(1, 21);
                for (int i=0;i<80;i++)
                    putchar(' ');
                gotoxy(1, 21);
                printf("Input the location: ");
                int x, y;
                cin >> x >> y;
                door[m->locx][m->locy][m->locz]=Door(x, y, 0);
                events[m->locx][m->locy][m->locz]=UseKey;
            }
            m->fl[m->locz].Show(m->locx, m->locy);
        };
        m.Exec();
    }
    unhidecursor();
    return 0;
}
