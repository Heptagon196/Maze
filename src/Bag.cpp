#include "Interpreter.h"
#include "InterpreterExt.h"
#include "Conio+.h"

unordered_map<string, int> Items;

Def(AddItem) {
    string s;
    GetStr(0, s);
    Items[s]++;
    return Empty;
}

Def(RemoveItem) {
    string s;
    GetStr(0, s);
    if (Items[s])
        Items[s]--;
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
    for (unordered_map<string, int>::iterator it = Items.begin(); it != Items.end(); ++it)
        s.push_back(it->first);
    gotoxy(1, 1);
    color(BLACK, WHITE);
    for (int i=0;i<80;i++) {
        for (int j=0;j<20;j++)
            putchar(' ');
        putchar('\n');
    }
    gotoxy(1, 1);
    MiddlePuts(s[0]);
    color(WHITE, BLACK);
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
        if (ans==-1)
            ans=s.size()-1;
        if (ans==s.size())
            ans=0;
        if (ans != bakans) {
            gotoxy(1, bakans+1);
            color(WHITE, BLACK);
            MiddlePuts(s[bakans]);
            gotoxy(1, ans+1);
            color(BLACK, WHITE);
            MiddlePuts(s[ans]);
        }
    }
    return ParaList(1, Pair(INTE, Transfer(ans)));
}

void ImportBag(Interpreter& s) {
    s.Add("item.add", AddItem);
    s.Add("item.remove", RemoveItem);
    s.Add("item.find", GetItem);
    s.Add("item.select", SelectItem);
}
