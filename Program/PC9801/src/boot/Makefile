#
#	Makefile for boot image.
#

AS=/bin/as
LD=/bin/ld
GCC=/usr/bin/gcc
#CONVOUT=/usr/local/bin/convout
CFLAGS=-D__LINUX__

image: boottable 1stboot 2ndboot build
	sync
	sync
	build bootimage
	sync
	sync

fd: image
	cp bootimage /dev/rfd0b

1stboot:
	cd 1st ; make 1stboot

2ndboot:
	cd 2nd ; make 2ndboot

build: build.c
	$(GCC) -o build build.c
	@echo make build done.

boottable: boottable.x mktable
	mktable boottable.x boottable

mktable: mktable.c
	$(GCC) $(CFLAGS) -o mktable mktable.c
	@echo	done
	
clean:
	(cd 1st; make clean)
	(cd 2nd; make clean)
