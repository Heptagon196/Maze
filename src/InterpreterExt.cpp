#include "Interpreter.h"
#include "Conio+.h"
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

void ImportExt(Interpreter &s) {
    s.AddVar("black", 0);
    s.AddVar("blue", 4);
    s.AddVar("green", 6);
    s.AddVar("cyan", 2);
    s.AddVar("red", 1);
    s.AddVar("purple", 5);
    s.AddVar("yellow", 3);
    s.AddVar("white", 7);
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
}

