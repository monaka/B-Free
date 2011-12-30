#
#
#
#

CC=gcc
GSTRIP = /usr/bin/strip
RM = /bin/rm

CSRC=main.c console.c keyboard.c memory.c \
     date.c idt.c cinterrupt.c fd.c \
     misc.c dma.c page.c version.c lib.c ide.c \
     vga.c evaluate.c

OBJ32= start32.o main.o console.o keyboard.o memory.o  \
	date.o io.o idt.o interrupt.o cinterrupt.o fd.o \
	misc.o dma.o page.o version.o lib.o ide.o sfs.o \
	vga.o evaluate.o
LIBS =

OBJ16 = start16.o
#TOOLS = build_boot2
TOOLS = build_boot2_elf

START_ADDR = 0x9000
#START_ADDR = 0x8fe0
# DEBUG=-DPG_DEBUG -DFDDEBUG -DDMADEBUG -DRECORD_DEBUG -DIDEDEBUG
# HDBOOT=-DHDBOOT -DBOOTOS=0x010001
COMMON = ../../include
ITRON = ../../kernel/ITRON
INCPATH = -I. -I$(COMMON) -I$(ITRON)

CFLAGS=-c -msoft-float -fno-builtin -Wall -D__LINUX__ -DSUPPORT_144 $(DEBUG) -DIBMPC $(HDBOOT) $(INCPATH)
#CFLAGS=-c -m386 -fno-builtin -msoft-float -D__LINUX__ -DSUPPORT_144 $(DEBUG) -DIBMPC
LDOPT32 = -n -Ttext=$(START_ADDR) -static -e start32

########################################################################
#
#	2ndboot ...... 最終的に作られる Second BOOT イメージ
#
2ndboot: $(TOOLS) start16 mode32
	./build_boot2_elf start16 mode32 > 2ndboot

#2ndboot: $(TOOLS) start16 mode32
#	build_boot2 start16 mode32 > 2ndboot


#
#	start16 ... 16ビットモードで動く部分
#
start16: start16.s
	as -a -o start16.o start16.s
	ld -e second_boot -o start16 start16.o

mode32: $(OBJ32)
	$(LD) $(LDOPT32) -o mode32 $(OBJ32) $(LIBS)
	cp mode32 mode32.sym
	$(GSTRIP) mode32


clean:
	$(RM) -f $(OBJ32) $(OBJ16) *.s
	$(RM) -f core mode32 start16 build_boot2 build_boot2_elf mode32.sym
	$(RM) -f 2ndboot 

tags:
	$(ETAGS) $(CSRC)

mkdepend:
	$(CC) -M -D__LINUX__ $(CSRC) > DEPEND

#######################################################################
#
#		T O O L S
#

build_boot2: build_boot2.c
	$(CC) -D__LINUX__ -DGCC_USE -o build_boot2 build_boot2.c

build_boot2_elf: build_boot2_elf.c
	$(CC) -D__LINUX__ -DGCC_USE -o build_boot2_elf build_boot2_elf.c


include DEPEND




