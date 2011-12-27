#
#	B-Free OS ºîÀ®ÍÑ makefile
#
#$Id: Makefile,v 1.1 2011/12/27 17:13:35 liu1 Exp $
#



COMPONENT=itron portmanager kernlib devices init posix MM


all: $(COMPONENT)
	(cd BTRON/make; make)

posix:
	(cd POSIX/; make)

kernlib: 
	(cd BTRON/kernlib; make)

devices:
	(cd BTRON/device; make)

portmanager: kernlib
	(cd BTRON/servers; make)

itron: portmanager
	(cd ITRON/make; make)

init: kernlib
	(cd BTRON/init; make)

MM: kernlib
	(cd BTRON/manager/MM; make)

clean:
	(cd BTRON/kernlib; make clean)
	(cd BTRON/device; make clean)
	(cd BTRON/servers; make clean)
	(cd BTRON/init; make clean)
	(cd BTRON/manager/MM; make clean)
	(cd POSIX/; make clean)
