#
#
#

CPP=/lib/cpp
COPT=-fno-builtin
CFLAGS=-b i486-linuxaout -I../itron-3.0/h -I../itron-3.0/i386 -I../itron-3.0 $(COPT)

%.o: %.s
	$(CPP) $(ASFLAGS) $*.s | $(AS) -o $*.o

OBJS=	call_kernel.o sys_task.o sys_message.o sys_debug.o string.o binary.o \
	port_manager.o message_port.o io.o dma.o sys_errno.o sys_interrupt.o eventflag.o \
	sys_eventflag.o interrupt.o \
	sys_virtual.o \
	console.o


libkernel.a: $(OBJS)
	ar rv libkernel.a $(OBJS)
	ranlib libkernel.a



clean:
	rm -f $(OBJS)