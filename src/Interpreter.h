#ifndef INTERPRETER_H
#define INTERPRETER_H
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <functional>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
using namespace std;

#define INTE 0
#define STR 1
#define FUNC 2
#define REFER 3

typedef pair<int, string> Pair;
typedef vector<Pair> ParaList;
class Interpreter;
typedef function<ParaList(Interpreter*, ParaList)> Function;
#define Def(x) ParaList x (Interpreter* p, ParaList para)
int Transfer(string str);
string Transfer(int integer);
int ReferType(Interpreter* p, Pair x);
static streambuf* OutBuf = cout.rdbuf();
static streambuf* InBuf = cin.rdbuf();
#define GetStr(x, y) if (para[x].first==REFER) y=p->Str[para[x].second]; else y=para[x].second.substr(1, para[x].second.length()-2);
#define GetInt(x, y) if (para[x].first==REFER) y=p->Int[para[x].second]; else y=Transfer(para[x].second);

const ParaList Empty=ParaList(0);
const ParaList True=ParaList(1, Pair(INTE, "1"));
const ParaList False=ParaList(1, Pair(INTE, "0"));

class Interpreter {
    public:
        int printed;
        double lastprint;
        unordered_map<string, int> Int;
        unordered_map<string, string> Str;
        unordered_map<string, Function> Func;
        unordered_map<string, int> preserved;
        unordered_map<string, int> type;

        void Add(string name, Function func);
        void AddPreserved(string name, Function func);
        void AddVar(string name, int val);
        ParaList Exec(istream &fin);
        Interpreter();
};
#endif
