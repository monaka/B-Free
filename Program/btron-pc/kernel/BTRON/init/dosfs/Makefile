
HDRS=dosfs.h
SRCS=fat.c dir.c filesystem.c lowdev.c paths.c ls.c cat.c
#OBJS=fat.o dir.o filesystem.o lowdev.o paths.o ls.o cat.o
#OBJS=fat.o dir.o filesystem.o lowdev.o paths.o
OBJS=dos_fs.o fat.o dir.o lowdev.o paths.o


#CFLAGS=-g -DDEBUG
#CFLAGS=-b i486-linuxaout -m386 -DDEBUG -I -I. -I$(ROOT) -I../../kernlib -I../../../ITRON -I../../../ITRON/i386 -I../../../ITRON/h -fno-builtin 
CFLAGS=-Wall -DDEBUG -I -I. -I$(ROOT) -I../../kernlib -I../../../ITRON -I../../../ITRON/i386 -I../../../ITRON/h -fno-builtin 


all: libdosfs.a

libdosfs.a: $(OBJS)
	ar rv libdosfs.a $(OBJS)
	ranlib libdosfs.a

clean:
	rm -f $(OBJS) *~ libdosfs.a

depend:
	cc -M $(CFLAGS) $(SRCS) > .depend

fat.s:
	$(CC) $(CFLAGS) -S fat.c

pack:
	tar cvzf dosfs.tar.gz $(SRCS) $(HDRS) Makefile 00README 

uupack:
	tar cvzf - $(SRCS) $(HDRS) Makefile 00README | uuencode dosfs.tar.gz > dosfs.uu

fat.o: fat.c ../init.h ../../../ITRON/h/types.h \
 ../../../ITRON/h/errno.h ../../../ITRON/ibmpc/ibmpc.h \
 ../../kernlib/device.h ../../kernlib/message.h \
 ../../kernlib/../../ITRON/h/types.h ../../servers/port-manager.h \
 ../../kernlib/libkernel.h ../../device/console/console.h \
 ../../device/console/../../kernlib/device.h \
 ../../device/keyboard/keyboard.h ../../../ITRON/pc9801/pc98.h \
 ../../../ITRON/h/itron.h ../../../ITRON/h/config.h \
 ../../../ITRON/h/../i386/i386.h ../../../ITRON/i386/i386.h \
 ../../../ITRON/h/region.h ../../../ITRON/h/../../../boot/2nd/boot.h \
 ../../device/keyboard/../../kernlib/device.h \
 ../../../POSIX/manager/posix_syscall.h ../file.h dosfs.h
dir.o: dir.c ../init.h ../../../ITRON/h/types.h \
 ../../../ITRON/h/errno.h ../../../ITRON/ibmpc/ibmpc.h \
 ../../kernlib/device.h ../../kernlib/message.h \
 ../../kernlib/../../ITRON/h/types.h ../../servers/port-manager.h \
 ../../kernlib/libkernel.h ../../device/console/console.h \
 ../../device/console/../../kernlib/device.h \
 ../../device/keyboard/keyboard.h ../../../ITRON/pc9801/pc98.h \
 ../../../ITRON/h/itron.h ../../../ITRON/h/config.h \
 ../../../ITRON/h/../i386/i386.h ../../../ITRON/i386/i386.h \
 ../../../ITRON/h/region.h ../../../ITRON/h/../../../boot/2nd/boot.h \
 ../../device/keyboard/../../kernlib/device.h \
 ../../../POSIX/manager/posix_syscall.h ../file.h dosfs.h
filesystem.o: filesystem.c ../init.h ../../../ITRON/h/types.h \
 ../../../ITRON/h/errno.h ../../../ITRON/ibmpc/ibmpc.h \
 ../../kernlib/device.h ../../kernlib/message.h \
 ../../kernlib/../../ITRON/h/types.h ../../servers/port-manager.h \
 ../../kernlib/libkernel.h ../../device/console/console.h \
 ../../device/console/../../kernlib/device.h \
 ../../device/keyboard/keyboard.h ../../../ITRON/pc9801/pc98.h \
 ../../../ITRON/h/itron.h ../../../ITRON/h/config.h \
 ../../../ITRON/h/../i386/i386.h ../../../ITRON/i386/i386.h \
 ../../../ITRON/h/region.h ../../../ITRON/h/../../../boot/2nd/boot.h \
 ../../device/keyboard/../../kernlib/device.h \
 ../../../POSIX/manager/posix_syscall.h ../file.h dosfs.h
lowdev.o: lowdev.c ../init.h ../../../ITRON/h/types.h \
 ../../../ITRON/h/errno.h ../../../ITRON/ibmpc/ibmpc.h \
 ../../kernlib/device.h ../../kernlib/message.h \
 ../../kernlib/../../ITRON/h/types.h ../../servers/port-manager.h \
 ../../kernlib/libkernel.h ../../device/console/console.h \
 ../../device/console/../../kernlib/device.h \
 ../../device/keyboard/keyboard.h ../../../ITRON/pc9801/pc98.h \
 ../../../ITRON/h/itron.h ../../../ITRON/h/config.h \
 ../../../ITRON/h/../i386/i386.h ../../../ITRON/i386/i386.h \
 ../../../ITRON/h/region.h ../../../ITRON/h/../../../boot/2nd/boot.h \
 ../../device/keyboard/../../kernlib/device.h \
 ../../../POSIX/manager/posix_syscall.h ../file.h dosfs.h
paths.o: paths.c
ls.o: ls.c /usr/i486-linuxaout/include/stdio.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/i486-linuxaout/include/libio.h \
 /usr/i486-linuxaout/include/_G_config.h /usr/include/fcntl.h \
 /usr/include/sys/types.h /usr/i486-linuxaout/include/linux/types.h \
 /usr/i486-linuxaout/include/linux/posix_types.h \
 /usr/i486-linuxaout/include/asm/posix_types.h \
 /usr/i486-linuxaout/include/asm/types.h /usr/include/sys/bitypes.h \
 /usr/include/gnu/types.h /usr/i486-linuxaout/include/linux/fcntl.h \
 /usr/i486-linuxaout/include/asm/fcntl.h dosfs.h
cat.o: cat.c ../init.h ../../../ITRON/h/types.h \
 ../../../ITRON/h/errno.h ../../../ITRON/ibmpc/ibmpc.h \
 ../../kernlib/device.h ../../kernlib/message.h \
 ../../kernlib/../../ITRON/h/types.h ../../servers/port-manager.h \
 ../../kernlib/libkernel.h ../../device/console/console.h \
 ../../device/console/../../kernlib/device.h \
 ../../device/keyboard/keyboard.h ../../../ITRON/pc9801/pc98.h \
 ../../../ITRON/h/itron.h ../../../ITRON/h/config.h \
 ../../../ITRON/h/../i386/i386.h ../../../ITRON/i386/i386.h \
 ../../../ITRON/h/region.h ../../../ITRON/h/../../../boot/2nd/boot.h \
 ../../device/keyboard/../../kernlib/device.h \
 ../../../POSIX/manager/posix_syscall.h ../file.h dosfs.h
