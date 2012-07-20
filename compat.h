/* compatibility stuff for DOS -> UNIX */

#ifndef COMPAT_H
#define COMPAT_H

void textattr(char textcolor);
void setmem(char* buf,int len,int fill);
void gotoxy(int col,int row);
void cputs(char *chrstr);
void window(int x1,int y1,int x2,int y2);
void movetext(int fromx1,int fromy1,int fromx2,int fromy2,int tox1,int toy2);
void clrscr();

int int86(int intno, union REGS *inregs, union REGS *outregs);
struct WORDREGS {
	unsigned int	ax, bx, cx, dx, si, di, cflag, flags;
};

struct BYTEREGS {
	unsigned char	al, ah, bl, bh, cl, ch, dl, dh;
};

union	REGS	{
	struct	WORDREGS x;
	struct	BYTEREGS h;
};

#endif
