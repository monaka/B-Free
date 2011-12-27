# $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/FM/Makefile,v 1.1 2011/12/27 17:13:35 liu1 Exp $
#
#
#

LD=/usr/local/bin/ld.old

# POSIX ファイルマネージャの挙動を指定するマクロ定義
#
# DIRCACHE	ディレクトリキャッシュを使用する
# ROOTDOS	root ファイルシステムは、DOS ファイルシステムを使用す
#		る。
# IDENT=-DDIRCACHE
IDENT=-DROOTDOS

DOSFS_OBJS=dosfs/dosfs.o

OBJS=main.o dircache.o request.o fileops.o $(DOSFS_OBJS)
SRCS=main.c dircache.c request.c fileops.c $(DOSFS_SRCS)
LIBS=../../../../../../kernel/kernlib/libkernel.a
INC = -I../../../../include

CFLAGS = $(INC) $(IDENT)
ENTRY=__main
STARTADDR=0x00001000


all: posix-fm

posix-fm: $(OBJS)
	$(LD) -static -z -o posix-fm -e ${ENTRY} -T $(STARTADDR) $(OBJS) $(LIBS)

dosfs/dosfs.o:
	cc $(CFLAGS) -c -o dosfs/dosfs.o dosfs/dosfs.c

####
install: posix-fm
	cp posix-fm ../../../../../server/posix-fm

clean:
	rm -f $(OBJS)
