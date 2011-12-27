# $Id: Makefile,v 1.1 2011/12/27 17:13:35 liu1 Exp $
# 
# Makefile for init.
#
ROOT=../../../ITRON
CFLAGS=-Wall -I. -I./_include -I$(ROOT) -I$(ROOT)/kernlib -I$(ROOT)/h -I$(ROOT)/i386
CXXFLAGS=$(CFLAGS)
#CFLAGS=-static -g -I. -I./_include -I$(ROOT) -I../../kernlib -I$(ROOT)/h -I$(ROOT)/i386 -D_DEBUG_ON_UNIX

CC=gcc

CMDDIR = ../libMittenCmd/
STACKOBJS=CellPool.o MitnStack.o GarbageCollect.o MitnDict.o
OBJS=builtin.o command.o evaluator.o parse.o math.o
CMDOBJ=$(CMDDIR)command.o $(CMDDIR)graphic.o $(CMDDIR)GraphicOp.o \
	$(CMDDIR)posix-misc.o $(CMDDIR)posix-file.o

#all: libMitten.a

libMitten.a: $(OBJS) $(STACKOBJS) $(CMDOBJ)
	ar rv libMitten.a $(OBJS) $(STACKOBJS) $(CMDOBJ)
	ranlib libMitten.a

CellPool.o: CellMemory.h

GarbageCollect.o: CellMemory.h

MitnStack.o: CellMemory.h

builtin.o: CellMemory.h builtin.c

math.o: CellMemory.h

command.o: commands.inc

$(CMDDIR)command.o: $(CMDDIR)command.c

$(CMDDIR)graphic.o: $(CMDDIR)graphic.c

$(CMDDIR)GraphicOp.o: $(CMDDIR)GraphicOp.c

$(CMDDIR)posix-misc.o: $(CMDDIR)posix-misc.c

$(CMDDIR)posix-file.o: $(CMDDIR)posix-file.c

includes:
	grep "^ER" CellPool.c | gawk '{ printf "extern "; print; print ";"}' > CellPool.h
	grep "^ER" GarbageCollect.c | gawk '{ printf "extern "; print; print ";"}' > GarbageCollect.h
	grep "^ER" MitnDict.c | gawk '{ printf "extern "; print; print ";"}' > MitnDict.h
	grep "^ER" MitnStack.c | gawk '{ printf "extern "; print; print ";"}' > MitnStack.h
	
test: $(STACKOBJS) $(OBJS) 
	gcc $(CFLAGS) -o test1 $(STACKOBJS) $(OBJS)

parse-test: parse.c
	gcc $(CFLAGS) -o parse-test parse.c
	
clean:
	rm -f libMitten.a
	rm -f $(OBJS) $(STACKOBJS) $(CMDOBJ)
	rm -f *~
