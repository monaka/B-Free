# $Revision: 1.1 $
#
#
#

IDENT=-DTIMER_TEST -DI386 -DVGA
DEBUG=-DTSKSW_DEBUG
OPT= -D__KERNEL__=1 -DMODULE -Wall -Wstrict-prototypes -DDEBUG_AHA152 -DAUTOCONF

CPP=/lib/cpp
ROOT=../../../ITRON
#CFLAGS= -b i486-linuxaout -I. -I$(ROOT) -I../../kernlib -fno-builtin $(IDENT)
#CFLAGS= -I. -I$(ROOT) -I../../kernlib -fno-builtin $(IDENT)
CFLAGS= -I. -I$(ROOT) -I$(ROOT)/kernlib -I../include/linux $(IDENT) $(OPT)
CC=gcc
#AS=/usr/local/bin/as
AS=/usr/bin/as
KP=kp -input-code euc
MKDEPEND=$(CC) -M
STARTADDR=0x80100000
ENTRY=start
LIBS=$(ROOT)/kernlib/libkernel.a
#LD=/usr/local/bin/ld
LD=/usr/bin/ld

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(CPP) $(ASFLAGS) $*.s | $(AS) -o $*.o



OBJ1=aha152x.o aha152xlow.o misc.o constants.o vsprintf.o ctype.o hosts.o
SRC1=aha152x.c aha152xlow.c misc.c constants.c vsprintf.c ctype.c hosts.c
OBJ2=ide-scsi.o fdomain.o
SRC2=ide-scsi.c fdomain.c


OBJS=$(OBJ1) $(OBJ2)
SRCS=$(SRC1) $(SRC2)

all: aha152x

aha152x: $(OBJS) $(SRCS) Makefile
	$(LD) -static -o $@ -e ${ENTRY} -Ttext=$(STARTADDR) $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS)
