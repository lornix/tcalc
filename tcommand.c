/* Turbo C++ - (C) Copyright 1987,1988,1990 by Borland International */

#include "tcalc.h"

char *name = MSGNAME;

/* Moves up 1 row */
void moverowup(void)
{
    displaycell(curcol, currow, NOHIGHLIGHT, NOUPDATE);
    if (currow > toprow)
        currow--;
    else if (toprow != 0) {
        tscroll(DOWN, 1, LEFTMARGIN + 1, 3, 80, SCREENROWS + 2, WHITE);
        displayrow(--toprow, NOUPDATE);
        currow--;
        setbottomrow();
    }
} /* moverowup */

/* Moves down one row */
void moverowdown(void)
{
    displaycell(curcol, currow, NOHIGHLIGHT, NOUPDATE);
    if (currow < bottomrow)
        currow++;
    else if (bottomrow < (MAXROWS - 1)) {
        tscroll(UP, 1, LEFTMARGIN + 1, 3, 80, SCREENROWS + 2, WHITE);
        toprow++;
        currow++;
        setbottomrow();
        displayrow(bottomrow, NOUPDATE);
    }
} /* moverowdown */

/* Moves left one column */
void movecolleft(void)
{
    int col, oldleftcol;
    unsigned char oldcolstart[SCREENCOLS];

    oldleftcol = leftcol;
    movmem(colstart, oldcolstart, sizeof(colstart));
    displaycell(curcol, currow, NOHIGHLIGHT, NOUPDATE);
    if (curcol > leftcol)
        curcol--;
    else if (leftcol != 0) {
        curcol--;
        leftcol--;
        setrightcol();
        setleftcol();
        if (oldleftcol <= rightcol)
        tscroll(RIGHT,colstart[oldleftcol-leftcol]-LEFTMARGIN,LEFTMARGIN+1,3,80,SCREENROWS+2,WHITE);
        clearlastcol();
        for (col = leftcol; col <= oldleftcol - 1; col++)
            displaycol(col, NOUPDATE);
    }
} /* movecolleft */

/* Moves right one column */
void movecolright(void)
{
    int col, oldleftcol, oldrightcol;
    unsigned char oldcolstart[SCREENCOLS];

    movmem(colstart, oldcolstart, sizeof(colstart));
    oldleftcol = leftcol;
    oldrightcol = rightcol;
    displaycell(curcol, currow, NOHIGHLIGHT, NOUPDATE);
    if (curcol < rightcol)
        curcol++;
    else if (rightcol < (MAXCOLS - 1)) {
        curcol++;
        rightcol++;
        setleftcol();
        setrightcol();
        if (oldrightcol >= leftcol)
        tscroll(LEFT,oldcolstart[leftcol-oldleftcol]-LEFTMARGIN,LEFTMARGIN+1,3,80,SCREENROWS+2,WHITE);
        clearlastcol();
        for (col = oldrightcol + 1; col <= rightcol; col++)
            displaycol(col, NOUPDATE);
    }
} /* movecolright */

/* Recalculates all of the numbers in the spreadsheet */
void recalc(void)
{
    int col, row, dummy;

    for (col = 0; col <= lastcol; col++) {
        for (row = 0; row <= lastrow; row++) {
            if ((cell[col][row] != NULL) && (cell[col][row]->attrib == FORMULA))
                cell[col][row]->v.f.fvalue = parse(cell[col][row]->v.f.formula, &dummy);
        }
    }
    displayscreen(UPDATE);
} /* recalc */

/* Changes and prints the current AutoCalc value on the screen */
void changeautocalc(int newmode)
{
    char s[15];

    if (!autocalc && newmode)
        recalc();
    autocalc = newmode;
    if (autocalc)
        strcpy(s, MSGAUTOCALC);
    else
        s[0] = 0;
    writef(73, 1, MSGAUTOCALCCOLOR, strlen(MSGAUTOCALC), s);
} /* autocalc */

/* Changes and prints the current formula display value on the screen */
void changeformdisplay(int newmode)
{
    char s[15];

    formdisplay = newmode;
    if (formdisplay)
        strcpy(s, MSGFORMDISPLAY);
    else
        s[0] = 0;
    writef(67, 1, MSGFORMDISPLAYCOLOR, strlen(MSGFORMDISPLAY), s);
} /* autocalc */

/* Edits a selected cell */
void editcell(CELLPTR ecell)
{
    char s[MAXINPUT + 1];

    if (ecell == NULL)
        return;
    switch(ecell->attrib) {
        case TEXT :
            strcpy(s, ecell->v.text);
            break;
        case VALUE :
            if (ecell->v.value == HUGE_VAL)
                strcpy(s, "0");
            else
                sprintf(s, "%.*f", MAXPLACES, ecell->v.value);
            break;
        case FORMULA :
            strcpy(s, ecell->v.f.formula);
            break;
    } /* switch */
    if (!editstring(s, "", MAXINPUT) || (s[0] == 0))
        return;
    act(s);
    changed = TRUE;
} /* editcell */

/* Clears the current spreadsheet */
void clearsheet(void)
{
    int col, row;

    for (row = 0; row <= lastrow; row++) {
        for (col = 0; col <= lastcol; col++)
            deletecell(col, row, NOUPDATE);
    }
    initvars();
    setrightcol();
    setbottomrow();
    displayscreen(NOUPDATE);
    printfreemem();
    changed = FALSE;
} /* clearsheet */

struct CELLREC rec;

/* Loads a new spreadsheet */
void loadsheet(char *filename)
{
    int allocated,col;
    FILE* file;
    const int MAXBUFLEN=255;
    char buf[MAXBUFLEN+1];

    if (filename[0] == 0) {
        writeprompt(MSGFILENAME);
        if (!editstring(filename, "", MAXINPUT))
            return;
    }
    if (access(filename, 0)) {
        errormsg(MSGNOEXIST);
        return;
    }
    if ((file = fopen(filename,"rb"))==0) {
        errormsg(MSGNOOPEN);
        return;
    }
    fgets(buf,MAXBUFLEN,file);
    if (strncmp(buf, name,strlen(name)) != 0) {
        errormsg(MSGNOTURBOCALC);
        fclose(file);
        return;
    }
    writef(1, 25, PROMPTCOLOR, 79, MSGLOADING);
    gotoxy(strlen(MSGLOADING) + 1, 25);
    clearsheet();

    /* read the max dimensions */
    fgets(buf,MAXBUFLEN,file);
    sscanf(buf,"%d %d",&lastcol,&lastrow);
    lastcol--;
    lastrow--;

    for (col=0; col<=lastcol; col++) {
        int dmy,width;
        fgets(buf,MAXBUFLEN,file);
        sscanf(buf,"%d width %d",&dmy,&width);
        colwidth[col]=width;
    }
    while (1) {
        int col,row;
        char fmt,what;
        struct CELLREC rec;
        setmem(&rec,sizeof(struct CELLREC),0);
        if (fgets(buf,MAXBUFLEN,file)==NULL)
            break;
        /* this allows us to resync if garbage occurs */
        if (strncmp(buf,"---",3)!=0)
            continue;
        /* throw the sync line away */
        fgets(buf,MAXBUFLEN,file);
        sscanf(buf,"%d %d %c %c",&col,&row,&fmt,&what);
        format[col][row]=fmt;
        switch (what) {
            case 'F':
                rec.attrib=FORMULA;
                fgets(buf,MAXBUFLEN,file);
                rec.v.f.fvalue=strtod(buf,NULL);
                fgets(buf,MAXBUFLEN,file);
                strncpy(rec.v.f.formula,buf,strlen(buf)-1);
                allocated = allocformula(col, row, rec.v.f.formula, rec.v.f.fvalue);
                break;
            case 'T':
                rec.attrib=TEXT;
                fgets(buf,MAXBUFLEN,file);
                strncpy(rec.v.text,buf,strlen(buf)-1);
                if ((allocated = alloctext(col, row, rec.v.text)) == TRUE)
                        setoflags(curcol, currow, NOUPDATE);
                break;
            case 'V':
                rec.attrib=VALUE;
                fgets(buf,MAXBUFLEN,file);
                rec.v.value=strtod(buf,NULL);
                allocated = allocvalue(col, row, rec.v.value);
                break;
            default:
                allocated=1;
                /* no idea what we got... drop it */
        }
        if (!allocated) {
            errormsg(MSGFILELOMEM);
            break;
        }
    }
    fclose(file);
    writef(1, 25, WHITE, strlen(MSGLOADING), "");
    gotoxy(1, 25);
    printfreemem();
    curcol = currow = 0;
    setrightcol();
    displayscreen(NOUPDATE);
    changed = FALSE;
} /* loadsheet */

/* Saves the current spreadsheet */
void savesheet(void)
{
    char filename[MAXINPUT+1];
    FILE* file;
    int col, row, overwrite;
    CELLPTR cellptr;

    filename[0] = 0;
    writeprompt(MSGFILENAME);
    if (!editstring(filename, "", MAXINPUT))
        return;
    if (!access(filename, 0)) {
        if (!getyesno(&overwrite, MSGOVERWRITE) || (overwrite == 'N'))
            return;
    }
    if ((file = fopen(filename,"wb"))==0) {
        errormsg(MSGNOOPEN);
        return;
    }
    writef(1, 25, PROMPTCOLOR, 79, MSGSAVING);
    gotoxy(strlen(MSGSAVING) + 1, 25);

    fprintf(file,"%s\n",name);

    fprintf(file,"%d %d\n",lastcol+1,lastrow+1);

    for (col = 0; col<=lastcol; col++) {
        fprintf(file,"%d width %d\n",col+1,colwidth[col]);
    }

    for (row = 0; row <= lastrow; row++) {
        for (col = lastcol; col >= 0; col--) {
            if (cell[col][row] != NULL) {
                cellptr = cell[col][row];
                fprintf(file,"%s\n","---");
                switch(cellptr->attrib) {
                    case TEXT :
                        fprintf(file,"%d %d %c T\n",col,row,format[col][row]);
                        fprintf(file,"%s\n",cellptr->v.text);
                        break;
                    case VALUE :
                        fprintf(file,"%d %d %c V\n",col,row,format[col][row]);
                        fprintf(file,"%.6f\n",cellptr->v.value);
                        break;
                    case FORMULA :
                        fprintf(file,"%d %d %c F\n",col,row,format[col][row]);
                        fprintf(file,"%.6f\n",cellptr->v.f.fvalue);
                        fprintf(file,"%s\n",cellptr->v.f.formula);
                        break;
                } /* switch */
            }
        }
    }
    fclose(file);
    writef(1, 25, WHITE, strlen(MSGSAVING), "");
    gotoxy(1, 25);
    changed = FALSE;
} /* savesheet */

/* Returns the number of rows to print */
int pagerows(int row, int toppage, int border)
{
    int rows;

    rows = toppage ? 66 - TOPMARGIN : 66;
    if (border)
        rows--;
    if (row + rows - 1 > lastrow)
        return(lastrow - row + 1);
    else
        return(rows);
} /* pagerows */

/* Returns the number of columns to print starting at col */
int pagecols(int col, int border, int columns)
{
    int len = ((col == 0) && (border)) ? columns - LEFTMARGIN : columns;
    int firstcol = col;

    while ((len > 0) && (col <= lastcol))
        len -= colwidth[col++];
    if (len < 0)
        col--;
    return(col - firstcol);
} /* pagecols */

/* Prints a copy of the spreadsheet to a file or to the printer */
void printsheet(void)
{
    char filename[MAXINPUT + 1], s[133], colstr[MAXCOLWIDTH + 1];
    FILE *file;
    int columns, counter1, counter2, counter3, col = 0, row, border, toppage, lcol, lrow, dummy, printed, oldlastcol;

    filename[0] = 0;
    writeprompt(MSGPRINT);
    if (!editstring(filename, "", MAXINPUT))
        return;
    if (filename[0] == 0)
        strcpy(filename, "PRN");
    if ((file = fopen(filename, "wt")) == NULL) {
        errormsg(MSGNOOPEN);
        return;
    }
    oldlastcol = lastcol;
    for (counter1 = 0; counter1 <= lastrow; counter1++) {
        for (counter2 = lastcol; counter2 < MAXCOLS; counter2++) {
            if (format[counter2][counter1] >= OVERWRITE)
                lastcol = counter2;
        }
    }
    if (!getyesno(&columns, MSGCOLUMNS))
        return;
    columns = (columns == 'Y') ? 131 : 79;
    if (!getyesno(&border, MSGBORDER))
        return;
    border = (border == 'Y');
    while (col <= lastcol) {
        row = 0;
        toppage = TRUE;
        lcol = pagecols(col, border, columns) + col;
        while (row <= lastrow) {
            lrow = pagerows(row, toppage, border) + row;
            printed = 0;
            if (toppage) {
                for (counter1 = 0; counter1 < TOPMARGIN; counter1++) {
                    fprintf(file, "\n");
                    printed++;
                }
            }
            for (counter1 = row; counter1 < lrow; counter1++) {
                if ((border) && (counter1 == row) && (toppage)) {
                    if ((col == 0) && (border))
                        sprintf(s, "%*s", LEFTMARGIN, "");
                    else
                        s[0] = 0;
                    for (counter3 = col; counter3 < lcol; counter3++) {
                        centercolstring(counter3, colstr);
                        strcat(s, colstr);
                    }
                    fprintf(file, "%s\n", s);
                    printed++;
                }
                if ((col == 0) && (border))
                    sprintf(s, "%-*d", LEFTMARGIN, counter1 + 1);
                else
                    s[0] = 0;
                for (counter2 = col; counter2 < lcol; counter2++)
                    strcat(s, cellstring(counter2, counter1, &dummy, FORMAT));
                fprintf(file, "%s\n", s);
                printed++;
            }
            row = lrow;
            toppage = FALSE;
            if (printed < 66)
                fprintf(file, "%c", FORMFEED);
        }
        col = lcol;
    }
    fclose(file);
    lastcol = oldlastcol;
} /* printsheet */

/* Sets the new column width for a selected column */
void setcolwidth(int col)
{
    int width, row;

    writeprompt(MSGCOLWIDTH);
    if (!getint(&width, MINCOLWIDTH, MAXCOLWIDTH))
        return;
    colwidth[col] = width;
    setrightcol();
    if (rightcol < col) {
        rightcol = col;
        setleftcol();
        setrightcol();
    }
    for (row = 0; row <= lastrow; row++) {
        if ((cell[col][row] != NULL) && (cell[col][row]->attrib == TEXT))
            clearoflags(col + 1, row, NOUPDATE);
        else
            clearoflags(col, row, NOUPDATE);
        updateoflags(col, row, NOUPDATE);
    }
    displayscreen(NOUPDATE);
    changed = TRUE;
} /* setcolwidth */

/* Moves to a selected cell */
void gotocell()
{
    writeprompt(MSGGOTO);
    if (!getcell(&curcol, &currow))
        return;
    leftcol = curcol;
    toprow = currow;
    setbottomrow();
    setrightcol();
    setleftcol();
    displayscreen(NOUPDATE);
} /* gotocell */

/* Prompts the user for a selected format and range of cells */
void formatcells(void)
{
    int col, row, col1, col2, row1, row2, temp, newformat = 0;

    writeprompt(MSGCELL1);
    if (!getcell(&col1, &row1))
        return;
    writeprompt(MSGCELL2);
    if (!getcell(&col2, &row2))
        return;
    if ((col1 != col2) && (row1 != row2))
        errormsg(MSGDIFFCOLROW);
    else {
        if (col1 > col2)
            swap(&col1, &col2);
        if (row1 > row2)
            swap(&row1, &row2);
        if (!getyesno(&temp, MSGRIGHTJUST))
            return;
        newformat += (temp == 'Y') * RJUSTIFY;
        if (!getyesno(&temp, MSGDOLLAR))
            return;
        newformat += (temp == 'Y') * DOLLAR;
        if (!getyesno(&temp, MSGCOMMAS))
            return;
        newformat += (temp == 'Y') * COMMAS;
        if (newformat & DOLLAR)
            newformat += 2;
        else {
            writeprompt(MSGPLACES);
            if (!getint(&temp, 0, MAXPLACES))
                return;
            newformat += temp;
        }
        for (col = col1; col <= col2; col++) {
            for (row = row1; row <= row2; row++) {
                format[col][row] = (format[col][row] & OVERWRITE) | newformat;
                if ((col >= leftcol) && (col <= rightcol) &&
                        (row >= toprow) && (row <= bottomrow))
                    displaycell(col, row, NOHIGHLIGHT, NOUPDATE);
            }
        }
    }
    changed = TRUE;
} /* formatcells */

/* Deletes a column */
void deletecol(int col)
{
    int counter, row;

    for (counter = 0; counter <= lastrow; counter++)
        deletecell(col, counter, NOUPDATE);
    printfreemem();
    if (col != MAXCOLS - 1) {
        movmem(&cell[col + 1][0], &cell[col][0], MAXROWS * sizeof(CELLPTR) * (MAXCOLS - col - 1));
        movmem(&format[col + 1][0], &format[col][0], MAXROWS * (MAXCOLS - col - 1));
        movmem(&colwidth[col + 1], &colwidth[col], MAXCOLS - col - 1);
    }
    setmem(&cell[MAXCOLS - 1][0], MAXROWS * sizeof(CELLPTR), 0);
    setmem(&format[MAXCOLS - 1][0], MAXROWS, DEFAULTFORMAT);
    colwidth[MAXCOLS - 1] = DEFAULTWIDTH;
    if ((lastcol >= col) && (lastcol > 0))
        lastcol--;
    setrightcol();
    if (curcol > rightcol) {
        rightcol++;
        setleftcol();
    }
    clearlastcol();
    for (counter = 0; counter <= lastcol; counter++) {
        for (row = 0; row <= lastrow; row++) {
            if ((cell[counter][row] != NULL) &&
                    (cell[counter][row]->attrib == FORMULA))
                fixformula(counter, row, COLDEL, col);
            updateoflags(col, row, NOUPDATE);
        }
    }
    while (col <= rightcol)
        displaycol(col++, NOUPDATE);
    changed = TRUE;
    recalc();
} /* deletecol */

/* Inserts a column */
void insertcol(int col)
{
    int counter, row;

    if (lastcol == MAXCOLS - 1) {
        for (counter = 0; counter <= lastrow; counter++)
            deletecell(lastcol, counter, NOUPDATE);
        printfreemem();
    }
    if (col != MAXCOLS - 1) {
        movmem(&cell[col][0], &cell[col + 1][0], MAXROWS * sizeof(CELLPTR) * (MAXCOLS - col - 1));
        movmem(&format[col][0], &format[col + 1][0], MAXROWS * (MAXCOLS - col - 1));
        movmem(&colwidth[col], &colwidth[col + 1], MAXCOLS - col - 1);
    }
    setmem(&cell[col][0], MAXROWS * sizeof(CELLPTR), 0);
    setmem(&format[col][0], MAXROWS, DEFAULTFORMAT);
    colwidth[col] = DEFAULTWIDTH;
    lastcol = MAXCOLS - 1;
    setlastcol();
    setrightcol();
    if (curcol > rightcol) {
        rightcol++;
        setleftcol();
    }
    for (counter = 0; counter <= lastcol; counter++) {
        for (row = 0; row <= lastrow; row++) {
            if ((cell[counter][row] != NULL) &&
                    (cell[counter][row]->attrib == FORMULA))
                fixformula(counter, row, COLADD, col);
            updateoflags(col, row, NOUPDATE);
        }
    }
    while (col <= rightcol)
        displaycol(col++, NOUPDATE);
    changed = TRUE;
    recalc();
} /* insertcol */

/* Deletes a row */
void deleterow(int row)
{
    int counter, rowc;

    for (counter = 0; counter <= lastcol; counter++)
        deletecell(counter, row, NOUPDATE);
    printfreemem();
    if (row != MAXROWS - 1) {
        for (counter = 0; counter < MAXCOLS; counter++) {
            movmem(&cell[counter][row + 1], &cell[counter][row],
                    sizeof(CELLPTR) * (MAXROWS - row - 1));
            movmem(&format[counter][row + 1], &format[counter][row], MAXROWS - row - 1);
        }
    }
    else {
        for (counter = 0; counter <= lastcol; counter++) {
            cell[counter][MAXROWS - 1] = NULL;
            format[counter][MAXROWS - 1] = DEFAULTFORMAT;
        }
    }
    if ((lastrow >= row) && (lastrow > 0))
        lastrow--;
    for (counter = 0; counter <= lastcol; counter++) {
        for (rowc = 0; rowc <= lastrow; rowc++) {
            if ((cell[counter][rowc] != NULL) &&
                    (cell[counter][rowc]->attrib == FORMULA))
                fixformula(counter, rowc, ROWDEL, row);
        }
    }
    while (row <= bottomrow)
        displayrow(row++, NOUPDATE);
    changed = TRUE;
    recalc();
} /* deleterow */

/* Inserts a row */
void insertrow(int row)
{
    int counter, rowc;

    if (lastrow == MAXROWS - 1) {
        for (counter = 0; counter <= lastcol; counter++)
            deletecell(counter, lastrow, NOUPDATE);
        printfreemem();
    }
    if (row != MAXROWS - 1) {
        for (counter = 0; counter < MAXCOLS; counter++) {
            movmem(&cell[counter][row], &cell[counter][row + 1], sizeof(CELLPTR) * (MAXROWS - row - 1));
            movmem(&format[counter][row], &format[counter][row + 1], MAXROWS - row - 1);
        }
    }
    for (counter = 0; counter < MAXCOLS; counter++) {
        cell[counter][row] = NULL;
        format[counter][row] = DEFAULTFORMAT;
    }
    lastrow = MAXROWS - 1;
    setlastrow();
    for (counter = 0; counter <= lastcol; counter++) {
        for (rowc = 0; rowc <= lastrow; rowc++) {
            if ((cell[counter][rowc] != NULL) &&
                    (cell[counter][rowc]->attrib == FORMULA))
                fixformula(counter, rowc, ROWADD, row);
        }
    }
    while (row <= bottomrow)
        displayrow(row++, NOUPDATE);
    changed = TRUE;
    recalc();
} /* insertrow */

/* Executes the commands in the spreadsheet menu */
void smenu(void)
{
    char filename[MAXINPUT + 1];

    filename[0] = 0;
    switch(getcommand(SMENU, SCOMMAND)) {
        case 0 :
            checkforsave();
            loadsheet(filename);
            break;
        case 1 :
            savesheet();
            break;
        case 2 :
            printsheet();
            break;
        case 3 :
            checkforsave();
            clearsheet();
            break;
    } /* switch */
} /* smenu */

/* Executes the commands in the column menu */
void cmenu(void)
{
    switch(getcommand(CMENU, CCOMMAND)) {
        case 0 :
            insertcol(curcol);
            break;
        case 1 :
            deletecol(curcol);
            break;
        case 2 :
            setcolwidth(curcol);
            break;
    } /* switch */
} /* cmenu */

/* Executes the commands in the row menu */
void rmenu(void)
{
    switch(getcommand(RMENU, RCOMMAND)) {
        case 0 :
            insertrow(currow);
            break;
        case 1 :
            deleterow(currow);
            break;
    } /* switch */
} /* rmenu */

/* Executes the commands in the utility menu */
void umenu(void)
{
    switch(getcommand(UMENU, UCOMMAND)) {
        case 0 :
            recalc();
            break;
        case 1 :
            changeformdisplay(!formdisplay);
            displayscreen(UPDATE);
            break;
    } /* switch */
} /* umenu */

/* Executes the commands in the main menu */
void mainmenu(void)
{
    switch(getcommand(MENU, COMMAND)) {
        case 0 :
            smenu();
            break;
        case 1 :
            formatcells();
            break;
        case 2 :
            deletecell(curcol, currow, UPDATE);
            printfreemem();
            if (autocalc)
                recalc();
            break;
        case 3 :
            gotocell();
            break;
        case 4 :
            cmenu();
            break;
        case 5 :
            rmenu();
            break;
        case 6 :
            editcell(curcell);
            break;
        case 7 :
            umenu();
            break;
        case 8 :
            changeautocalc(!autocalc);
            break;
        case 9 :
            checkforsave();
            stop = TRUE;
            break;
    } /* switch */
} /* mainmenu */

