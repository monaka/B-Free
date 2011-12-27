LOAD_POINT=0x00010000
ENTRY=_Main
INC=-I../itron-3.0/h -I../itron-3.0/i386


echo: echo.o
	ld -static -z -o echo -e $(ENTRY) -T $(LOAD_POINT) echo.o ../kernlib/libkernel.a

echo.o: echo.c
	cc -m386 $(INC) -c echo.c

clean:
	rm -f echo.o echo