#
# @(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/servers/Makefile,v 1.1 2011/12/27 17:13:35 liu1 Exp $
#
#

TARGET =	port-manager
LOAD_POINT =	0x00010000
ENTRY =		_main
#LD = /usr/local/bin/ld		# ZIMAGE タイプの実行ファイルを作成する
				# ことができる LD を使う。
LD=/usr/bin/ld

CC=gcc
INC=-I../../ITRON/h -I../../ITRON/i386
CFLAGS=$(INC) -fno-builtin


OBJS=port-manager.o database.o


all: $(TARGET)


$(TARGET): $(OBJS)
	$(LD) -static -o $(TARGET) -e $(ENTRY) -Ttext=$(LOAD_POINT)  $(OBJS) ../kernlib/libkernel.a	


clean:
	rm -f $(OBJS) $(TARGET)


database.o: port-manager.h

port-manager.o: port-manager.h
