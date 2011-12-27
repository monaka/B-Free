
#DEBUG=-DDEBUG

TARGET1=tool

all:
#bfree_fd:
#	(cd kernel/BTRON/make; make)
#bfree_fd: dummy
#	(cd boot; make)
#	(cd kernel/BTRON/make; make)
#	(cd kernel/POSIX/mkfs; make)
#	(cd kernel/POSIX/mkfs; make btron_fd)
#
#all_3fd:
	(cd boot; make)
	cp boot/bootimage ./1st_fd.img
	(cd kernel/BTRON/make; make)
	cp kernel/BTRON/make/btron ./2nd_fd.img
	(cd kernel/POSIX/mkfs; make)
	(cd kernel/POSIX/mkfs; ./mk_img.sh)
	cp kernel/POSIX/mkfs/posix_fd.img ./3rd_fd.img
	(cd contribution; make)

applications:
	(cd contribution/applications/frtm-bf/src; make)

clean:
#	rm -f bfree_fd.img
	(cd boot; make clean)
	(cd kernel/BTRON/make; make clean)
	(cd kernel/POSIX/mkfs; make clean)
	(cd contribution; make clean)
#
#clean_all:
	rm -f 1st_fd.img 2nd_fd.img 3rd_fd.img
	(cd kernel/BTRON/make; make clean)

dummy:
	echo '*dummy*'

