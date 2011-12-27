#
# POSIX メモリマネージャのための Makefile.
#
#

#
#
#
INCLUDE=-I../../../../include
CC=/usr/bin/gcc
LD=/usr/local/bin/ld.old
CFLAGS=$(INCLUDE) 

OBJS=main.o doit.o pageinfo.o
KERNLIB=../../../../../../kernel/kernlib/libkernel.a 
LIBS=$(KERNLIB)
ENTRY=__main
STARTADDR=0x00001000

all: posix-mm

posix-mm: $(OBJS)
	$(LD) -static -z -o posix-mm -e ${ENTRY} -T $(STARTADDR) $(OBJS) $(LIBS)

install: posix-mm
	cp posix-mm ../../../../../server/posix-mm

clean:
	rm $(OBJS)

