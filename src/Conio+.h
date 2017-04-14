#ifndef CONIOPLUS_H
#define CONIOPLUS_H

#ifdef linux
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

static bool kbhit() {
	struct termios oldt, newt;
	int ch;
	int oldf;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	if(ch != EOF) {
		ungetc(ch, stdin);
		return 1;
	}
	return 0;
}

static int getch(void) {
     struct termios tm, tm_old;
     int fd = 0, ch;
     if (tcgetattr(fd, &tm) < 0) {//保存现在的终端设置
          return -1;
     }
     tm_old = tm;
     cfmakeraw(&tm);//更改终端设置为原始模式，该模式下所有的输入数据以字节为单位被处理
     if (tcsetattr(fd, TCSANOW, &tm) < 0) {//设置上更改之后的设置
          return -1;
     }
     ch = getchar();
     if (tcsetattr(fd, TCSANOW, &tm_old) < 0) {//更改设置为最初的样子
          return -1;
     }
     return ch;
}

static void gotoxy(int x,int y) {
	printf("\033[%d;%dH",y,2*x-1);
}

static void hidecursor() {
	printf("\033[?25l");
}

static void unhidecursor() {
	printf("\033[?25h");
}

static void color(int a,int b) {
	printf("\033[%dm\033[%dm",b+40,a+30);
}

static void foreground(int a) {
    printf("\033[%dm",a+30);
}

static void background(int a) {
    printf("\033[%dm",a+40);
}

static int random(int m) {
	return (int)((double)rand()/RAND_MAX*(m-1)+0.5);
}

#else

#include <conio.h>
#include <windows.h>
#include <time.h>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#define HighLight()
#define ClearColor()
using namespace std;
/*#define black 8
#define blue 9
#define green 10
#define cyan 11
#define red 12
#define purple 13
#define yellow 14
#define white 15*/
#define BLACK 8
#define BLUE 9
#define GREEN 10
#define CYAN 11
#define RED 12
#define PURPLE 13
#define YELLOW 14
#define WHITE 15

#define on_(x,y) (x+16*y)

#define Clear() system("cls")

static void gotoxy(int x,int y) {
    x=2*x-1;
	COORD c;
	c.X=x-1;
	c.Y=y-1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}

static int wherex() {
	CONSOLE_SCREEN_BUFFER_INFO pBuffer;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&pBuffer);
	return (pBuffer.dwCursorPosition.X+1);
}


static int wherey() {
	CONSOLE_SCREEN_BUFFER_INFO pBuffer;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&pBuffer);
	return (pBuffer.dwCursorPosition.Y+1);
}

static void hidecursor() {
	CONSOLE_CURSOR_INFO cursor_info = {1, 0};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
}

static void unhidecursor() {
	CONSOLE_CURSOR_INFO cursor_info = {1, 25};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
}

static int color(int a,int b)
{
	if ((a>15)||(b>15)) return -1;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),on_(a,b));
	return on_(a,b);
}

static int foreground(int a) {
    return color(a, BLACK);
}

static int background(int a) {
    return color(WHITE, a);
}

#endif

static double pro_time() {
	return (double)clock()/CLOCKS_PER_SEC;
}

static int readkey(double _time__)
{
	double time_last=pro_time();
	while (!kbhit()&&(pro_time()-time_last<_time__))  ;
	if (!kbhit()) return 0;
	return getch();
}

static string colorname(int x) {
    switch (x) {
        case BLACK:
            return "black";
        case BLUE:
            return "blue";
        case GREEN:
            return "green";
        case CYAN:
            return "cyan";
        case RED:
            return "red";
        case PURPLE:
            return "purple";
        case YELLOW:
            return "yellow";
        case WHITE:
            return "white";
    }
    return "none";
}

static int getcolorr(string x) {
    if (x=="black")
        return BLACK;
    if (x=="blue")
        return BLUE;
    if (x=="green")
        return GREEN;
    if (x=="cyan")
        return CYAN;
    if (x=="red")
        return RED;
    if (x=="purple")
        return PURPLE;
    if (x=="yellow")
        return YELLOW;
    if (x=="white")
        return WHITE;
    return -1;
}
#endif
