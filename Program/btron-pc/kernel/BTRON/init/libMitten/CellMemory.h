/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

The Mitten language has contributed to B-Free Project.

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

1996-1998 (C) Monamona Monaka.
(C) B-Free Project.

*/
#ifndef __CELLMEMORY_H__
#define __CELLMEMORY_H__

#include <types.h>
#include "mitten-command.h"

#define CELLMEMORY_MAX 1000

struct _cell {
  B mark; 
  UW type;
  struct _cell *prev;
  struct _cell *next;

  union {
    long inum;
    double dnum;

    struct {
      char str[48];
      long length;
    } string;

    struct {
      struct _cell *link;
      char str[48];
    } name;

    ER (*mitn_func)(void);
    struct {
      W arg_num;
      ER (*func)(int argc, char *argv[]);
    } trad;

    struct _cell *link;

  } value;
};

typedef struct _cell Cell;

extern Cell MitnCellMemory[CELLMEMORY_MAX];

#define MITN_ARG_NUM(x) ((x) & 0xFFF)

#endif
/* __CELLMEMORY_H__*/
