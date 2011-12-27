#ifndef __MITTEN_COMMAND_H_LOCAL__
#define __MITTEN_COMMAND_H_LOCAL__
/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

The Mitten language has contributed to B-Free Project.

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

1996-1998 (C) Monamona Monaka.
(C) B-Free Project.

*/

/* $Id: mitten-command.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * $Log: mitten-command.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.6  1998-12-18 02:29:07  monaka
 * Major modify. The stack data structure was re-born. It was no change for adding new operators.
 *
 * Revision 1.5  1998/12/08 05:38:53  monaka
 * I init has been included the Mitten Scripting Language since this release.
 * But, it's just a first preview. So you can't use many convenience
 * features. Mitten works extension scripting environment like Tcl, Guile,
 * Windows Scripting Host, and so on. If you wished, you may connect your
 * function written in C to Mitten operator. If you wished, you may call
 * Mitten from C.
 *
 * Revision 1.4  1998/12/02 09:45:30  monaka
 * Major bug fix. Since this version, we got Mitten's core components.
 *
 * Revision 1.3  1998/12/02 01:03:47  monaka
 * Major modified for implementing scripting language.
 * But there are a partly connection to user interface.
 *
 * Revision 1.2  1998/11/26 15:24:14  monaka
 *
 * Attributes for built-in commands added.
 * It indicates function call type, the number of argument, and so on.
 *
 * Revision 1.1  1998/11/26 13:53:15  monaka
 *
 * init/command.c has split and moved to some subdirs.
 * This means it was changed how to add a built-in function.
 *
 */

#include <h/types.h>
#include <h/errno.h>

#define STACK_NUM 3
#define STACK_MAX (STACK_NUM + 1)
#define CELL_POOL 0
#define DATA_STACK 1
#define EXEC_STACK 2
#define DICT_STACK  3

/* This tag matches the follows (all) operator tags. */
#define CELL_OBJ  (0xFFFFFFFF)


/* Cell tag for numerical operators */
#define CELL_SOBJ (0x10000000)
#define CELL_MARK (0x00010000 | CELL_SOBJ)
#define CELL_NUM  (0x00020000 | CELL_SOBJ)
#define CELL_INT  (0x00040000 | CELL_SOBJ | CELL_NUM)
#define CELL_DBL  (0x00080000 | CELL_SOBJ | CELL_NUM)
#define CELL_NAME (0x000C0000 | CELL_SOBJ)
#define CELL_NIL  (0x00100000 | CELL_SOBJ)

#define CELL_COBJ (0x20000000)
#define CELL_STR  (0x00010000 | CELL_COBJ)
#define CELL_VECT (0x00020000 | CELL_COBJ)
#define CELL_PROC (0x00040000 | CELL_COBJ)

/* Cell tag for built-in operators */
#define CELL_BLTI (0x02000000)
#define CELL_TRAD (0x00010000 | CELL_COBJ | CELL_BLTI)
#define CELL_MITN (0x00020000 | CELL_COBJ | CELL_BLTI)

/* Cell tag for link operators */
#define CELL_LINK (0x40000000)


#define MITN_OP_ARGS_UNKNOWN (0xFFF)
#define MITN_T_R_A_D (0x1000)
#define MITN_M_I_T_N (0x2000)


#define MITN_ERROR_CHECK(_x_) if ((_x_) != E_OK) { return (_x_); }

/* externs*/
 
#define TRAD_OPERATOR(fname, func, flag) \
  extern ER func (W argc, B **argv);
#define MITN_OPERATOR(fname, func, flag) \
  extern ER func (void);
#include "commands.inc"
#undef TRAD_OPERATOR
#undef MITN_OPERATOR

#endif /* __MITTEN_COMMAND_H_LOCAL__ */
