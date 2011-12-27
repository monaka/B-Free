#
#
#
#

GCC = /usr/bin/gcc
##GPP = /usr/bin/cpp
GPP = /lib/cpp
GAS = /usr/local/bin/as
#GLD = /usr/bin/ld

#GCC = /usr/local/bin/gcc
#GPP = /lib/cpp
#GAS = /usr/local/bin/as
GLD = /usr/local/bin/ld.old

GSTRIP = /usr/bin/strip
RM = /bin/rm
ETAGS = /usr/local/bin/etags

CSRC=main.c config.c console.c vram.c keyboard.c memory.c \
     date.c idt.c bcopy.c cinterrupt.c hd.c fd.c printf.c \
     misc.c strings.c dma.c atoi.c file.c page.c version.c 

SSRC=start32.s io.s interrupt.s
OBJ32 = start32.o main.o config.o console.o vram.o keyboard.o memory.o \
	date.o io.o idt.o bcopy.o interrupt.o cinterrupt.o hd.o fd.o   \
	printf.o misc.o strings.o dma.o atoi.o file.o page.o version.o
LIBS =  dos/libdosfs.a

OBJ16 = start16.o
TOOLS = build_boot2

START_ADDR = 0x9000
#START_ADDR = 0x8fe0
# DEBUG=-DPG_DEBUG -DFDDEBUG -DDMADEBUG -DRECORD_DEBUG
CFLAGS=-c -m386 -msoft-float -fno-builtin -D__LINUX__ $(DEBUG)
LDOPT32 = -n -T $(START_ADDR) -static

.c.o:
	$(GCC) $(CFLAGS) $*.c

########################################################################
#
#	2ndboot ...... 最終的に作られる Second BOOT イメージ
#
2ndboot: $(TOOLS) start16 mode32
	build_boot2 start16 mode32 > 2ndboot

#
#	start16 ... 16ビットモードで動く部分
#
start16: start16.s
	as86 -0 -a -o start16.o start16.s
	ld86 -0 -s -o start16 start16.o

mode32: $(OBJ32) libdos
	$(GLD) $(LDOPT32) -o mode32 $(OBJ32) $(LIBS)
	cp mode32 mode32.sym
	$(GSTRIP) mode32
	
libdos:
	(cd dos; make)

start32.o: start32.s
	$(GPP) start32.s | $(GAS) -o start32.o

io.o:	io.s
	$(GPP) io.s | $(GAS) -o io.o

interrupt.o:	interrupt.s
	$(GPP) interrupt.s | $(GAS) -o interrupt.o

#console.o: console.c
#	$(GCC) -c console.c

clean:
	$(RM) -f $(OBJ32) $(OBJ16) core mode32 start16 build_boot2

tags:
	$(ETAGS) $(CSRC)

mkdepend:
	$(CC) -M -D__LINUX__ $(CSRC) > MKDEPEND

#######################################################################
#
#		T O O L S
#

build_boot2: build_boot2.c
	$(GCC) -D__LINUX__ -DGCC_USE -o build_boot2 build_boot2.c


include DEPEND




