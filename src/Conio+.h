#ifndef CONIOPLUS_H
#define CONIOPLUS_H

#if defined(linux) || defined(__APPLE__)
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
using namespace std;

#define BLACK 0
#define BLUE 4
#define GREEN 6
#define CYAN 2
#define RED 1
#define PURPLE 5
#define YELLOW 3
#define WHITE 7

bool kbhit();
int getch(void);
#else

#include <conio.h>
#include <windows.h>
#include <time.h>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#define BLACK 8
#define BLUE 9
#define GREEN 10
#define CYAN 11
#define RED 12
#define PURPLE 13
#define YELLOW 14
#define WHITE 15

#endif

void gotoxy(int x,int y);
void gotoxy_origin(int x,int y);
void hidecursor();
void unhidecursor();
void color(int a,int b);
void foreground(int a);
void background(int a);
double pro_time();
int readkey(double _time__);
string colorname(int x);
int getcolor(string x);
void clearcolor();
void clear();
#endif
