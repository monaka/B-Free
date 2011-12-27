#
# B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。
#
# GNU GENERAL PUBLIC LICENSE
# Version 2, June 1991
#
# (C) B-Free Project.
#


all:
	(cd src/tool/kp; make; cp kp ../../../bin)
	(cd src/boot; make)
	(cd src/kernel/kernlib; make)
	(cd src/kernel/itron-3.0/make; make)
	(cd src/kernel/servers; make)
	(cd src/kernel/device; make)
	(cd src/posix/usr/src/sys/server/FM; make; make install)
	(cd src/posix/usr/src/sys/server/MM; make; make install)
	(cd src/posix/usr/src/sys/server/PM; make; make install)
	(cd src/kernel/init; make)
	(cd src/kernel/make; make)

clean:
	(cd src/tool/kp; make clean; cp kp ../../../bin)
	(cd src/boot; make clean)
	(cd src/kernel/kernlib; make clean)
	(cd src/kernel/itron-3.0/make clean; make clean)
	(cd src/kernel/servers; make clean)
	(cd src/kernel/device/console; make clean)
	(cd src/kernel/device/keyboard; make clean)
	(cd src/posix/usr/src/sys/server/FM; make clean)
	(cd src/posix/usr/src/sys/server/MM; make clean)
	(cd src/posix/usr/src/sys/server/PM; make clean)
	(cd src/kernel/init; make clean)
	(cd src/kernel/make; make clean)


