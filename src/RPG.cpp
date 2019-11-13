#include "RPG.h"

Map Main;
map<int, function<void()>> Keys;
map<int, map<int, map<int, GosFunction>>> Events;
BlockType Null(BLACK, WHITE, "  ", true, 0);
BlockType Wall(BLACK, BLUE, "  ", false, 1);
BlockType Stair(BLACK, RED, "  ", true, 2);
BlockType Player(BLACK, YELLOW, "  ");
map<int, BlockType> UserType;
string MapName;
vector<BlockType*> BlockArray = {&Null, &Wall, &Stair};
map<int, GosFunction> UserKeys;
GosFunction exec;

// 0 for Overview mode
// 1 for Explore mode
// 2 for Torch mode
const int print_overview = 0;
const int print_explore = 1;
const int print_torch = 2;

int print, horizon = 2;

void Exit() {
    gotoxy(1, 21);
    color(BLACK, WHITE);
    printf("Are you sure you want to exit? [Y/n]");
    for (int i = 0; i < 240; i ++) {
        putchar(' ');
    }
    char ch = getch();
    if (ch == 'Y' || ch == 'y') {
        gotoxy(1, 1);
        clearcolor();
        puts("");
        clearscreen();
        showcursor();
        exit(0);
    } else {
        gotoxy(1, 21);
        color(BLACK, WHITE);
        for (int i = 0; i < 80; i ++) {
            putchar(' ');
        }
    }
}

void LocationMoved(int bakx, int baky) {
    string tmp;
    if (Main.locx < 1 || Main.locx > 40 || Main.locy < 1 || Main.locy > 20 || (!Main.cur().Get(Main.locx, Main.locy)->crossable)) {
        Main.locx = bakx;
        Main.locy = baky;
    }
    if (Main.locx != bakx || Main.locy != baky) {
        if(print == print_overview) {
            Main.cur().Show(bakx, baky);
            gotoxy(Main.locx, Main.locy);
            Player.Show();
        } else if (print == print_explore) {
            if (bakx >= Main.locx - horizon && bakx <= Main.locx + horizon && baky >= Main.locy - horizon && baky <= Main.locy + horizon) {
                Main.cur().Show(bakx, baky);
            }
            gotoxy(Main.locx, Main.locy);
            for (int i = Main.locx - horizon; i <= Main.locx + horizon; i ++) {
                for (int j = Main.locy - horizon; j <= Main.locy + horizon; j ++) {
                    if (!(i < 1 || i > 40 || j < 1 || j > 20)) {
                        if (!Main.cur().explored[i][j]) {
                            Main.cur().explored[i][j] = 1;
                            Main.cur().Show(i, j);
                        }
                    }
                }
            }
            gotoxy(Main.locx, Main.locy);
            Player.Show();
        } else if (print == print_torch) {
            if (bakx >= Main.locx - horizon && bakx <= Main.locx + horizon && baky >= Main.locy - horizon && baky <= Main.locy + horizon) {
                Main.cur().Show(bakx, baky);
            }
            for (int i = min(bakx, Main.locx) - horizon; i <= max(bakx, Main.locx) + horizon; i ++) {
                for (int j = min(baky, Main.locy) - horizon; j <= max(baky, Main.locy) + horizon; j ++) {
                    if (!(i < 1 || i > 40 || j < 1 || j > 20)) {
                        if ((i >= Main.locx - horizon && i <= Main.locx + horizon && j >= Main.locy - horizon && j <= Main.locy + horizon) &&
                            ((!(i >= bakx - horizon && i <= bakx + horizon && j >= baky - horizon && j <= baky + horizon))) ) {
                            Main.cur().explored[i][j] = 1;
                            Main.cur().Show(i, j);
                        } else if (!(i >= Main.locx - horizon && i <= Main.locx + horizon && j >= Main.locy - horizon && j <= Main.locy + horizon)){
                            Main.cur().explored[i][j] = 0;
                            gotoxy(i, j);
                            Dark.Show();
                        }
                    }
                }
            }
            gotoxy(Main.locx, Main.locy);
            Player.Show();
        }
        if (Events[Main.locz][Main.locx].find(Main.locy) != Events[Main.locz][Main.locx].end()) {
            Gos::ExecuteFunc(Events[Main.locz][Main.locx][Main.locy], {});
        }
    }
}

void KeyPressed() {
    if (!kbhit()) {
        return;
    }
    char ch = getch();
    int bakx = Main.locx, baky = Main.locy;
    if (Keys.find(ch) != Keys.end()) {
        Keys[ch]();
    }
    if (UserKeys.find(ch) != UserKeys.end()) {
        Gos::ExecuteFunc(UserKeys[ch], {});
    }
    if (Main.locx != bakx || Main.locy != baky) {
        LocationMoved(bakx, baky);
    }
}

unsigned long long say_time = 0;
int showing_time = 3000 / Main.timePerFrame;
void HideMessage() {
    if (Main.framecnt - say_time > showing_time) {
        gotoxy(1, 21);
        color(BLACK, WHITE);
        for (int i = 0; i < 320; i ++) {
            cout << ' ';
        }
    }
}

#define Func(name) Any name(vector<Any> args)

Func(AddEvent) {
    int x = args[0].Int();
    int y = args[1].Int();
    int z = Main.locz;
    if (args.size() > 3) {
        z = args[2].Int();
        Events[z][x][y] = args[3];
    } else {
        Events[z][x][y] = args[2];
    }
    return nullptr;
}

Func(AddKey) {
    string ch = args[0].String();
    for (const char& x : ch) {
        UserKeys[x] = args[1];
    }
    return nullptr;
}

Func(SetBlock) {
    if (args.size() == 2) {
        ifstream fin;
        int type = args[0].Int();
        int x, y;
        string filename = args[1].String();
        if (filename[0] != '.') {
            fin.open("maps/" + MapName + "/" + filename);
        } else {
            fin.open(filename);
        }
        if (fin) {
            while (fin >> x >> y) {
                Main.cur().Set(x, y, BlockArray[type]);
            }
        }
        fin.close();
    } else if (args.size() == 3) {
        int type = args[0].Int();
        int x = args[1].Int();
        int y = args[2].Int();
        Main.cur().Set(x, y, BlockArray[type]);
        Main.cur().Show(x, y);
        color(BLACK, WHITE);
    }
    return nullptr;
}

Func(GetBlock) {
    return (int)(Main.cur().Get(args[0].Int(), args[1].Int())->id);
}

Func(Crossable) {
    return (int)(Main.cur().Get(args[0].Int(), args[1].Int())->crossable ? 1 : 0);
}

int typecnt = 3;
Func(DefBlock) {
    Any tmp = (int)BlockArray.size();
    args[0].Assign(tmp);
    UserType[typecnt] = BlockType(args[1].Int(), args[2].Int(), args[3].String(), args[4].Int(), typecnt);
    BlockArray.push_back(&UserType[typecnt]);
    typecnt ++;
    return nullptr;
}

Func(GetX) {
    return Main.locx;
}

Func(GetY) {
    return Main.locy;
}

Func(GetZ) {
    return Main.locz;
}

Func(UseMap) {
    MapName = args[0].String();
    return nullptr;
}

Func(Refresh) {
    gotoxy(1, 1);
    Main.cur().Init();
    gotoxy(Main.locx, Main.locy);
    Player.Show();
    return nullptr;
}

Func(SetLocation) {
    int x = Main.locx, y = Main.locy;
    Main.locx = args[0].Int();
    Main.locy = args[1].Int();
    if (args.size() == 3) {
        Main.locz = args[2].Int();
        gotoxy(1, 1);
        Main.cur().Init();
    }
    LocationMoved(x, y);
    return nullptr;
}

Func(ResetMap) {
    memset(Main.cur().explored, 0, sizeof(Main.cur().explored));
    Main.cur() = Floor(&Null);
    if (print == print_explore || print == print_torch) {
        for (int i = 1; i <= horizon + 1; i ++) {
            for (int j = 1; j <= horizon + 1; j ++) {
                Main.cur().explored[i][j] = 1;
            }
        }
    } else if (print == print_overview) {
        for (int i = 1; i <= 40; i ++) {
            for (int j = 1; j <= 20; j ++) {
                Main.cur().explored[i][j] = 1;
            }
        }
    }
    return nullptr;
}

Func(AddFloor) {
    Main.fl.push_back(Floor(&Null));
    Main.locz ++;
    if (print == print_explore || print == print_torch) {
        for (int i = 1; i <= horizon + 1; i ++) {
            for (int j = 1; j <= horizon + 1; j ++) {
                Main.cur().explored[i][j] = 1;
            }
        }
    } else if (print == print_overview) {
        for (int i = 1; i <= 40; i ++) {
            for (int j = 1; j <= 20; j ++) {
                Main.cur().explored[i][j] = 1;
            }
        }
    }
    return nullptr;
}

Func(FloorCount) {
    return (int)Main.fl.size();
}

Func(SetGamemode) {
    print = args[0].Int();
    return nullptr;
}

Func(SetHorizon) {
    horizon = args[0].Int();
    return nullptr;
}

Func(MapInit) {
    if (print == print_explore || print == print_torch) {
        for (int i = 1; i <= horizon + 1; i ++) {
            for (int j = 1; j <= horizon + 1; j ++) {
                Main.cur().explored[i][j] = 1;
            }
        }
    } else if (print == print_overview) {
        for (int i = 1; i <= 40; i ++) {
            for (int j = 1; j <= 20; j ++) {
                Main.cur().explored[i][j] = 1;
            }
        }
    }
    gotoxy(1, 1);
    Main.cur().Init();
    gotoxy(1, 1);
    Player.Show();
    Main.locx = Main.locy = 1;
    return nullptr;
}

#if defined(linux) || defined(__APPLE__)
 #define UTF8ToGBK(x) (x)
#else
string UTF8ToGBK(string str) {
	const char * strUTF8 = str.c_str();
    int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, NULL, 0);
    wchar_t* wszGBK = new wchar_t[len + 1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, wszGBK, len);
    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
    char* szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
    string strTemp(szGBK);
    if (wszGBK) {
        delete[] wszGBK;
    }
    if (szGBK) {
        delete[] szGBK;
    }
    return strTemp;
}
#endif

Func(Say) {
    say_time = Main.framecnt;
    gotoxy(1, 21);
    color(BLACK, WHITE);
    for (auto& x : args) {
        if (x.GetType() == typeid(string)) {
            cout << UTF8ToGBK(x.String());
        } else {
            cout << x;
        }
    }
    for (int i = 0; i < 240; i ++) {
        cout << ' ';
    }
    return nullptr;
}

Func(Print) {
    for (auto& x : args) {
        if (x.GetType() == typeid(string)) {
            cout << UTF8ToGBK(x.String());
        } else {
            cout << x;
        }
    }
    return nullptr;
}

Func(Println) {
    Print(args);
    cout << endl;
    return nullptr;
}

Func(Path) {
#if defined(linux) || defined(__APPLE__)
    return (string)("\"" + args[0].String() + "\"");
#else
    string s = args[0].String();
    string ans = "\"";
    for (const auto& ch : s) {
        if (ch == '/') {
            ans.push_back('\\');
        } else {
            ans.push_back(ch);
        }
    }
    ans.push_back('\"');
    return ans;
#endif
}

void Init() {
    Gos::ImportDefaultLib();

    Main.fl.push_back(Floor(&Null));
    Keys['w'] = []() { Main.locy --; };
    Keys['s'] = []() { Main.locy ++; };
    Keys['a'] = []() { Main.locx --; };
    Keys['d'] = []() { Main.locx ++; };
    Keys['q'] = []() { Exit(); };

    Main.Add(KeyPressed);
    Main.Add(HideMessage);
    
    Gos::ImportConst({
        {"Empty", 0},
        {"Wall", 1},
        {"Stair", 2},
        {"Overview", 0},
        {"Explore", 1},
        {"Torch", 2},
    });

    Gos::ImportFunc({
        {"getBlock", GetBlock},
        {"setBlock", SetBlock},
        {"defBlock", DefBlock},
        {"isCrossable", Crossable},
        {"mapX", GetX},
        {"mapY", GetY},
        {"mapZ", GetZ},
        {"setMap", UseMap},
        {"mapRefresh", Refresh},
        {"mapReset", ResetMap},
        {"mapInit", MapInit},
        {"setPos", SetLocation},
        {"setHorizon", SetHorizon},
        {"setGamemode", SetGamemode},
        {"addLevel", AddFloor},
        {"levelCount", FloorCount},
        {"say", Say},
        {"path", Path},
        {"print", Print},
        {"println", Println},
        {"setEvent", AddEvent},
        {"setKeyEvent", AddKey},
    });
}

int RPGStart(vector<Any> args) {
    hidecursor();
    Init();
    exec = Gos::BuildGos("config.gos");
    Gos::ExecuteFunc(exec, {args});
    Main.Exec();
    return 0;
}
