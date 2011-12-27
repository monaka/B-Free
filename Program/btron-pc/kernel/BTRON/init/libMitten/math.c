/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

The Mitten language has contributed to B-Free Project.

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

1996-1998 (C) Monamona Monaka.
(C) B-Free Project.

*/

/* $Id: math.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "$Id: math.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: math.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.5  1998-12-18 02:29:06  monaka
 * Major modify. The stack data structure was re-born. It was no change for adding new operators.
 *
 * Revision 1.4  1998/12/12 03:14:48  monaka
 * sign operator was added.
 *
 * Revision 1.3  1998/12/08 08:10:27  monaka
 * Include files changed.
 *
 * Revision 1.2  1998/12/08 05:38:52  monaka
 * I init has been included the Mitten Scripting Language since this release.
 * But, it's just a first preview. So you can't use many convenience
 * features. Mitten works extension scripting environment like Tcl, Guile,
 * Windows Scripting Host, and so on. If you wished, you may connect your
 * function written in C to Mitten operator. If you wished, you may call
 * Mitten from C.
 *
 * Revision 1.1  1998/12/02 09:42:29  monaka
 * First version. Implemented but not used by user.
 *
 */
#include <h/types.h>
#include <h/errno.h>
#include "CellMemory.h"
#include "mitten-command.h"

#include "MitnStack.h"
#include "CellPool.h"

ER
MitnMath_Neg(void)
{
  Cell *c;
  long n;
  ER result;

  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&n, c);
  MITN_ERROR_CHECK(result);

  n = 0 - n;

  result = MitnCell_GetCellAsLong(&c, n);
  MITN_ERROR_CHECK(result);
  result = MitnStack_Push(DATA_STACK, c);
  MITN_ERROR_CHECK(result);

  return E_OK;
}

ER
MitnMath_Add(void)
{
  Cell *c;
  long n1, n2, n;
  ER result;

  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&n1, c);
  MITN_ERROR_CHECK(result);
  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&n2, c);

  n = n2 + n1;

  result = MitnCell_GetCellAsLong(&c, n);
  MITN_ERROR_CHECK(result);
  result = MitnStack_Push(DATA_STACK, c);
  MITN_ERROR_CHECK(result);

  return E_OK;
}

ER
MitnMath_Sub(void)
{
  Cell *c;
  long n1, n2, n;
  ER result;

  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&n1, c);
  MITN_ERROR_CHECK(result);
  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&n2, c);

  n = n2 - n1;

  result = MitnCell_GetCellAsLong(&c, n);
  MITN_ERROR_CHECK(result);
  result = MitnStack_Push(DATA_STACK, c);
  MITN_ERROR_CHECK(result);

  return E_OK;
}

ER
MitnMath_Mul(void)
{
  Cell *c;
  long n1, n2, n;
  ER result;

  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&n1, c);
  MITN_ERROR_CHECK(result);
  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&n2, c);

  n = n2 * n1;

  result = MitnCell_GetCellAsLong(&c, n);
  MITN_ERROR_CHECK(result);
  result = MitnStack_Push(DATA_STACK, c);
  MITN_ERROR_CHECK(result);

  return E_OK;
}

ER
MitnMath_Div(void)
{
  Cell *c;
  long n1, n2, n;
  ER result;

  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&n1, c);
  MITN_ERROR_CHECK(result);
  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&n2, c);

  n = n2 / n1;

  result = MitnCell_GetCellAsLong(&c, n);
  MITN_ERROR_CHECK(result);
  result = MitnStack_Push(DATA_STACK, c);
  MITN_ERROR_CHECK(result);

  return E_OK;
}


