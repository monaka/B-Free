#
# @(#) $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/servers/Makefile,v 1.1 2011/12/27 17:13:36 liu1 Exp $
#
#

TARGET =	port-manager
LOAD_POINT =	0x00010000
ENTRY =		__main
LD = /usr/local/bin/ld		# ZIMAGE タイプの実行ファイルを作成する
				# ことができる LD を使う。

INC=-I../itron-3.0/h -I../itron-3.0/i386
CFLAGS=-b i486-linuxaout $(INC) -fno-builtin


OBJS=port-manager.o database.o


all: $(TARGET)


$(TARGET): $(OBJS)
	$(LD) -static -z -o $(TARGET) -e $(ENTRY) -T $(LOAD_POINT)  $(OBJS) ../kernlib/libkernel.a	


port-manager.o: port-manager.c
	cc $(CFLAGS) -c port-manager.c

database.o: database.c
	cc $(CFLAGS) -c database.c

clean:
	rm -f $(OBJS)


database.o: port-manager.h

port-manager.o: port-manager.h