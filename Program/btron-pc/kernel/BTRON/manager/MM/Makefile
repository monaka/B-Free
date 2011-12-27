#
# $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/manager/MM/Makefile,v 1.1 2011/12/27 17:13:35 liu1 Exp $
#
#
# $Log: Makefile,v $
# Revision 1.1  2011/12/27 17:13:35  liu1
# Initial Version.
#
# Revision 1.4  1997-07-06 11:54:09  night
# オブジェクトリストから malloc.o を削除。
#
# Revision 1.3  1997/07/02 13:10:31  night
# malloc の修正。
#
# Revision 1.2  1997/06/29 13:18:12  night
# BTRON メモリ manager の最初の登録。
#
# Revision 1.1  1996/10/29  16:19:12  night
# 最初の登録
#
#
#

CPP=/lib/cpp
#IDENT=-DDEBUG
CFLAGS=-I. -I../../../ITRON -I../../../kernlib -fno-builtin $(IDENT)
CC=gcc
MKDEPEND=$(CC) -M
STARTADDR=0x00001000
ENTRY=start
LIBS=../../kernlib/libkernel.a


TARGET=MM


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(CPP) $(ASFLAGS) $*.s | $(AS) -o $*.o


# リンクするオブジェクトファイル
OBJS=memory_manager.o port.o global.o assert.o message.o fs_if.o page_management.o 
SRCS=memory_manager.c port.c global.c assert.c message.c fs_if.c page_management.c 


all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) -static -o $@ -e $(ENTRY) -Ttext=$(STARTADDR) $(OBJS) $(LIBS)


clean:
	rm -f $(OBJS) $(TARGET)


wc:
	wc $(SRCS)
