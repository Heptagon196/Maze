#include <cstdio>
#include <stack>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <functional>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include "Conio+.h"
#include "Interpreter.h"
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

#if defined(linux) || defined(__APPLE__)
 #define UTF8ToGBK(x) (x)
#else
string UTF8ToGBK(string str) {
	const char * strUTF8 = str.c_str();
    int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, NULL, 0);
    wchar_t* wszGBK = new wchar_t[len+1];
    memset(wszGBK, 0, len*2+2);
    MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, wszGBK, len);
    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
    char* szGBK = new char[len+1];
    memset(szGBK, 0, len+1);
    WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
    string strTemp(szGBK);
    if(wszGBK) delete[] wszGBK;
    if(szGBK) delete[] szGBK;
    return strTemp;
}
#endif

void Puts(string s) {
    if (s=="")
        return ;
    for (int i=0;i<s.length()-1;i++)
        if (s[i]=='\\') {
            if (s[i+1]=='n')
                s.replace(i, 2, "\n");
            else
                s.replace(i, 2, s.substr(i+1, 1).c_str());
        }
    cout << UTF8ToGBK(s);
}

int Transfer(string str) {
    stringstream ss;
    ss.str("");
    if (str[0]=='"' && str[str.length()-1]=='"')
        ss << str.substr(1, str.length()-2);
    else
        ss << str;
    int x;
    ss >> x;
    return x;
}

string Transfer(int integer) {
    stringstream ss;
    ss.str("");
    ss << integer;
    string x;
    ss >> x;
    return x;
}

void Format(string &s, string x, int n) {
    string f='{'+Transfer(n)+'}';
    int pos;
    pos=s.find(f);
    while (pos!=-1) {
        s.replace(pos, f.length(), x);
        pos=s.find(f);
    }
}

void Format(string &s, int x, int n) {
    Format(s, Transfer(x), n);
}

void Format(string &s) {
    string f="\\n";
    int pos;
    pos=s.find(f);
    while (pos!=-1) {
        s.replace(pos, f.length(), "\n");
        pos=s.find(f);
    }
}

int ReferType(Interpreter* p, Pair x);

Def(print);
Def(mainfunc);
Def(println);
Def(Mov);
Def(Plus);
Def(Minus);
Def(Times);
Def(Div);
Def(Mod);

void Interpreter::Add(string name, Function func) {
    Func[name]=func;
    type[name]=FUNC;
}

void Interpreter::AddPreserved(string name, Function func) {
    Func[name]=func;
    type[name]=FUNC;
    preserved[name]=1;
}

void Interpreter::AddVar(string name, int val) {
    Int[name]=val;
    type[name]=INTE;
}

string GetArray(Interpreter *p, string s);

ParaList Interpreter::Exec(istream &fin) {
    ParaList para;
    string name;
    string tmp;
    char ch;
    int readstr=-1;
    while ((fin.read(&ch, 1)) && ch!=')' && ch!=' ' && ch!='\n')
        name.push_back(ch);
    if (Func.find(name)==Func.end() && name!="exit") {
        cerr << "Unknown function: " << name << endl;
        exit(1);
    }
    if (ch==')') {
        if (name=="exit") {
            unhidecursor();
            exit(0);
        }
        return Func[name](this, para);
    }
    map<char, function<void(void)> > exec;
    exec['"']=[&]() {
        readstr*=-1;
        tmp.push_back('"');
    };
    exec['[']=[&]() {
        int in1=1, in2=0, instr=-1;
        tmp+='[';
        while ((in1 || in2) && fin.read(&ch, 1)) {
            if (ch=='"')
                instr*=-1;
            if (instr==1) {
                tmp.push_back(ch);
                continue;
            }
            if (ch=='[')
                in1++;
            if (ch==']')
                in1--;
            if (ch=='(')
                in2++;
            if (ch==')')
                in2--;
            tmp.push_back(ch);
        }
    };
    exec['(']=[&]() {
        if (preserved[name]) {
            int in=1, instr=-1;
            tmp='(';
            while (in && fin.read(&ch, 1)) {
                if (ch=='"')
                    instr*=-1;
                if (instr==1) {
                    tmp.push_back(ch);
                    continue;
                }
                if (ch=='(')
                    in++;
                if (ch==')')
                    in--;
                tmp.push_back(ch);
            }
            return ;
        }
        ParaList tmp=Exec(fin);
        for (int i=0;i<tmp.size();i++)
            para.push_back(tmp[i]);
    };
    exec[' ']=[&]() {
        if (tmp=="")
            return ;
        int type;
        type=INTE;
        for (int i=0;i<tmp.size();i++)
            if (!isdigit(tmp[i]) && (!(tmp[i]=='-' && i==0)))
                type=REFER;
        if (tmp[0]=='"' && tmp[tmp.length()-1]=='"')
            type=STR;
        if (tmp[0]=='(' && tmp[tmp.length()-1]==')')
            type=FUNC;
        if (tmp[tmp.length()-1]==']') {
            tmp=GetArray(this, tmp);
            type=REFER;
        }
        para.push_back(Pair(type, tmp));
        tmp="";
    };
    exec['\n']=exec[' '];
    exec[';']=[&]() {
        string tmp;
        getline(fin, tmp, '\n');
    };
    while ((fin.read(&ch, 1)) && ch!=')') {
        if (readstr==1 && ch!='"') {
            tmp.push_back(ch);
            continue;
        }
        if (exec.find(ch)!=exec.end())
            exec[ch]();
        else
            tmp.push_back(ch);
    }
    if (tmp!="")
        exec[' ']();
    if (name=="exit") {
        unhidecursor();
        exit(0);
    }
    return Func[name](this, para);
}

Def(print) {
    if (cout.rdbuf()==OutBuf && p->Int["OUTPUT_TO_THE_BOTTOM"]==0) {
        gotoxy(1, 21);
        color(BLACK, WHITE);
        for (int i=0;i<80;i++)
            putchar(' ');
        gotoxy(1, 21);
    }
    for (int i=0;i<para.size();i++)
        if (para[i].first==INTE)
            cout << para[i].second;
        else if (para[i].first==STR)
            Puts(para[i].second.substr(1, para[i].second.length()-2));
        else if (para[i].first==REFER) {
            if (ReferType(p, para[i])==INTE)
                cout << p->Int[para[i].second];
            else if (p->Str.find(para[i].second)!=p->Str.end())
                Puts(p->Str[para[i].second]);
            else {
                cerr << "Unknown variable." << endl;
                exit(1);
            }
        } else {
            cerr << "Unknown variable." << endl;
            exit(1);
        }
    return para;
}

int ReferType(Interpreter* p, Pair x) {
    return p->type[x.second];
}

Def(println) {
    print(p, para);
    cout << '\n';
    return Empty;
}

Def(mainfunc) {
    stringstream ss;
    function<ParaList(string&)> exec=[&](string &s)->ParaList {
        ss.str("");
        ss << s;
        return p->Exec(ss);
    };
    string tmp;
    for (int i=para.size()-1;i>=0;i--)
        if (para[i].first==REFER) {
            para[i].first=p->type[para[i].second];
            if (para[i].first==INTE)
                para[i].second=Transfer(p->Int[para[i].second]);
            else
                para[i].second='"'+p->Str[para[i].second]+'"';
        } else if (para[i].first==FUNC)
            exec(tmp=para[i].second.substr(1, para[i].second.length()-1));
    return para;
}

Def(defint) {
    for (int i=0;i<para.size();i++) {
        p->Int[para[i].second]=0;
        p->type[para[i].second]=INTE;
    }
    return para;
}

Def(defstring) {
    for (int i=0;i<para.size();i++) {
        p->Str[para[i].second]="";
        p->type[para[i].second]=STR;
    }
    return para;
}

Def(Mov) {
    if (para[0].first!=REFER) {
        cerr << "Error parameter in function mov" << endl;
        exit(1);
    }
    if (para[1].first==REFER) {
        if (ReferType(p, para[1])==INTE) {
            if (ReferType(p, para[0])==INTE)
                p->Int[para[0].second]=p->Int[para[1].second];
            else
                p->Str[para[0].second]=Transfer(p->Int[para[1].second]);
        }
        else {
            if (ReferType(p, para[0])==INTE)
                p->Int[para[0].second]=Transfer(p->Str[para[1].second]);
            else
                p->Str[para[0].second]=p->Str[para[1].second];
        }
        return ParaList(1, para[0]);
    }
    if (ReferType(p, para[0])==INTE)
        p->Int[para[0].second]=Transfer(para[1].second);
    else
        p->Str[para[0].second]=(para[1].first==STR)?(para[1].second.substr(1, para[1].second.length()-2)):para[1].second;
    return ParaList(1, para[0]);
}

Def(Read) {
    for (int i=0;i<para.size();i++) {
        if (para[i].first!=REFER) {
            cerr << "Not a reference." << endl;
            exit(1);
        }
        if (ReferType(p, para[i])==INTE)
            cin >> p->Int[para[i].second];
        else if (ReferType(p, para[i])==STR)
            getline(cin, p->Str[para[i].second], '\n');
        else {
            cerr << "Unknown variable." << endl;
            exit(1);
        }
    }
    return para;
}

ParaList Base_Math(Interpreter* p, ParaList para,
    function<int(int, int)> op1,
    function<string(string, string)> op2) {
    int type[2];
    int Int[2];
    string Str[2];
    for (int i=0;i<2;i++)
        if (para[i].first==REFER) {
            type[i]=ReferType(p, para[i]);
            Int[i]=p->Int[para[i].second];
            Str[i]=p->Str[para[i].second];
        }
        else {
            type[i]=para[i].first;
            Int[i]=Transfer(para[i].second);
            Str[i]=para[i].second.substr(1, para[i].second.length()-2);
        }

    if (type[0]==INTE && type[1]==INTE) {
        Pair x;
        x.first=INTE;
        x.second=Transfer(op1(Int[0], Int[1]));
        return ParaList(1, x);
    }
    Pair x;
    x.first=STR;
    x.second='"';
    string s[2];
    for (int i=0;i<2;i++)
        if (type[i]==INTE)
            s[i]=Transfer(Int[i]);
        else
            s[i]=Str[i];
    x.second+=op2(s[0], s[1]);
    x.second+='"';
    return ParaList(1, x);
}

string Exit_Argu() {
    cerr << "Invalid argument type." << endl;
    exit(1);
    return "";
}
#define DefOp(x, y, z) Def(x) {return Base_Math(p, para,[](int a, int b)->int {return y;},[](string a, string b)->string {return z;});}
DefOp(Plus, a+b, a+b);
DefOp(Minus, a-b, Exit_Argu());
DefOp(Times, a*b, Exit_Argu());
DefOp(Div, a/b, Exit_Argu());
DefOp(Mod, a%b, Exit_Argu());
DefOp(Pow, pow(a, b), Exit_Argu());
DefOp(Smaller, a<b, a<b?"1":"0");
DefOp(Bigger, a>b, a>b?"1":"0");
DefOp(NoBigger, a<=b, a<=b?"1":"0");
DefOp(NoSmaller, a>=b, a>=b?"1":"0");
DefOp(Equalto, a==b, a==b?"1":"0");
DefOp(NotEqualto, a!=b, a!=b?"1":"0");
DefOp(Or, a||b, Exit_Argu());
DefOp(And, a&&b, Exit_Argu());
//DefOp(Byte_Or, a|b, Exit_Argu());
//DefOp(Byte_And, a&b, Exit_Argu());
//DefOp(Byte_Xor, a^b, Exit_Argu());
//DefOp(Byte_Left, a<<b, Exit_Argu());
//DefOp(Byte_Right, a>>b, Exit_Argu());
Def(Not) {
    int type;
    int Int;
    if (para[0].first==REFER) {
        if (ReferType(p, para[0])==INTE) {
            if (p->Int[para[0].second]==0)
                return True;
            else
                return False;
        }
    } else {
        if (para[0].first==INTE) {
            if (para[0].second=="0")
                return True;
            else
                return False;
        }
    }
    cerr << "Invalid operator argument." << endl;
    exit(1);
}
#define OpBy(x, y) Def(x) {ParaList tmp= y (p, para);para.pop_back();para.push_back(tmp[0]);return Mov(p, para);}
#define OpOp(x, y, z) Def(x) {para.push_back(y);return z (p, para);}
OpBy(PlusBy, Plus);
OpBy(MinusBy, Minus);
OpBy(TimesBy, Times);
OpBy(DivBy, Div);
OpBy(ModBy, Mod);
OpOp(PlusPlus, Pair(INTE, "1"), PlusBy);
OpOp(MinusMinus, Pair(INTE, "1"), MinusBy);

Def(For) {
    string Loop="(main ";
    string Start=para[0].second.substr(1, para[0].second.length()-1),
           Condition=para[1].second.substr(1, para[1].second.length()-1),
           Summer=para[2].second.substr(1, para[2].second.length()-1);
    for (int i=3;i<para.size();i++)
        Loop+=para[i].second+' ';
    Loop+=")";
    stringstream ss;
    function<ParaList(string&)> exec=[&](string &s)->ParaList {
        ss.str("");
        ss << s;
        return p->Exec(ss);
    };
    string tmp;
    for (exec(Start);(tmp=exec(Condition)[0].second)!="0" && tmp!="\"0\"";exec(Summer))
        exec(Loop);
    return Empty;
}

Def(If) {
    string Loop="(main ";
    string ElseLoop="(main ";
    string Condition=para[0].second.substr(1, para[0].second.length()-1);
    int readloop=1;
    for (int i=1;i<para.size();i++) {
        if (para[i].second=="(else)") {
            readloop=0;
            continue;
        }
        if (readloop)
            Loop+=para[i].second+' ';
        else
            ElseLoop+=para[i].second+' ';
    }
    Loop+=")";
    ElseLoop+=")";
    stringstream ss;
    function<ParaList(string&)> exec=[&](string &s)->ParaList {
        ss.str("");
        ss << s;
        return p->Exec(ss);
    };
    string tmp;
    if ((tmp=exec(Condition)[0].second)!="0" && (tmp!="\"0\""))
        exec(Loop);
    else
        exec(ElseLoop);
    return Empty;
}

Def(While) {
    string Loop="(main ";
    string Condition=para[0].second.substr(1, para[1].second.length()-1);
    int readloop=1;
    for (int i=1;i<para.size();i++)
        Loop+=para[i].second+' ';
    Loop+=")";
    stringstream ss;
    function<ParaList(string&)> exec=[&](string &s)->ParaList {
        ss.str("");
        ss << s;
        return p->Exec(ss);
    };
    string tmp;
    while ((tmp=exec(Condition)[0].second)!="0" && tmp!="\"0\"")
        exec(Loop);
    return Empty;
}

string GetArray(Interpreter *p, string s) {
    string s1, s2;
    int pos;
    for (int i=0;i<s.length();i++)
        if (s[i]=='[') {
            pos=i;
            break;
        }
    s1=s.substr(0, pos);
    s2=s.substr(pos+1, s.length()-pos-2);
    stringstream ss;
    string x="(main "+s2+')';
    ss << x;
    x=p->Exec(ss)[0].second;
    return s1+'_'+x;
}

Def(Defer) {
    return para;
}

Def(Bool_True) {
    return True;
}

Def(Bool_False) {
    return False;
}

Def(Formater) {
    string target;
    if (para[0].first==REFER)
        target=p->Str[para[0].second];
    else if (para[0].first==STR)
        target=para[0].second.substr(1, para[0].second.length()-2);
    if (para.size()<=1) {
        Format(target);
        return ParaList(1, Pair(STR, target));
    }
    for (int i=1;i<para.size();i++) {
        if (para[i].first==REFER) {
            if (ReferType(p, para[i])==INTE)
                Format(target, p->Int[para[i].second], i-1);
            else
                Format(target, p->Str[para[i].second], i-1);
        } else if (para[i].first==INTE)
            Format(target, para[i].second, i-1);
        else if (para[i].first==STR)
            Format(target, para[i].second.substr(1, para[i].second.length()-2), i-1);
    }
    return ParaList(1, Pair(STR, '"'+target+'"'));
}

Interpreter::Interpreter() {
    Str["CR"]="\r";
    type["CR"]=STR;
    Str["ENTER"]="\n";
    type["ENTER"]=STR;
    Add("true", Bool_True);
    Add("false", Bool_False);
    AddVar("true", 1);
    AddVar("false", 0);
    Add("(main", mainfunc);
    Add("_", mainfunc);
    Add("print", print);
    Add("read", Read);
    Add("println", println);
    Add("int", defint);
    Add("string", defstring);
    Add("format", Formater);
    Add("=", Mov);
    Add("+", Plus);
    Add("-", Minus);
    Add("*", Times);
    Add("/", Div);
    Add("%", Mod);
    Add("+=", PlusBy);
    Add("++", PlusPlus);
    Add("-=", MinusBy);
    Add("--", MinusMinus);
    Add("*=", TimesBy);
    Add("/=", DivBy);
    Add("%=", ModBy);
    Add("**", Pow);
    Add("<", Smaller);
    Add(">=", NoSmaller);
    Add(">", Bigger);
    Add("<=", NoBigger);
    Add("==", Equalto);
    Add("!=", NotEqualto);
    Add("||", Or);
    Add("&&", And);
    Add("!", Not);
    AddPreserved("for", For);
    AddPreserved("if", If);
    AddPreserved("while", While);
    AddPreserved("defer", Defer);
}
