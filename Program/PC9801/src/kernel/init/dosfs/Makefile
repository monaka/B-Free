
HDRS=dosfs.h
SRCS=fat.c dir.c filesystem.c lowdev.c paths.c ls.c cat.c
#OBJS=fat.o dir.o filesystem.o lowdev.o paths.o ls.o cat.o
OBJS=fat.o dir.o filesystem.o lowdev.o paths.o


#CFLAGS=-g -DDEBUG
CFLAGS=-b i486-linuxaout -m386 -DDEBUG -I -I. -I$(ROOT) -I../../kernlib -I../../itron-3.0 -I../../itron-3.0/i386 -I../../itron-3.0/h -fno-builtin


all: libdosfs.a

libdosfs.a: $(OBJS)
	ar rv libdosfs.a $(OBJS)
	ranlib libdosfs.a

clean:
	rm -f $(OBJS) *~ libdosfs.a

depend:
	cc -M $(CFLAGS) $(SRCS) > .depend

pack:
	tar cvzf dosfs.tar.gz $(SRCS) $(HDRS) Makefile 00README 

uupack:
	tar cvzf - $(SRCS) $(HDRS) Makefile 00README | uuencode dosfs.tar.gz > dosfs.uu


fat.o: fat.c /usr/include/stdio.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/i486-linuxaout/include/libio.h \
 /usr/i486-linuxaout/include/_G_config.h /usr/include/fcntl.h \
 /usr/include/sys/types.h /usr/i486-linuxaout/include/linux/types.h \
 /usr/i486-linuxaout/include/asm/types.h \
 /usr/i486-linuxaout/include/gnu/types.h \
 /usr/i486-linuxaout/include/linux/fcntl.h \
 /usr/i486-linuxaout/include/asm/fcntl.h \
 /usr/i486-linuxaout/include/unistd.h /usr/include/posix_opt.h \
 /usr/lib/gcc-lib/i486-linuxaout/2.7.0/include/stddef.h \
 /usr/include/confname.h /usr/include/malloc.h dosfs.h
paths.o: paths.c
ls.o: ls.c /usr/include/stdio.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/i486-linuxaout/include/libio.h \
 /usr/i486-linuxaout/include/_G_config.h /usr/include/fcntl.h \
 /usr/include/sys/types.h /usr/i486-linuxaout/include/linux/types.h \
 /usr/i486-linuxaout/include/asm/types.h \
 /usr/i486-linuxaout/include/gnu/types.h \
 /usr/i486-linuxaout/include/linux/fcntl.h \
 /usr/i486-linuxaout/include/asm/fcntl.h dosfs.h
