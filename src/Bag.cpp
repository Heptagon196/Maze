#include <map>
#include "Interpreter.h"
#include "InterpreterExt.h"
#include "Conio+.h"

map<string, int> Items;

Def(AddItem) {
    string s;
    GetStr(0, s);
    Items[s] ++;
    return Empty;
}

Def(RemoveItem) {
    string s;
    GetStr(0, s);
    if (Items.find(s) == Items.end())
        return Empty;
    if (Items[s] > 0)
        Items[s] --;
    return Empty;
}

Def(GetItem) {
    string s;
    GetStr(0, s);
    return ParaList(1, Pair(INTE, Transfer(Items[s])));
}

Def(SelectItem) {
    int i=0;
    vector<string> s;
    for (map<string, int>::iterator it = Items.begin(); it != Items.end(); ++it)
        if (it->second)
            s.push_back(it->first);
    if (s.size() == 0)
        return ParaList(1, Pair(STR, "\"EMPTY\""));
    gotoxy(1, 1);
    color(BLACK, WHITE);
    for (int i=0;i<80;i++) {
        for (int j=0;j<20;j++)
            putchar(' ');
        putchar('\n');
    }
    gotoxy(1, 1);
    color(WHITE, BLACK);
    MiddlePuts("Select an item");
    MiddlePuts(s[0]);
    color(BLACK, WHITE);
    for (int i=1;i<s.size();i++)
        MiddlePuts(s[i]);
    char ch = 0;
    int ans=0;
    while (ch != '\n' && ch != '\r') {
        int bakans = ans;
        ch = getch();
        if (ch=='w')
            ans--;
        if (ch=='s')
            ans++;
        if (ch == 'q')
            return ParaList(1, Pair(STR, "\"EMPTY\""));
        if (ans==-1)
            ans=s.size()-1;
        if (ans==s.size())
            ans=0;
        if (ans != bakans) {
            gotoxy(1, bakans+2);
            color(BLACK, WHITE);
            MiddlePuts(s[bakans]);
            gotoxy(1, ans+2);
            color(WHITE, BLACK);
            MiddlePuts(s[ans]);
        }
    }
    return ParaList(1, Pair(STR, "\"" + s[ans] + "\""));
}

void ImportBag(Interpreter& s) {
    s.Add("item.add", AddItem);
    s.Add("item.remove", RemoveItem);
    s.Add("item.find", GetItem);
    s.Add("item.select", SelectItem);
}
