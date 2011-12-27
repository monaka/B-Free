
HDRS=dosfs.h
SRCS=fat.c dir.c filesystem.c lowdev.c paths.c ls.c cat.c
OBJS=fat.o dir.o filesystem.o lowdev.o paths.o ls.o cat.o


#CFLAGS=-g -DDEBUG
CFLAGS=-g -m386 -D__BOOT__ -DDEBUG


all: libdosfs.a

libdosfs.a: $(OBJS)
	ar rv libdosfs.a $(OBJS)
	ranlib libdosfs.a

clean:
	rm -f $(OBJS) *~

depend:
	cc -M $(SRCS) > .depend

pack:
	tar cvzf dosfs.tar.gz $(SRCS) $(HDRS) Makefile 00README 

uupack:
	tar cvzf - $(SRCS) $(HDRS) Makefile 00README | uuencode dosfs.tar.gz > dosfs.uu


fat.o: fat.c /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/libio.h /usr/include/_G_config.h \
  /usr/include/fcntl.h /usr/include/sys/types.h /usr/include/linux/types.h \
  /usr/include/asm/types.h /usr/include/gnu/types.h \
  /usr/include/linux/fcntl.h /usr/include/unistd.h /usr/include/posix_opt.h \
  /usr/lib/gcc-lib/i486-linux/2.6.3/include/stddef.h \
  /usr/include/confname.h /usr/include/malloc.h dosfs.h
dir.o: dir.c /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/libio.h /usr/include/_G_config.h \
  /usr/include/fcntl.h /usr/include/sys/types.h /usr/include/linux/types.h \
  /usr/include/asm/types.h /usr/include/gnu/types.h \
  /usr/include/linux/fcntl.h /usr/include/string.h \
  /usr/lib/gcc-lib/i486-linux/2.6.3/include/stddef.h dosfs.h
filesystem.o: filesystem.c /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/libio.h /usr/include/_G_config.h \
  /usr/include/fcntl.h /usr/include/sys/types.h /usr/include/linux/types.h \
  /usr/include/asm/types.h /usr/include/gnu/types.h \
  /usr/include/linux/fcntl.h dosfs.h
lowdev.o: lowdev.c ../types.h ../file.h ../errno.h dosfs.h
paths.o: paths.c /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/libio.h /usr/include/_G_config.h \
  /usr/include/strings.h /usr/include/string.h \
  /usr/lib/gcc-lib/i486-linux/2.6.3/include/stddef.h
ls.o: ls.c /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/libio.h /usr/include/_G_config.h \
  /usr/include/fcntl.h /usr/include/sys/types.h /usr/include/linux/types.h \
  /usr/include/asm/types.h /usr/include/gnu/types.h \
  /usr/include/linux/fcntl.h dosfs.h
cat.o: cat.c /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/libio.h /usr/include/_G_config.h \
  /usr/include/fcntl.h /usr/include/sys/types.h /usr/include/linux/types.h \
  /usr/include/asm/types.h /usr/include/gnu/types.h \
  /usr/include/linux/fcntl.h /usr/include/string.h \
  /usr/lib/gcc-lib/i486-linux/2.6.3/include/stddef.h dosfs.h
