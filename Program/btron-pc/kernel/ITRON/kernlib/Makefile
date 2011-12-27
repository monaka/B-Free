#
#
#

# -DDEBUG
COPT=-fno-builtin
CFLAGS=-I../h -I../i386 -I.. -I../servers $(COPT)

OBJS=	call_kernel.o sys_task.o sys_message.o sys_debug.o string.o binary.o \
	port_manager.o message_port.o io.o dma.o sys_errno.o sys_interrupt.o eventflag.o \
	sys_eventflag.o interrupt.o \
	sys_virtual.o \
	console.o \
	sys_misc.o \
	sys_lowlib.o \
	sys_time.o \
	malloc.o


libkernel.a: $(OBJS)
	ar rv libkernel.a $(OBJS)
	ranlib libkernel.a



clean:
	rm -f $(OBJS) libkernel.a
