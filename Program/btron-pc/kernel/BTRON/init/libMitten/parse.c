/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

The Mitten language has contributed to B-Free Project.

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

1996-1998 (C) Monamona Monaka.
(C) B-Free Project.

*/
/* $Id: parse.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "$Id: parse.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: parse.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.7  1998-12-18 02:29:08  monaka
 * Major modify. The stack data structure was re-born. It was no change for adding new operators.
 *
 * Revision 1.6  1998/12/12 05:30:39  monaka
 * Some error trap and some return value added.
 *
 * Revision 1.5  1998/12/08 08:10:28  monaka
 * Include files changed.
 *
 * Revision 1.4  1998/12/08 05:38:54  monaka
 * I init has been included the Mitten Scripting Language since this release.
 * But, it's just a first preview. So you can't use many convenience
 * features. Mitten works extension scripting environment like Tcl, Guile,
 * Windows Scripting Host, and so on. If you wished, you may connect your
 * function written in C to Mitten operator. If you wished, you may call
 * Mitten from C.
 *
 * Revision 1.3  1998/12/02 09:45:31  monaka
 * Major bug fix. Since this version, we got Mitten's core components.
 *
 * Revision 1.2  1998/12/02 01:03:48  monaka
 * Major modified for implementing scripting language.
 * But there are a partly connection to user interface.
 *
 * Revision 1.1  1998/11/26 10:19:09  monaka
 *
 * Parser routines has moved to libMitten/parse.c and modified.
 * This modify makes you use multi word string and some escape character on
 * init scripting interpreter.
 * Note that you can't use Mitten language yet. The command syntax is still
 * traditional.
 *
 */
#include <h/types.h>
#include <h/errno.h>
#include "CellMemory.h"
#include "mitten-command.h"

#include "MitnStack.h"
#include "CellPool.h"

static B *
edit_str(B *str, B *pos, B termChar)
{
  B* p, *pStr;
  int phase;

  phase = 0;
  for(p = pos, pStr = str; *p != '\0'; p++)
    {
      if (phase == 0)
	{
	  if (*p == termChar)
	    {
	      *pStr = '\0';
	      p++;
	      return p;
	    }
	  switch(*p)
	    {
	    case '\\':
	      phase = 1;
	      continue;
	      break; /* not reached */
	    default:
	      *pStr++ = *p;
	    }
	}
      else
	{
	  switch(*p)
	    {
	    case 'r':
	      *pStr++ = '\r';
	      break;
	    case 'n':
	      *pStr++ = '\n';
	      break;
	    case ')':
	      *pStr++ = ')';
	      break;
	    case '(':
	      *pStr++ = '(';
	      break;
	    case '\\':
	      *pStr++ = '\\';
	      break;
	    default:
	      /* do nothing */
	      break;
	    }
	}
    }
  *pStr = '\0';
  return p;
}

ER
parse_line (B *line)
{
  Cell *c;
  B *pos;
  ER result;

  pos = line;
  while(TRUE)
    {
      if (*pos == '\0')
	{
	  return E_OK;
	}

      while (isspace(*pos))
	{
	  if (*pos == '\0')
	    {
	      return E_OK;
	    }
	  pos++;
	}

      if (*pos == '(')
	{
	  B str[255];
	  pos = edit_str(str, ++pos, ')');
	  if (pos == NULL)
	    {
	      return E_PAR;
	    }
	  MitnCell_GetCellAsStr(&c, str);
	  MitnStack_BottomPush(EXEC_STACK, c);
	}
      else if (isnum(*pos))
	{
	  long num;
	  num = atoi(pos);
	  for( ; !(isspace(*pos) || *pos == '\0'); pos++)
	    {
	      /* do nothing */
	    }
	  MitnCell_GetCellAsLong(&c, num);
	  MitnStack_BottomPush(EXEC_STACK, c);
	}
      else
	{
	  B str[255];

	  pos = edit_str(str, pos, ' ');

	  result = MitnDict_FindCellFromDict(&c, str);
	  if (result != E_OK)
	    {
	      /* not found */
	      continue;
	    }

	  switch (c->type)
	    {
	    case CELL_MITN:
	    case CELL_TRAD:
	      MitnStack_BottomPush(EXEC_STACK, c);
	      break;
	    default:
	      break;
	    }
	}
    }
  return E_OK;
}

