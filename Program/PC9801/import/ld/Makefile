# Makefile for GNU binary-file utilities
# Select a set of CFLAGS and PROGS, below, depending on the system type.
# Copyright (C) 1989, 1990 Free Software Foundation, Inc.
#
# This file is part of the GNU binutils.
# 
# The GNU binutils are free software; you can redistribute them and/or modify
# them under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 1, or (at your option)
# any later version.
# 
# The GNU binutils are distributed in the hope that they will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with the GNU binutils; see the file COPYING.  If not, write to
# the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

# gprof.c contains `void *' and very long string constants which many
# C compilers can't handle, so you might need gcc to compile it.
GNUCC = $(CC)
# But you might need to use your old C compiler to compile the other
# programs (ld at least), if you can't run gcc without them
# (particularly on COFF systems).
CC = gcc #-V 2.5.7

# Where you want to put them.
bindir=/usr/bin

# For Linux

archpfx=l-

# C++ demangler
CPLUS_DEM = cp-dem.o

HOSTNAME=linux
#HOSTNAME=i386-att-sysv
#HOSTNAME=decstation
#HOSTNAME=sparc-sun-sunos4.1


ifeq "$(HOSTNAME)" "linux"

# Those 4 are not needed for Linux.
GNU_GETOPT = 
GNU_GETOPT_LONG =
SIGNAME =
CPLUS_DEM = 

#LDFLAGS= -liberty

# you can define AR_H and A_OUT_H for the header files you want to
# includes.
HEADERS=-DAR_H=\<ar.h\> -DA_OUT_H=\<a.out.h\>

STANDARD_SEARCH_DIRS=\"/usr/local/lib\",\"/usr/ucb/lib\",\"/usr/lib\",\"/usr/X386/lib\"

XCFLAGS= -DSHORT_FILENAME -DHAVE_TRAILING_SLASH_IN_NAME
XCFLAGS= -DHAVE_TRAILING_SLASH_IN_NAME \
	-DSTDC_HEADERS -DSTANDARD_SEARCH_DIRS="$(STANDARD_SEARCH_DIRS)"

OFLAGS= -Wall -O2 -fomit-frame-pointer

#MISCFLAGS= -b i486-linux

PROGS = $(archpfx)gprof $(archpfx)ld $(archpfx)size \
        $(archpfx)nm $(archpfx)strip $(archpfx)ar $(archpfx)ranlib \
        $(archpfx)objdump

else


ifeq "$(HOSTNAME)" "i386-att-sysv"

HOST_ROOT=/local/i486-linux

MISCFLAGS= -shlib -DUSG -DSYS_SIGLIST_MISSING -DSHORT_FILENAM

endif

ifeq "$(HOSTNAME)" "decstation"

HOST_ROOT=/net/ren/users/hlu/local/i486-linux

MISCFLAGS= -Di386 -DHAVE_TRAILING_SLASH_IN_NAME

endif

ifeq "$(HOSTNAME)" "sparc-sun-sunos4.1"

HOST_ROOT=/usr/gnu/i486-linux

MISCFLAGS= -Di386 -DHAVE_TRAILING_SLASH_IN_NAME \
	-DHOST_TARGET_ENDIANESS_DIFFERS -Usparc -Usun

MDOBJS = biglittle.o

OFLAGS= -Wall -O2

endif

# where get those header files.
AR_H=linux/ar.h
A_OUT_H=linux/a.out.h
HEADERS=-I./include -DAR_H=\"$(AR_H)\" -DA_OUT_H=\"$(A_OUT_H)\"

CROSS_AR=$(HOST_ROOT)/bin/ar

CROSS_FLAGS=-DCROSS_AR=\"$(CROSS_AR)\"

STANDARD_SEARCH_DIRS=\"$(HOST_ROOT)/lib\",\"$(HOST_ROOT)/X386/lib\"

XCFLAGS=-D__linux__ -DCROSS_LINKER -DNON_NATIVE $(CROSS_FLAGS) \
	-DSTANDARD_SEARCH_DIRS="$(STANDARD_SEARCH_DIRS)"
XCFLAGS=-DCROSS_LINKER -DNON_NATIVE $(CROSS_FLAGS) \
	-DSTANDARD_SEARCH_DIRS="$(STANDARD_SEARCH_DIRS)"

GNU_GETOPT = getopt.o
GNU_GETOPT_LONG = getopt.o getopt1.o
SIGNAME = signame.o

PROGS = $(archpfx)ld $(archpfx)size \
        $(archpfx)nm $(archpfx)strip $(archpfx)ar $(archpfx)ranlib \
        $(archpfx)objdump

endif

CFLAGS= $(MISCFLAGS) $(XCFLAGS) $(HEADERS) $(OFLAGS)

# For BSD:
# For SunOS 4.x, take out -DVPRINTF_MISSING.
#CFLAGS = -g -DVPRINTF_MISSING
# Don't add robotussin; it won't compile on BSD or GNU systems.
# objdump is not here because it (at least used to) not compile
# on most systems (trouble with N_DATADDR).  Some of those problems
# have been fixed, though.
#PROGS = $(archpfx)gprof $(archpfx)ld $(archpfx)size \
#        $(archpfx)nm $(archpfx)strip $(archpfx)ar $(archpfx)ranlib

# For USG systems using COFF_ENCAPSULATE:
# Also, you will want to make the target libc.a (but it takes a long time).
# Note that you should leave a copy of `ar' in this directory
# after you install it, since `ranlib' will try to run it from here.
#CFLAGS=-g -DUSG -DCOFF_ENCAPSULATE -DPORTAR -DNON_NATIVE -DSYS_SIGLIST_MISSING
#PROGS = ld size nm strip ar robotussin objdump ranlib gprof
# On ALTOS systems, add -DALTOS to CFLAGS.

#It's better to move a copy of alloca into your libc than to risk getting some
#incompatible functions from -lPW (like index()), but if you
#want to be lazy, uncomment this line.
#ALLOCALIBS = -lPW

# For HP-UX:
# Don't add `robotussin'; use `hpxt' instead.
# Note that you should leave a copy of `ar' in this directory
# after you install it, since `ranlib' will try to run it from here.
#CFLAGS = -g -Ihp-include -DUSG -DNON_NATIVE -DVPRINTF_MISSING
#PROGS = ld size nm strip ar ranlib
#ALLOCALIBS = alloca.o

# For the NeXT:
# Set GNUCC = cc -O.
# Put -DMACH_O in CFLAGS, take out -DVPRINTF_MISSING.
# Comment out MALLOC below to use the system's malloc().

# For Sun386:
# Compile with -DPORTAR -DCOFF_ENCAPSULATE.
# Depending on how you configure gcc, you might also want -Dnounderscore,
# though I did not wind up using it.

# If you run out of stack space while running GNU ar or GNU ld (this
# manifests itself as a segment violation), you should link in alloca.c
# ld.c; or you could try to get Sun to fix this annoying "feature".
#CFLAGS = -g -DCOFF_ENCAPSULATE -DPORTAR
#PROGS = ld size nm strip ar robotussin objdump ranlib gprof

# nm tries to malloc enough space for the string table.  The old GNU malloc
# rounds this up to a power of two (e.g. 5M becomes 8M), and so it might 
# fail unnecessarily.  I've also seen some Unix malloc's fail, even when
# there is enough memory.  So use the new GNU malloc.
#MALLOC = $(archpfx)gmalloc.o

# C++ demangler
#CPLUS_DEM = cp-dem.o

LIBS=$(ALLOCALIBS) $(SIGNAME) $(MDOBJS)

all: $(PROGS)
	mv $(archpfx)ar ar
	if [ -f $(archpfx)gprof ]; then \
	  mv $(archpfx)gprof gprof; \
	else true; fi
	mv $(archpfx)ld ld
	mv $(archpfx)nm nm
	mv $(archpfx)objdump objdump
	mv $(archpfx)ranlib ranlib
	mv $(archpfx)size size

$(archpfx)ld: ld.o $(GNU_GETOPT_LONG) $(CPLUS_DEM) $(LIBS)
# LIBS is used here since ld needs to use alloca.
# Alternatively, compile it with GNU C--then the compiler handles alloca.
	$(CC) $(CFLAGS) -o $(archpfx)ld ld.o $(GNU_GETOPT_LONG) $(CPLUS_DEM) $(LIBS) $(LDFLAGS)

$(archpfx)size: size.o
	$(CC) $(CFLAGS) -o $(archpfx)size size.o $(MDOBJS) $(LDFLAGS)

$(archpfx)nm: nm.o $(GNU_GETOPT_LONG) $(CPLUS_DEM) $(MALLOC) $(LIBS)
	$(CC) $(CFLAGS) -o $(archpfx)nm nm.o \
		$(GNU_GETOPT_LONG) $(CPLUS_DEM) $(LIBS) $(MALLOC) \
		$(LDFLAGS)

$(archpfx)strip: strip.o error.o $(GNU_GETOPT_LONG) $(LIBS)
	$(CC) $(CFLAGS) -o $(archpfx)strip strip.o error.o $(GNU_GETOPT_LONG) $(LIBS) $(LDFLAGS)

$(archpfx)ar: ar.o $(LIBS)
	$(CC) $(CFLAGS) -o $(archpfx)ar ar.o $(LIBS) $(LDFLAGS)

$(archpfx)gprof: gprof.o $(GNU_GETOPT_LONG) $(CPLUS_DEM) $(LIBS)
	$(GNUCC) $(CFLAGS) -o $(archpfx)gprof gprof.o \
		$(GNU_GETOPT_LONG) $(CPLUS_DEM) $(LIBS) $(LDFLAGS)
gprof.o: gprof.c gmon.h
	$(GNUCC) $(CFLAGS) -c gprof.c $(OUTPUT_OPTION)

$(archpfx)ranlib: ranlib.o $(GNU_GETOPT_LONG) $(LIBS)
	$(CC) $(CFLAGS) -o $(archpfx)ranlib ranlib.o $(GNU_GETOPT_LONG) $(LIBS) $(LDFLAGS)
ranlib.o: ranlib.c
	$(CC) -c $(CFLAGS) -DAR_PROG=\"$(bindir)/ar\" ranlib.c $(OUTPUT_OPTION)

$(archpfx)objdump: objdump.o error.o $(GNU_GETOPT_LONG) $(LIBS)
	$(CC) $(CFLAGS) -o $(archpfx)objdump objdump.o error.o \
		$(GNU_GETOPT_LONG) $(LIBS) $(LDFLAGS)

# usg-gnulib is the file gcc makes using the usg compiler
gnulib: $(archpfx)robotussin libconvert usg-gnulib $(archpfx)ar
	libconvert usg-gnulib gnulib

clean:
	-rm -f *.o core
	-rm -f $(PROGS)
	-rm -f ar ld ranlib nm objdump size strip gprof

DISTNAME = binutils-1.9l.2
dist:
	rm -rf $(DISTNAME)
	mkdir $(DISTNAME) $(DISTNAME)/hp-bin $(DISTNAME)/hp-include
	ln COPYING ChangeLog Makefile README README-ENCAP \
	a.out.encap.h a.out.gnu.h ar.c cp-dem.c \
	error.c getopt.c getopt.h getopt1.c gmalloc.c \
	gmon.h gprof.c gprof.texinfo ld.c libconvert nm.c objdump.c \
	ranlib.c ranlib.h robotussin.c signame.c signame.h size.c \
	stab.def stab.h strip.c symseg.h $(DISTNAME)
	cd hp-bin; ln Makefile chatr.c hpxt.c ioutil.c ioutil.h \
	mkhplib ../$(DISTNAME)/hp-bin
	cd hp-include; ln a.out.h stab.def stab.h ../$(DISTNAME)/hp-include
	tar chZf $(DISTNAME).tar.Z $(DISTNAME)
	rm -rf $(DISTNAME)
.PHONY: dist

.PHONY: install
install: $(PROGS)
#	for file in $(PROGS); do \
#	cp $$file $(bindir)/$${file}.new; \
#	mv $(bindir)/$${file}.new $(bindir)/$$file; \
#	done
	install $(archpfx)ar $(bindir)/ar
	if [ -f $(archpfx)gprof ]; then \
	  install $(archpfx)gprof $(bindir)/grof; \
	else true; fi
	install $(archpfx)ld $(bindir)/ld
	install $(archpfx)nm $(bindir)/nm
	install $(archpfx)objdump $(bindir)/objdump
	install $(archpfx)ranlib $(bindir)/ranlib
	install $(archpfx)size $(bindir)/size
