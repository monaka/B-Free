#
# $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/manager/Makefile,v 1.1 2011/12/27 17:13:35 liu1 Exp $
#
#
# $Log: Makefile,v $
# Revision 1.1  2011/12/27 17:13:35  liu1
# Initial Version.
#
# Revision 1.3  1999-12-18 15:36:26  kishida0
# $B%3%s%Q%$%i%*%W%7%g%s$,$*$+$7$$$N$rD>$7$?(B
#
# Revision 1.2  1996/10/29 16:19:56  night
# ¥¹¥¿¡¼¥È¥¢¥Ã¥×¥ë¡¼¥Á¥óÍÑ¤Î¥Õ¥¡¥¤¥ë¤òÄÉ²Ã (crt0.s)
#
# Revision 1.1  1996/10/29  15:48:08  night
# ºÇ½é¤ÎÅÐÏ¿
#
#
#

#
# make ¥Þ¥¯¥í¤ÎÄêµÁ
#
CPP=/lib/cpp
#CFLAGS= -b i486-linuxaout -I. -I../../ITRON -I../../kernlib -fno-builtin $(IDENT)
IDENT=-DI386 -DUSE_LOWLIB -fno-builtin

ROOT=../../ITRON
CFLAGS=-Wall -I. -I$(ROOT) -I../../kernlib -I$(ROOT)/h -I$(ROOT)/i386 $(IDENT)

CC=gcc
#AS=/usr/local/bin/as
AS=/usr/bin/as
KP=kp -input-code euc
MKDEPEND=$(CC) -M
STARTADDR=0x80100000
ENTRY=_start
LIBS=../kernlib/libkernel.a FM/libfm.a PM/libpm.a MM/libmm.a
LD=/usr/local/bin/ld

#
# ¥³¥ó¥Ñ¥¤¥ë¥ë¡¼¥ë¤ÎÄêµÁ
#
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(CPP) $(ASFLAGS) $*.s | $(AS) -o $*.o

#
# À¸À®Êª
# 
DIR=FM PM MM
LIBS=FM/libfm.a PM/libpm.a MM/libmm.a ../kernlib/libkernel.a
START=crt0.o
OBJS=main.o

all: manager


manager: $(START) $(OBJS) $(LIBS)
	$(LD) -z -o itron.image -e ${ENTRY} -Ttext $(STARTADDR) $(START) $(OBJS) $(LIBS) 

#
# ³Æ¥Þ¥Í¡¼¥¸¥ã¤ÎºîÀ®
#
FM/libfm.a:
	(cd FM; make libfm.a)

PM/libpm.a:
	(cd PM; make libpm.a)

MM/libmm.a:
	(cd MM; make libmm.a)


#
# ¥á¥¤¥ó¥Æ¥Ê¥ó¥¹ÍÑ¤ÎÆ°ºî¤ÎÄêµÁ
#
clean:
	rm -f $(OBJS) $(START)
