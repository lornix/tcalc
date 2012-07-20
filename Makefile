#
# Generic compiles-it-all Makefile
#  Loni Nix <lornix@lornix.com>
#    Last modified **********
#
CFLAGS+=-ggdb3
CFLAGS+=-Wall -Wextra -Wundef
CFLAGS+=-Werror
CFLAGS+=-O0
#
# LDFLAGS+=-lm
#
.PHONY: all clean

OBJS=tcdisply tcinput tcommand tcparser tcutil tcalc

all: tcalc

%.o : %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ -c $<

tcalc: tcdisply.o tcinput.o tcommand.o tcparser.o tcutil.o

clean:
	-rm -f *.o $(OBJS)
