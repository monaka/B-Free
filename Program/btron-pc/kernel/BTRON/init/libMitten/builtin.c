/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

The Mitten language has contributed to B-Free Project.

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* $Id: builtin.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "$Id: builtin.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: builtin.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.8  1998-12-19 05:35:15  monaka
 * dup operator fixed.
 *
 * Revision 1.7  1998/12/18 02:29:03  monaka
 * Major modify. The stack data structure was re-born. It was no change for adding new operators.
 *
 * Revision 1.6  1998/12/12 03:10:26  monaka
 * roll operator defined. Strictly speaking, it doesn't have complete
 *  compatibility with PostScript.
 *
 * Revision 1.5  1998/12/08 08:10:24  monaka
 * Include files changed.
 *
 * Revision 1.4  1998/12/08 05:38:47  monaka
 * I init has been included the Mitten Scripting Language since this release.
 * But, it's just a first preview. So you can't use many convenience
 * features. Mitten works extension scripting environment like Tcl, Guile,
 * Windows Scripting Host, and so on. If you wished, you may connect your
 * function written in C to Mitten operator. If you wished, you may call
 * Mitten from C.
 *
 * Revision 1.3  1998/12/02 09:45:27  monaka
 * Major bug fix. Since this version, we got Mitten's core components.
 *
 * Revision 1.2  1998/12/02 01:03:41  monaka
 * Major modified for implementing scripting language.
 * But there are a partly connection to user interface.
 *
 */
#include <h/types.h>
#include <h/errno.h>
#include "CellMemory.h"
#include "mitten-command.h"

#include "MitnStack.h"
#include "CellPool.h"
#include "MitnDict.h"

ER echo (W ac, B **av)
{
  W	i;

  for (i = 1; i < ac; i++)
    {
      printf ("%s\n", av[i]);
    }
  return (0);
}

ER exch (void)
{
  Cell *c1;
  Cell *c2;
  ER result;

  result = MitnStack_Pop(DATA_STACK, &c1);
  MITN_ERROR_CHECK(result);
  result = MitnStack_Pop(DATA_STACK, &c2);
  MITN_ERROR_CHECK(result);

  result = MitnStack_Push(DATA_STACK, c1);
  MITN_ERROR_CHECK(result);
  result = MitnStack_Push(DATA_STACK, c2);
  MITN_ERROR_CHECK(result);

  return E_OK;
}

ER clear (void)
{
  ER result;

  result = MitnStack_Clear(DATA_STACK);
  MITN_ERROR_CHECK(result);

  return E_OK;
}

ER cleartomark (void)
{
  ER result;

  result = MitnStack_ClearToMark(DATA_STACK);
  MITN_ERROR_CHECK(result);

  return E_OK;
}

ER dup (void)
{
  Cell *c;
  ER result;

  result = MitnStack_Duplicate (DATA_STACK, 1);
  MITN_ERROR_CHECK(result);

  return E_OK;
}

ER copy (void)
{
  Cell *c;
  ER result;
  long num;

  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&num, c);
  MITN_ERROR_CHECK(result);
  result = MitnStack_Duplicate(DATA_STACK, num);
  MITN_ERROR_CHECK(result);

  return E_OK;
}

ER roll (void)
{
  Cell *c;
  ER result;
  long num;
  long times;
  long i;

  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&times, c);
  MITN_ERROR_CHECK(result);
  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&num, c);
  MITN_ERROR_CHECK(result);

  times %= num;
  if (num <= 0) {
    times = 0 - times;
  } else {
    times = num - times;
  }
  
  for(i = 0; i < times; i++)
    {
      result = MitnStack_Roll(DATA_STACK, num);
      MITN_ERROR_CHECK(result);
    }

  return E_OK;
}

ER counttomark (void)
{
  ER result;
  W count;
  Cell *c;

  result = MitnStack_CountToMark(DATA_STACK, &count);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetCellAsLong(&c, count);
  MITN_ERROR_CHECK(result);
  result = MitnStack_Push(DATA_STACK, c);
  MITN_ERROR_CHECK(result);

  return E_OK;
}

ER mark (void)
{
  ER result;

  result = MitnStack_PushMark(DATA_STACK);
  MITN_ERROR_CHECK(result);

  return E_OK;
}

ER help (void)
{
  return E_NOSPT;
}

ER pop (void)
{
  Cell *c; // but not used.
  ER result;

  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  
  return E_OK;
}

ER count (void)
{
  Cell *c;
  ER result;
  W count;

  result = MitnStack_Count(DATA_STACK, &count);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetCellAsLong(&c, count);
  MITN_ERROR_CHECK(result);
  result = MitnStack_Push(DATA_STACK, c);
  MITN_ERROR_CHECK(result);

  return E_OK;
}

ER PopAndPrint (void)
{
  Cell *c;
  char str[255];
  ER result;

  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_ToString(str, c, 255);
  MITN_ERROR_CHECK(result);

  printf("%s", str);

  return E_OK;
}

