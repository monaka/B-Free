

#DEBUG=-DDEBUG


INC=../../ITRON/h
BUILDSRC=build.c $(INC)/itron_module.h
#CFLAGS=$(DEBUG) -I$(INC)
CFLAGS=$(DEBUG)

KERNLIB=kernlib
MKERNEL=itron
SERVER=port-manager
DRIVERS=console keyboard fd765a ide ne2000 rs232c beep hcm_pci wconsole \
	pcmcia
LOWLIB=posix_lowlib
MANAGERS=posix_manager
USERLIBS=posix_libc
USERAP=init


PROGRAMS=$(MKERNEL) $(DRIVERS) $(LOWLIB) $(MANAGERS) $(USERLIBS) $(USERAP)

all: $(PROGRAMS) btron 

btron: build config.tab dummy
	./build config.tab btron

build: build.o $(BUILDSRC)
	cc $(CFLAGS) -o build build.o

clean:
	rm -f build build.o btron
	(cd ../../BTRON/device/console; make clean)
	(cd ../../BTRON/device/wconsole; make clean)
	(cd ../../BTRON/device/keyboard; make clean)
	(cd ../../BTRON/device/fd765a; make clean)
	(cd ../../BTRON/device/ide; make clean)
	(cd ../../BTRON/device/ne2000; make clean)
	(cd ../../BTRON/device/rs232c; make clean)
	(cd ../../BTRON/device/beep; make clean)
	(cd ../../BTRON/device/hcm_pci; make clean)
	(cd ../../BTRON/device/pcmcia; make clean)
	(cd ../../ITRON/make; make clean)
	(cd ../../BTRON/init; make clean)
	(cd ../../POSIX/manager; make clean)
	(cd ../../POSIX/lowlib; make clean)
	(cd ../../POSIX/libc; make clean)


dummy:
	echo '*dummy*'

$(DRIVERS):
	(cd ../device/$@; make)

itron:
	(cd ../../ITRON/make; make)

posix_lowlib:
	(cd ../../POSIX/lowlib; make)

posix_manager:
	(cd ../../POSIX/manager; make)

posix_libc:
	(cd ../../POSIX/libc; make)

init:
	(cd ../init; make)

