#
#	B-Free OS ºîÀ®ÍÑ makefile
#
#$Id: Makefile,v 1.1 2011/12/27 17:13:35 liu1 Exp $
#

COMPONENT=kernlib posix libc

all: $(COMPONENT)


kernlib: 
	make -C ../BTRON/kernlib

posix: kernlib
	make -C manager

libc: kernlib
	make -C libc

clean:
	make -C libc clean
	make -C manager clean
