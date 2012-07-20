/* compatibility stuff for DOS -> UNIX */

#ifndef COMPAT_H
#define COMPAT_H

#define HUGE_VAL __DBL_MAX__
#define O_BINARY        0x8000
#define BLINK           128     /* blink bit */
enum text_modes { LASTMODE=-1, BW40=0, C40, BW80, C80, MONO=7, C4350=64 };
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
struct WORDREGS { unsigned int  ax, bx, cx, dx, si, di, cflag, flags; };
struct BYTEREGS { unsigned char al, ah, bl, bh, cl, ch, dl, dh; };
union  REGS     {
    struct  WORDREGS x;
    struct  BYTEREGS h;
};
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

char  *strupr(char *s);
double pow10(double x);
int    bioskey(int cmd);
int    cputs(const char *str);
int    int86(int intno, union REGS *inregs, union REGS *outregs);
int    movetext(int left, int top, int right, int bottom, int destleft, int desttop);
void   clrscr(void);
void   gettextinfo(struct text_info *r);
void   gotoxy(int x, int y);
void   movmem(void *src, void *dest, unsigned length);
void   setmem(void *dest, unsigned length, char value);
void   textattr(int newattr);
void   window(int left, int top, int right, int bottom);
unsigned long coreleft(void);

#endif
