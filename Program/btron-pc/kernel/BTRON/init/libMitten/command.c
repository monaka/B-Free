/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

The Mitten language has contributed to B-Free Project.

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* $Id: command.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "$Id: command.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: command.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.6  1998-12-08 08:08:37  monaka
 * There was modified how to search built-in command.
 * command structure was already obsolute.
 *
 * Revision 1.5  1998/12/08 05:38:48  monaka
 * I init has been included the Mitten Scripting Language since this release.
 * But, it's just a first preview. So you can't use many convenience
 * features. Mitten works extension scripting environment like Tcl, Guile,
 * Windows Scripting Host, and so on. If you wished, you may connect your
 * function written in C to Mitten operator. If you wished, you may call
 * Mitten from C.
 *
 * Revision 1.4  1998/12/02 01:03:42  monaka
 * Major modified for implementing scripting language.
 * But there are a partly connection to user interface.
 *
 * Revision 1.3  1998/11/26 15:24:08  monaka
 *
 * Attributes for built-in commands added.
 * It indicates function call type, the number of argument, and so on.
 *
 * Revision 1.2  1998/11/26 13:53:13  monaka
 *
 * init/command.c has split and moved to some subdirs.
 * This means it was changed how to add a built-in function.
 *
 */

#include <h/types.h>
#include <h/errno.h>
#include "CellMemory.h"
#include "mitten-command.h"



#define TRAD_OPERATOR(fname, func, flag) \
{ \
    Cell *c; \
    MitnCell_GetCellAsTradFunc(&c, (func), 0xFFF & (flag)); \
    MitnDict_DefineOperator((fname), c); \
}
#define MITN_OPERATOR(fname, func, flag) \
{ \
    Cell *c; \
    MitnCell_GetCellAsMitnFunc(&c, (func)); \
    MitnDict_DefineOperator((fname), c); \
}

void InitMittenHook(void)
{
#include "commands.inc"
}
#undef TRAD_OPERATOR
#undef MITN_OPERATOR
