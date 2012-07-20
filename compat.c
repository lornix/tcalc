/* compatibility stuff for DOS -> UNIX */

#include "compat.h"

char  *strupr(char *s) { return s; }
double acos(double x) { return x; }
double asin(double x) { return x; }
double atan(double x) { return x; }
double cos(double x) { return x; }
double cosh(double x) { return x; }
double exp(double x) { return x; }
double log(double x) { return x; }
double log10(double x) { return x; }
double pow(double x, double y) { return x+y; }
double pow10(double p) { /* was 'int' */ return p; }
double sin(double x) { return x; }
double sinh(double x) { return x; }
double sqrt(double x) { return x; }
double tan(double x) { return x; }
double tanh(double x) { return x; }
int    access(const char *path, int amode) { return amode+(long int)path; }
int    bioskey(int cmd) { return cmd; }
int    close(int handle) { return handle; }
int    cputs(const char *str) { return *str; }
int    int86(int intno, union REGS *inregs, union REGS *outregs) { outregs=outregs; inregs=inregs; return intno; }
int    movetext(int left, int top, int right, int bottom, int destleft, int desttop) { return top+right+bottom+destleft+left+desttop; }
int    read(int handle, void *buf, unsigned len) { buf=buf; return handle+len; }
int    write(int handle, void *buf, unsigned len) { buf=buf; return handle+len; }
void   clrscr(void) { }
void   gettextinfo(struct text_info *r) { r=r; }
void   gotoxy(int x, int y) { x=y; y=x; }
void   movmem(void *src, void *dest, unsigned length) { src=src; dest=dest; length=length; }
void   setmem(void *dest, unsigned length, char value) { length=(unsigned long)dest+value; length=length; }
void   textattr(int newattr) { newattr=newattr; }
void   window(int left, int top, int right, int bottom) { left=top+right+bottom; top=left; }
unsigned long coreleft(void) { return 0; }
