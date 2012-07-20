#
# Generic compiles-it-all Makefile
#  Loni Nix <lornix@lornix.com>
#    Last modified **********
#
CFLAGS+=-ggdb3
CFLAGS+=-Wall -Wextra -Wunused
CFLAGS+=-Werror
CFLAGS+=-O0
# CFLAGS+=-nostdlib
#
LDFLAGS+=-lm
LDFLAGS+=-lncurses
# LDFLAGS+=-lgcc
#
.PHONY: all clean

all: tcalc tags core

tcalc: tcdisply.o tcinput.o tcommand.o tcparser.o tcutil.o tcalc.o compat.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

tags:
	@ctags .

core:
	@touch core
	@-rm core

%.o : %.c tcalc.h compat.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ -c $<

clean:
	@touch tags core tcalc
	@-rm -f *.o tcalc core tags
