#
# B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���
# 
# GNU GENERAL PUBLIC LICENSE
# Version 2, June 1991
# 
# (C) B-Free Project.
#

# �ǥХå��� Define
#
# VMDEBUG	���۵�����Ϣ
# MMDEBUG	���������Ϣ
# PMDEBUG	�ץ�����������Ϣ
# FMDEBUG	�ե����������Ϣ
#
#DEBUG=-DFMDEBUG -DEXEC_DEBUG -DDEBUG
DEBUG=-DFMDEBUG -DEXEC_DEBUG
IDENT=$(DEBUG) -DKERNEL
INC=-I../../ITRON -I../../ITRON/servers -I../../ITRON/h
CFLAGS=-Wall $(INC) $(IDENT)
CC=gcc

ENTRY=startup
STARTADDR=0x00001000


OBJS=s0.o posix.o version.o request.o filesystem.o memory.o process.o \
	log.o syscall.o global.o vmtree.o exec.o fork.o
SRCS=posix.c version.c request.c filesystem.c memory.c process.c \
	log.c syscall.c global.c vmtree.c exec.c fork.c

KERNLIB=../../ITRON/kernlib/libkernel.a
LIBS=sfs/libsfs.a syscalls/libsyscall_mgr.a $(KERNLIB)

all: posix_manager


posix_manager:	$(OBJS) sfs syscalls
	(cd sfs; make)
	(cd syscalls; make)
	$(LD) -static -o $@ -e $(ENTRY) -Ttext=$(STARTADDR) $(OBJS) $(LIBS)
	size $@

clean:
	rm -f $(OBJS) posix_manager
	(cd sfs; make clean)
	(cd syscalls; make clean)

depend:
	cc -M $(CFLAGS) $(SRCS) > .depend



########################################

-include .depend
