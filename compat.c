/* compatibility stuff for DOS -> UNIX */

#include "compat.h"
#include <stdio.h>

char* strupr(char* s)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    return s;
}
double acos(double x)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    return x;
}
double asin(double x)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    return x;
}
double atan(double x)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    return x;
}
double cos(double x)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    return x;
}
double cosh(double x)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    return x;
}
double exp(double x)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    return x;
}
double log(double x)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    return x;
}
double log10(double x)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    return x;
}
double pow(double x, double y)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    return x+y;
}
double pow10(double p)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    /* was 'int' */ return p;
}
double sin(double x)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    return x;
}
double sinh(double x)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    return x;
}
double sqrt(double x)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    return x;
}
double tan(double x)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    return x;
}
double tanh(double x)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    return x;
}
int access(const char* path, int amode)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    return amode+(long int)path;
}
int bioskey(int cmd)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    return cmd;
}
int close(int handle)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    return handle;
}
int cputs(const char* str)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    return *str;
}
int int86(int intno, union REGS* inregs, union REGS* outregs)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    outregs=outregs;
    inregs=inregs;
    return intno;
}
int movetext(int left, int top, int right, int bottom, int destleft, int desttop)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    return top+right+bottom+destleft+left+desttop;
}
int read(int handle, void* buf, unsigned len)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    buf=buf;
    return handle+len;
}
int write(int handle, void* buf, unsigned len)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    buf=buf;
    return handle+len;
}
void clrscr(void)
{
}

void gettextinfo(struct text_info* r)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    r=r;
}
void gotoxy(int x, int y)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    x=y;
    y=x;
}
void movmem(void* src, void* dest, unsigned length)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    src=src;
    dest=dest;
    length=length;
}
void setmem(void* dest, unsigned length, char value)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    length=(unsigned long)dest+value;
    length=length;
}
void textattr(int newattr)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    newattr=newattr;
}
void window(int left, int top, int right, int bottom)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    left=top+right+bottom;
    top=left;
}
unsigned long coreleft(void)
{
    fprintf(stderr,"%s: %s (%d)\n",__FILE__,__FUNCTION__,__LINE__);
    return 640*1024*1024;
}
