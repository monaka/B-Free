
#
#
#
INCLUDE=-I../../../../include
CC=/usr/bin/gcc
LD=/usr/local/bin/ld
CFLAGS=$(INCLUDE) -b i486-linuxaout -fno-builtin $(IDENT) -I../../../../include -nostdlib 

KERNLIB=../../../../../../kernel/kernlib/libkernel.a 
LIBS=$(KERNLIB)
ENTRY=__main
STARTADDR=0x00001000


OBJS=main.o doit.o proctable.o fork.o
SRCS=main.c doit.c proctable.c fork.c


all: posix-pm

posix-pm: $(OBJS)
	$(LD) -static -z -o posix-pm -e ${ENTRY} -T $(STARTADDR) $(OBJS) $(LIBS)

install: posix-pm
	cp posix-pm ../../../../../server/posix-pm

clean:
	rm -f $(OBJS)
