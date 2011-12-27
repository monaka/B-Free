
1stboot: 1stboot.s
	as86 -0 -a -o 1stboot.o 1stboot.s
	ld86 -0 -s -o a.out 1stboot.o
	dd if=a.out of=1stboot bs=16w skip=1 count=16 2>/dev/null
	@rm a.out
clean:
	rm -f a.out 1stboot.o 1stboot

test: test.s
	as86 -0 -a -o test.o test.s
	ld86 -0 -s -o a.out test.o
	dd if=a.out of=test bs=16w skip=1 count=16 2>/dev/null
	@rm a.out

