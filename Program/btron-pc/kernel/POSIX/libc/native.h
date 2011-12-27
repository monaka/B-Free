/*

B-Free Project ÇÃê∂ê¨ï®ÇÕ GNU Generic PUBLIC LICENSE Ç…è]Ç¢Ç‹Ç∑ÅB

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/libc/native.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */


#ifndef __NATIVE_H__

#include "../manager/posix.h"

#define NAME_MAX	14


#ifdef notdef
struct dirent
{
  long			d_ino;
  unsigned long		d_off;
  unsigned short	d_reclen;
  char			d_name[NAME_MAX + 1];
};
#endif

struct sigaction
{
};

typedef int	sigset_t;


typedef unsigned long	time_t;

struct tms
{
};

typedef unsigned long	clock_t;


struct utimbuf
{
};


typedef unsigned int	pid_t;

#endif


