#
# $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/bin/init/Makefile,v 1.1 2011/12/27 17:13:36 liu1 Exp $
#
#
# $Log: Makefile,v $
# Revision 1.1  2011/12/27 17:13:36  liu1
# Initial Version.
#
# Revision 1.1  1995-08-21 13:22:23  night
# ╨г╫И╓непо©
#
#
#

LD=/usr/local/bin/ld.old

OBJS=init.o fs.o interpret.o misc.o
SRCS=init.c fs.c interpret.c misc.c
LIBS=../../../../../kernel/kernlib/libkernel.a

CFLAGS=-I../../../include


all: init


init: $(OBJS)
	$(LD) -o init -z -e ___main $(OBJS) $(LIBS)


