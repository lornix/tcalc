/* compatibility stuff for DOS -> UNIX */

#ifndef COMPAT_H
#define COMPAT_H

#define BLINK           128     /* blink bit */
#define O_BINARY        0x8000  /* no translation       */
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
double acos(double x);
double asin(double x);
double atan(double x);
double cos(double x);
double cosh(double x);
double exp(double x);
double log(double x);
double log10(double x);
double pow(double x, double y);
double pow10(double p);
double sin(double x);
double sinh(double x);
double sqrt(double x);
double tan(double x);
double tanh(double x);
int    access(const char *path, int amode);
int    bioskey(int cmd);
int    close(int handle);
int    cputs(const char *str);
int    int86(int intno, union REGS *inregs, union REGS *outregs);
int    movetext(int left, int top, int right, int bottom, int destleft, int desttop);
int    read(int handle, void *buf, unsigned len);
int    write(int handle, void *buf, unsigned len);
void   clrscr(void);
void   gettextinfo(struct text_info *r);
void   gotoxy(int x, int y);
void   movmem(void *src, void *dest, unsigned length);
void   setmem(void *dest, unsigned length, char value);
void   textattr(int newattr);
void   window(int left, int top, int right, int bottom);
unsigned long coreleft(void);

#endif
