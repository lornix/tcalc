/* Turbo C++ - (C) Copyright 1987,1988,1990 by Borland International */

#include "tcalc.h"

static unsigned char colortable[256];

/* Sets the current color using the color table */
void setcolor(int color)
{
    textattr(colortable[color]);
} /* setcolor */

/* Prints a string in video memory at a selected location in a color */
void writef(int col, int row, int color, int width, char *format, ...)
{
    va_list arg_ptr;
    char output[81];
    int len;

    va_start(arg_ptr, format);
    vsprintf(output, format, arg_ptr);
    output[width] = 0;
    if ((len = strlen(output)) < width)
        setmem(&output[len], width - len, ' ');
    setcolor(color);
    gotoxy(col, row);
    cputs(output);
} /* writef */

/* Scrolls an area of the screen */
void tscroll(int direction, int lines, int x1, int y1, int x2, int y2, int attrib)
{
    if (lines == 0)
        window(x1, y1, x2, y2);
    else switch(direction) {
        case UP :
            movetext(x1, y1 + lines, x2, y2, x1, y1);
            window(x1, y2 - lines + 1, x2, y2);
            break;
        case DOWN :
            movetext(x1, y1, x2, y2 - lines, x1, y1 + lines);
            window(x1, y1, x2, y1 + lines - 1);
            break;
        case LEFT :
            movetext(x1 + lines, y1, x2, y2, x1, y1);
            window(x2 - lines + 1, y1, x2, y2);
            break;
        case RIGHT :
            movetext(x1, y1, x2 - lines, y2, x1 + lines, y1);
            window(x1, y1, x1 + lines - 1, y2);
            break;
    } /* switch */
    setcolor(attrib);
    clrscr();
    window(1, 1, 80, 25);
} /* tscroll */

/* Sets the shape of the cursor */
void setcursor(unsigned int shape)
{
    union REGS reg;

    reg.h.ah = 1;
    reg.x.cx = shape;
    int86(0X10, &reg, &reg);
} /* setcursor */

/* Returns the shape of the current cursor */
unsigned int getcursor(void)
{
    union REGS reg;

    reg.h.ah = 3;
    reg.h.bh = 0;
    int86(0X10, &reg, &reg);
    return(reg.x.cx);
} /* getcursor */

void changecursor(int insmode)
    /* Changes the cursor shape based on the current insert mode */
{
    if (insmode)
        setcursor(tallcursor);
    else
        setcursor(shortcursor);
} /* changecursor */

/* Prints the column headings */
void printcol(void)
{
    int col;
    char colstr[MAXCOLWIDTH + 1];

    tscroll(UP, 0, 1, 2, 80, 2, HEADERCOLOR);
    for (col = leftcol; col <= rightcol; col++) {
        centercolstring(col, colstr);
        writef(colstart[col - leftcol] + 1, 2, HEADERCOLOR, colwidth[col], colstr);
    }
} /* printcol */

/* Clears any data left in the last column */
void clearlastcol()
{
    int col;

    if ((col = colstart[rightcol - leftcol] + colwidth[rightcol]) < 80)
        tscroll(UP, 0, col + 1, 3, 80, SCREENROWS + 2, WHITE);
} /* clearlastcol */

/* Prints the row headings */
void printrow(void)
{
    int row;

    for (row = 0; row < SCREENROWS; row++)
        writef(1, row + 3, HEADERCOLOR, LEFTMARGIN, "%-d", row + toprow + 1);
} /* printrow */

/* Displays the contents of a cell */
void displaycell(int col, int row, int highlighting, int updating)
{
    int color;
    char *s;

    if ((updating) &&
            ((cell[col][row] == NULL) || (cell[col][row]->attrib != FORMULA)))
        return;
    s = cellstring(col, row, &color, FORMAT);
    if (highlighting) {
        color = HIGHLIGHTCOLOR;
        if (color == ERRORCOLOR)
            color = HIGHLIGHTERRORCOLOR;
    }
    writef(colstart[col-leftcol]+1,row-toprow+3,color,colwidth[col],"%s",s);
} /* displaycell */

/* Displays a column on the screen */
void displaycol(int col, int updating)
{
    int row;

    for (row = toprow; row <= bottomrow; row++)
        displaycell(col, row, NOHIGHLIGHT, updating);
} /* displaycol */

/* Displays a row on the screen */
void displayrow(int row, int updating)
{
    int col;

    for (col = leftcol; col <= rightcol; col++)
        displaycell(col, row, NOHIGHLIGHT, updating);
} /* displayrow */

/* Displays the current screen of the spreadsheet */
void displayscreen(int updating)
{
    int row;

    for (row = toprow; row <= bottomrow; row++)
        displayrow(row, updating);
    clearlastcol();
} /* displayscreen */

/* Clears the input line */
void clearinput(void)
{
    tscroll(UP, 0, 1, 25, 80, 25, WHITE);
    gotoxy(1, 25);
} /* clearinput */

/* Prints the type of cell and what is in it */
void showcelltype(void)
{
    char colstr[3], *s;
    int color;

    formdisplay = !formdisplay;
    s = cellstring(curcol, currow, &color, NOFORMAT);
    colstring(curcol, colstr);
    if (curcell == NULL)
        writef(1, 23, CELLTYPECOLOR, 80, "%s%d %s", colstr, currow + 1, MSGEMPTY);
    else switch(curcell->attrib) {
        case TEXT :
            writef(1, 23, CELLTYPECOLOR, 80, "%s%d %s", colstr, currow + 1, MSGTEXT);
            break;
        case VALUE :
            writef(1, 23, CELLTYPECOLOR, 80, "%s%d %s", colstr, currow + 1, MSGVALUE);
            break;
        case FORMULA :
            writef(1, 23, CELLTYPECOLOR, 80, "%s%d %s", colstr, currow + 1, MSGFORMULA);
            break;
    } /* switch */
    writef(1, 24, CELLCONTENTSCOLOR, 80, "%s", s);
    formdisplay = !formdisplay;
} /* showcelltype */

/* Displays the entire screen */
void redrawscreen(void)
{
    setrightcol();
    setbottomrow();
    writef(1, 1, MSGMEMORYCOLOR, strlen(MSGMEMORY), MSGMEMORY);
    writef(32, 1, PROMPTCOLOR, strlen(MSGCOMMAND), MSGCOMMAND);
    changeautocalc(autocalc);
    changeformdisplay(formdisplay);
    printfreemem();
    displayscreen(NOUPDATE);
}

/* Initializes the different cursor types */
void initcursor(void)
{
    struct text_info ti;

    gettextinfo(&ti);
    oldcursor = getcursor();
    shortcursor = 0x0607;
    tallcursor = 0x0507;
    if (ti.currmode == MONO) {
        shortcursor = 0x0A0C;
        tallcursor = 0x090C;
    }
} /* initcursor */

/* Sets up the color table */
void initcolortable(void)
{
    int color, fg, bg, fcolor, bcolor;
    struct text_info ti;

    gettextinfo(&ti);
    if (ti.currmode == C80) {
        for (color = 0; color <= 255; color++)
            colortable[color] = color;
    }
    else {
        for (fg = BLACK; fg <= WHITE; fg++) {
            if (fg == BLACK)
                fcolor = BLACK;
            else if (fg <= LIGHTGRAY)
                fcolor = LIGHTGRAY;
            else
                fcolor = WHITE;
            for (bg = BLACK; bg <= LIGHTGRAY; bg++) {
                if (bg == BLACK)
                    bcolor = BLACK;
                else {
                    if (fcolor == WHITE)
                        fcolor = BLACK;
                    bcolor = LIGHTGRAY;
                }
                colortable[fg + (bg << 4)] = fcolor + (bcolor << 4);
            }
        }
        for (fg = 128; fg <= 255; fg++)
            colortable[fg] = colortable[fg - 128] | 0x80;
    }
} /* initcolortable */
