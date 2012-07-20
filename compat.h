/* compatibility stuff for DOS -> UNIX */

#ifndef COMPAT_H
#define COMPAT_H

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

void textattr(int newattr);
void setmem(void *dest, unsigned length, char value);
void gotoxy(int x, int y);
int  cputs(const char *str);
void window(int left, int top, int right, int bottom);
int movetext(int left, int top, int right, int bottom, int destleft, int desttop);
void clrscr(void);
struct WORDREGS { unsigned int  ax, bx, cx, dx, si, di, cflag, flags; };
struct BYTEREGS { unsigned char al, ah, bl, bh, cl, ch, dl, dh; };
union  REGS     {
    struct  WORDREGS x;
    struct  BYTEREGS h;
};
int int86(int intno, union REGS *inregs, union REGS *outregs);
#define BLINK           128     /* blink bit */
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
void gettextinfo(struct text_info *r);
int bioskey(int cmd);
void movmem(void *src, void *dest, unsigned length);
int  access(const char *path, int amode);
#define O_BINARY        0x8000  /* no translation       */
int read(int handle, void *buf, unsigned len);
int close(int handle);
int write(int handle, void *buf, unsigned len);
double pow10(double p); /* was 'int' */
char *strupr(char *s);

#endif
