/* compatibility stuff for DOS -> UNIX */

#include "compat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <ncurses.h>
#include <signal.h>

#include "tcalc.h"

static struct text_info master_text_info;
static int donesetup=0;
static WINDOW* nwin=NULL;

char* strupr(char* s)
{
    int l=strlen(s);
    while (l>=0) {
        s[l]=toupper(s[l]);
        l--;
    }
    return s;
}
double pow10(double p)
{
    return pow(10.0f,p);
}
int bioskey(int cmd __attribute__((unused)))
{
    int key=getch();
    switch (key) {
        case KEY_HOME:      key=HOMEKEY;      break;
        case KEY_END:       key=ENDKEY;       break;
        case KEY_IC:        key=INSKEY;       break;
        case KEY_LEFT:      key=LEFTKEY;      break;
        case KEY_RIGHT:     key=RIGHTKEY;     break;
        case KEY_BACKSPACE: key=BS;           break;
        case KEY_DC:        key=DELKEY;       break;
        case KEY_ENTER:     key=CR;           break;
        case KEY_UP:        key=UPKEY;        break;
        case KEY_DOWN:      key=DOWNKEY;      break;
        case KEY_EIC:       key=ESC;          break;
        case KEY_PPAGE:     key=PGUPKEY;      break;
        case KEY_NPAGE:     key=PGDNKEY;      break;
        case KEY_SLEFT:     key=CTRLLEFTKEY;  break;
        case KEY_SRIGHT:    key=CTRLRIGHTKEY; break;
        case KEY_F(1):      key=F1;           break;
        case KEY_F(2):      key=F2;           break;
        case KEY_F(3):      key=F3;           break;
        case KEY_F(4):      key=F4;           break;
        case KEY_F(5):      key=F5;           break;
        case KEY_F(6):      key=F6;           break;
        case KEY_F(7):      key=F7;           break;
        case KEY_F(8):      key=F8;           break;
        case KEY_F(9):      key=F9;           break;
        case KEY_F(10):     key=F10;          break;
        default: break;
    }
    if (key>255) {
        key=(key-256)*256;
    }
    return key;
}
int cputs(const char* str)
{
    /* position cursor */
    attrset(COLOR_PAIR(master_text_info.attribute));
    mvaddstr(master_text_info.cury,master_text_info.curx,str);
    refresh();
    return 0;
}
int int86(int intno, union REGS* inregs, union REGS* outregs)
{
    static int cursorshape=0xFFFF;
    switch (intno) {
        case 0x10: /* get/set cursor shape */
            if (inregs->h.ah==1) { /* set cursor */
                cursorshape=inregs->x.cx;
                break;
            }
            else if (inregs->h.ah==3) { /* get cursor */
                outregs->x.cx=cursorshape;
                break;
            }
            /* fall through */
        default:
            fprintf(stderr,"int86: %04x AX: %04x\n",intno,inregs->x.ax);
    }
    return intno;
}
int movetext(int left, int top, int right, int bottom, int destleft, int desttop)
{
    /*
     * gotoxy(top,left);
     * cputs("from here");
     * gotoxy(destleft,desttop);
     * cputs("to here");
     */
    return top+right+bottom+destleft+left+desttop;
}
void clrscr(void)
{
    int row;
    static const int MAXBLANKS=255;
    char blankstr[MAXBLANKS+1];
    master_text_info.curx=1;
    master_text_info.cury=1;
    row=master_text_info.winright-master_text_info.winleft+1;
    if (row>MAXBLANKS)
        row=MAXBLANKS;
    sprintf(blankstr,"%*s",row,"");
    for (row=master_text_info.wintop; row<=master_text_info.winbottom; row++) {
        gotoxy(master_text_info.winleft,row);
        cputs(blankstr);
    }
}

void gettextinfo(struct text_info* r)
{
    r->winleft=master_text_info.winleft;
    r->wintop=master_text_info.wintop;
    r->winright=master_text_info.winright;
    r->winbottom=master_text_info.winbottom;
    r->attribute=master_text_info.attribute;
    r->normattr=master_text_info.normattr;
    r->currmode=master_text_info.currmode;
    r->screenheight=master_text_info.screenheight;
    r->screenwidth=master_text_info.screenwidth;
    r->curx=master_text_info.curx;
    r->cury=master_text_info.cury;
    return;
}
void gotoxy(int x, int y)
{
    master_text_info.curx=x-1;
    master_text_info.cury=y-1;
}
void movmem(void* src, void* dest, unsigned length)
{
    memmove(dest,src,length);
}
void setmem(void* dest, unsigned length, char value)
{
    memset(dest,value,length);
}
void textattr(int newattr)
{
    master_text_info.attribute=newattr;
}
static void finish_atexit(void)
{
    endwin();
}
static void finish_signal(int sig __attribute__((unused)))
{
    finish_atexit();
}
void window(int left, int top, int right, int bottom)
{
    if (!donesetup) {
        int color;
        master_text_info.attribute=7;
        master_text_info.normattr=7;
        master_text_info.currmode=C80;
        master_text_info.screenheight=24;
        master_text_info.screenwidth=80;
        /* arrange interrupts to terminate */
        signal(SIGINT, finish_signal);
        /* normal exit cleanup too */
        atexit(finish_atexit);
        /* begin ncurses setup */
        nwin=initscr();
        start_color();
        for (color=1; color<256; color++) {
            init_pair(color,(color&0x0f),(color>>4)&0x0f);
        }
        keypad(stdscr, TRUE);  /* enable keyboard mapping */
        nonl();         /* tell curses not to do NL->CR/NL on output */
        cbreak();       /* take input chars one at a time, no wait for \n */
        echo();         /* echo input - in color */
        /* set colorpairs */

        /* set flag so this only done once */
        donesetup=1;
    }
    master_text_info.curx=left;
    master_text_info.cury=top;
    master_text_info.winleft=left;
    master_text_info.wintop=top;
    master_text_info.winright=right;
    master_text_info.winbottom=bottom;
}
unsigned long coreleft(void)
{
    return 30*1024;
}
