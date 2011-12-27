#
#
#

IDENT=-DTIMER_TEST

CPP=/lib/cpp
CFLAGS=-I. -fno-builtin $(IDENT) -b i486-linuxaout
CC=/usr/bin/gcc
MKDEPEND=$(CC) -M
STARTADDR=0x80010000


%.o: %.c
	kp -input $< -output kptmp.c
	$(CC) $(CFLAGS) -c kptmp.c -o $@
	rm -f kptmp.c

%.o: %.s
	$(CPP) $(ASFLAGS) $*.s | $(AS) -c -o $*.o

#OBJS=startup.o main.o memory.o task.o version.o error.o dummy.o printf.o 
OBJS=startup.o main.o task.o version.o error.o dummy.o printf.o \
     console.o io.o interrupt.o fault.o locore.o syscall.o misc.o timer.o \
     virtual_memory.o itron_memory.o pmemory.o kalloc.o

SRCS=main.c memory.c task.c version.c error.c dummy.c printf.c \
     console.c fault.c syscall.c misc.c timer.c \
     virtual_memory.c itron_memory.c pmemory.c kalloc.c

ASRCS=startup.s io.s interrupt.s locore.s

all: itron.image

itron.image: $(OBJS)
	$(LD) -static -z -o itron.image -T $(STARTADDR) $(OBJS)

tags:
	etags *.c

depend:
	$(MKDEPEND) -x c $(SRCS) > .depend

clean:
	rm -f $(OBJS)


main.o : main.c itron.h types.h config.h i386.h version.h errno.h task.h func.h 
memory.o : memory.c itron.h types.h config.h i386.h memory.h errno.h func.h \
  task.h pc98.h 
task.o : task.c itron.h types.h config.h i386.h errno.h task.h misc.h func.h 
version.o : version.c itron.h types.h config.h i386.h version.h 
error.o : error.c itron.h types.h config.h i386.h errno.h version.h func.h task.h 
dummy.o : dummy.c itron.h types.h config.h i386.h 
printf.o : printf.c itron.h types.h config.h i386.h func.h task.h 
console.o : console.c itron.h types.h config.h i386.h errno.h func.h task.h \
  misc.h 
fault.o : fault.c itron.h types.h config.h i386.h pc98.h interrupt.h errno.h \
  task.h 
syscall.o : syscall.c itron.h types.h config.h i386.h errno.h func.h task.h 
misc.o : misc.c itron.h types.h config.h i386.h 
timer.o : timer.c itron.h types.h config.h i386.h pc98.h interrupt.h 
virtual_memory.o : virtual_memory.c i386.h types.h itron.h config.h misc.h func.h \
  task.h 
itron_memory.o : itron_memory.c itron.h types.h config.h i386.h task.h errno.h \
  func.h misc.h 
