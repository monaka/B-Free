#
#
#
#

GCC = /usr/bin/gcc
GPP = /lib/cpp
GAS = /usr/bin/as
GLD = /usr/bin/ld

GSTRIP = /usr/bin/strip
RM = /bin/rm
#ETAGS = /usr/local/bin/etags
ETAGS = /usr/bin/etags

CSRC=main.c console.c keyboard.c memory.c \
     date.c idt.c cinterrupt.c fd.c \
     misc.c dma.c page.c version.c lib.c ide.c \
     vga.c evaluate.c

SSRC=start32.s io.s interrupt.s
OBJ32 = start32.o main.o console.o keyboard.o memory.o  \
	date.o io.o idt.o interrupt.o cinterrupt.o fd.o \
	misc.o dma.o page.o version.o lib.o ide.o sfs.o \
	vga.o evaluate.o
LIBS =

OBJ16 = start16.o
TOOLS = build_boot2_elf

START_ADDR = 0x9000
CFLAGS=-c -m386 -msoft-float -fno-builtin -Wall -D__LINUX__ -DSUPPORT_144 $(DEBUG) -DIBMPC $(HDBOOT)
LDOPT32 = -n -Ttext=$(START_ADDR) -static -e start32

########################################################################
#
#	2ndboot ...... 最終的に作られる Second BOOT イメージ
#
2ndboot: $(TOOLS) start16 mode32
	./build_boot2_elf start16 mode32 > 2ndboot

#
#	start16 ... 16ビットモードで動く部分
#
start16: start16.s
	as86 -0 -a -o start16.o start16.s
	ld86 -0 -s -o start16 start16.o

mode32: $(OBJ32)
	$(GLD) $(LDOPT32) -o mode32 $(OBJ32) $(LIBS)
	cp mode32 mode32.sym
	$(GSTRIP) mode32


start32.o: start32.s
	$(GPP) start32.s | $(GAS) -o start32.o

io.o:	io.s
	$(GPP) io.s | $(GAS) -o io.o

interrupt.o:	interrupt.s
	$(GPP) interrupt.s | $(GAS) -o interrupt.o

clean:
	$(RM) -f $(OBJ32) $(OBJ16) *.s
	$(RM) -f core mode32 start16 build_boot2 build_boot2_elf mode32.sym
	$(RM) -f 2ndboot 
	$(RM) -f *.c~
	$(RM) -f *.h~
	
tags:
	$(ETAGS) $(CSRC)

mkdepend:
	$(CC) -M -D__LINUX__ $(CSRC) > DEPEND

#######################################################################
#
#		T O O L S
#

build_boot2_elf: build_boot2_elf.c
	$(GCC) -D__LINUX__ -DGCC_USE -o build_boot2_elf build_boot2_elf.c


include DEPEND




