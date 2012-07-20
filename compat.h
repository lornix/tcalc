/* compatibility stuff for DOS -> UNIX */

#ifndef COMPAT_H
#define COMPAT_H

/* from 'conio.h' */
#define _NOCURSOR      0
#define _SOLIDCURSOR   1
#define _NORMALCURSOR  2
struct text_info {
 unsigned char winleft;
 unsigned char wintop;
 unsigned char winright;
 unsigned char winbottom;
 unsigned char attribute;
 unsigned char normattr;
 unsigned char currmode;
 unsigned char screenheight;
 unsigned char screenwidth;
 unsigned char curx;
 unsigned char cury;
};
enum text_modes { LASTMODE=-1, BW40=0, C40, BW80, C80, MONO=7, C4350=64 };
#if !defined(__COLORS)
#define __COLORS
enum COLORS {
 BLACK,   /* dark colors */
 BLUE,
 GREEN,
 CYAN,
 RED,
 MAGENTA,
 BROWN,
 LIGHTGRAY,
 DARKGRAY,  /* light colors */
 LIGHTBLUE,
 LIGHTGREEN,
 LIGHTCYAN,
 LIGHTRED,
 LIGHTMAGENTA,
 YELLOW,
 WHITE
};
#endif
#define BLINK  128 /* blink bit */

/*
 * extern  int _Cdecl directvideo;
 * extern  int _Cdecl _wscroll;
 */

void  clreol(void);
void  clrscr(void);
void  delline(void);
int   gettext(int left,int top,int right,int bottom,void* destin);
void  gettextinfo(struct text_info* r);
void  gotoxy(int x,int y);
void  highvideo(void);
void  insline(void);
void  lowvideo(void);
int   movetext(int left,int top,int right,int bottom,int destleft,int desttop);
void  normvideo(void);
int   puttext(int left,int top,int right,int bottom,void* source);
void  textattr(int newattr);
void  textbackground(int newcolor);
void  textcolor(int newcolor);
void  textmode(int newmode);
int   wherex(void);
int   wherey(void);
void  window(int left,int top,int right,int bottom);
void  _setcursortype(int cur_t);
char* cgets(char* str);
int   cprintf(const char* format,...);
int   cputs(const char* str);
int   cscanf(const char* format,...);
int   getch(void);
int   getche(void);
char* getpass(const char* prompt);
int   kbhit(void);
int   putch(int c);
int   ungetch(int ch);

#endif
