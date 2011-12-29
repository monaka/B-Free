#
#	Makefile for boot image.
#

AS=/bin/as
LD=/bin/ld
GCC=/usr/bin/gcc
#CONVOUT=/usr/local/bin/convout
CFLAGS=-D__LINUX__
IMGS=boottable 2ndboot build

all: image

help:
	@echo "make [option]"
	@echo "  fd or hdd"
	@echo "  image    (fd:  make fd image: default)"
	@echo "  hd_image (hdd: make hdd image)"
	@echo "  clean" 

fd: image
image: ${IMGS}
	sync
	sync
	./build 1st/1stboot 2nd/2ndboot bootimage
	sync
	sync

hdd: hd_image
hd_image:boottable 1stboot_hd 2ndboot build
	sync
	sync
	./build 1st/1stboot_hd 2nd/2ndboot bootimage_hd
	sync
	sync

#fd: image
#	cp bootimage /dev/rfd0b

1stboot:
	cd 1st ; make 1stboot

1stboot_hd:
	cd 1st ; make 1stboot_hd

2ndboot:
	cd 2nd ; make 2ndboot

build: build.c
	$(GCC) -o build build.c
	@echo make build done.

boottable: boottable.x mktable
	./mktable boottable.x boottable

mktable: mktable.c
	$(GCC) $(CFLAGS) -o mktable mktable.c
	@echo	done

clean:
	(cd 1st; make clean)
	(cd 2nd; make clean)
	rm -f build boottable mktable
	rm -f bootimage bootimage_hd log
