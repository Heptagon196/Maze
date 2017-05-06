#include "Interpreter.h"
#include "Conio+.h"
#if defined(linux) || defined(__APPLE__)
#include <sys/types.h>
#include <sys/ioctl.h>
#include <termios.h>
#endif
#define GetStr(x, y) if (para[x].first==REFER) y=p->Str[para[x].second]; else y=para[x].second.substr(1, para[x].second.length()-2);
#define GetInt(x, y) if (para[x].first==REFER) y=p->Int[para[x].second]; else y=Transfer(para[x].second);

Def(Kbhit) {
    if (kbhit())
        return True;
    else
        return False;
}

Def(HideCursor) {
    hidecursor();
    return Empty;
}

Def(UnHideCursor) {
    unhidecursor();
    return Empty;
}

Def(Getch) {
    char ch=getch();
    return ParaList(1, Pair(STR, '"'+string(1, ch)+'"'));
}

Def(Readkey) {
    int x;
    if (para[0].first==REFER && p->type[para[0].second]==INTE)
        x=p->Int[para[0].second];
    else if (para[0].first==INTE)
        x=Transfer(para[0].second);
    char ch=readkey((double)x/1000.0);
    return ParaList(1, Pair(STR, '"'+string(1, ch)+'"'));
}

Def(GotoXY) {
    int x[2];
    for (int i=0;i<2;i++)
        if (para[i].first==REFER && p->type[para[i].second]==INTE)
            x[i]=p->Int[para[i].second];
        else if (para[i].first==INTE)
            x[i]=Transfer(para[i].second);
    gotoxy_origin(x[0], x[1]);
    return Empty;
}

Def(Color) {
    int x[2];
    for (int i=0;i<2;i++)
        if (para[i].first==REFER && p->type[para[i].second]==INTE)
            x[i]=p->Int[para[i].second];
        else if (para[i].first==INTE)
            x[i]=Transfer(para[i].second);
    color(x[0], x[1]);
    return Empty;
}

Def(ClearColor) {
    clearcolor();
    return Empty;
}

Def(Clear) {
    clear();
    return Empty;
}

Def(Bash) {
    if (para[0].first==REFER)
        system(p->Str[para[0].second].c_str());
    else if (para[0].first==STR)
        system(para[0].second.substr(1, para[0].second.length()-2).c_str());
    return Empty;
}

Def(Random) {
    if (para.size()==0)
        return ParaList(1, Pair(INTE, Transfer(rand())));
    int a[2];
    for (int i=0;i<2;i++)
        if (para[i].first==REFER)
            a[i]=p->Int[para[i].second];
        else
            a[i]=Transfer(para[i].second);
    return ParaList(1, Pair(INTE, Transfer(rand()%(a[1]-a[0]+1)+a[0])));
}

ifstream fin;
ofstream fout;
Def(ReadFrom) {
    string file;
    GetStr(0, file);
    if (file=="/dev/stdin") {
        fin.close();
        cin.rdbuf(InBuf);
        return True;
    }
    fin.open(file);
    if (!fin)
        return False;
    cin.rdbuf(fin.rdbuf());
    return True;
}

Def(PrintTo) {
    string file;
    GetStr(0, file);
    if (file=="/dev/stdout") {
        fout.close();
        cout.rdbuf(OutBuf);
        return True;
    }
    fout.open(file);
    if (!fout)
        return False;
    cout.rdbuf(fout.rdbuf());
    return True;
}

Def(Exec) {
    string e;
    GetStr(0, e);
    ifstream fin;
    fin.open(e);
    if (fin)
        p->Exec(fin);
    fin.close();
    return Empty;
}

Def(PrintMid) {
#if defined(linux) || defined(__APPLE__)
    struct winsize size;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &size);
    int W=size.ws_col;
#else
#define W 80
#endif
    string s;
    GetStr(0, s);
    for (int i=0;i<(W-s.length())/2;i++)
        putchar(' ');
    cout << s;
    for (int i=0;i<W-s.length()-(W-s.length())/2;i++)
        putchar(' ');
#if defined(linux) || defined(__APPLE__)
#else
    putchar('\n');
#endif
    return Empty;
}

void ImportExt(Interpreter &s) {
#if defined(linux) || defined(__APPLE__)
    s.AddVar("black", 0);
    s.AddVar("blue", 4);
    s.AddVar("green", 6);
    s.AddVar("cyan", 2);
    s.AddVar("red", 1);
    s.AddVar("purple", 5);
    s.AddVar("yellow", 3);
    s.AddVar("white", 7);
    s.AddVar("LINUX", 1);
#else
    s.AddVar("black", 8);
    s.AddVar("blue", 9);
    s.AddVar("green", 10);
    s.AddVar("cyan", 11);
    s.AddVar("red", 12);
    s.AddVar("purple", 13);
    s.AddVar("yellow", 14);
    s.AddVar("white", 15);
    s.AddVar("LINUX", 0);
#endif
    s.Add("getch", Getch);
    s.Add("readkey", Readkey);
    s.Add("kbhit", Kbhit);
    s.Add("gotoxy", GotoXY);
    s.Add("color", Color);
    s.Add("hidecursor", HideCursor);
    s.Add("unhidecursor", UnHideCursor);
    s.Add("clearcolor", ClearColor);
    s.Add("clear", Clear);
    s.Add("system", Bash);
    s.Add("random", Random);
    s.Add("printto", PrintTo);
    s.Add("readfrom", ReadFrom);
    s.Add("printmid", PrintMid);
    s.Add("exec", Exec);
}

