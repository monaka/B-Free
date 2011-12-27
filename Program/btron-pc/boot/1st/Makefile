#
# $Revision: 1.1 $
# $Date: 2011/12/27 17:13:35 $
#
#
help:
	@echo "make [option]"
	@echo "  fd or hdd"
	@echo "  1stboot    (fd boot: default)"
	@echo "  1stboot_hd (hdd boot)"
	@echo "  clean"

fd: 1stboot
1stboot: 1stboot.s
	as86 -0 -a -o 1stboot.o 1stboot.s
	ld86 -0 -s -o a.out 1stboot.o
	dd if=a.out of=1stboot bs=32 skip=1 count=16 2>/dev/null
	@rm a.out

hdd: 1stboot_hd
1stboot_hd:1stboot_hd.s
	as86 -0 -a -o 1stboot_hd.o 1stboot_hd.s
	ld86 -0 -s -o a.out 1stboot_hd.o
	dd if=a.out of=1stboot_hd bs=32 skip=1 count=16 2>/dev/null
	@rm a.out

clean:
	rm -f 1stboot.o 1stboot.s 1stboot
	rm -f 1stboot_hd.o 1stboot_hd.s 1stboot_hd
	rm -f a.out

test: test.s
	as86 -0 -a -o test.o test.s
	ld86 -0 -s -o a.out test.o
	dd if=a.out of=test bs=16w skip=1 count=16 2>/dev/null
	@rm a.out

