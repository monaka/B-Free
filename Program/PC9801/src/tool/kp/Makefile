#
#
#
#

TOPDIR=../../..
BINDIR=$(TOPDIR)/bin

CC=/usr/bin/cc
CFLAGS=-I../../include

#
#	UJIS §À —¥π
#
# CFLAGS=-DUJIS


SRCS=main.c kanji.c top_level.c help.c version.c
OBJS=main.o kanji.o top_level.o help.o version.o

GOAL:	kp

kp: $(OBJS)
	$(CC) -o kp $(OBJS)


clean:
	rm -f $(OBJS)

install: kp
	cp kp $(BINDIR)