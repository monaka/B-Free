/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

The Mitten language has contributed to the B-Free Project.

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

1996-1998 (C) Monamona Monaka.
(C) B-Free Project.

*/
/* $Id: evaluator.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "$Id: evaluator.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: evaluator.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.10  1998-12-18 02:29:05  monaka
 * Major modify. The stack data structure was re-born. It was no change for adding new operators.
 *
 * Revision 1.9  1998/12/12 05:30:38  monaka
 * Some error trap and some return value added.
 *
 * Revision 1.8  1998/12/08 08:10:26  monaka
 * Include files changed.
 *
 * Revision 1.7  1998/12/08 05:38:51  monaka
 * I init has been included the Mitten Scripting Language since this release.
 * But, it's just a first preview. So you can't use many convenience
 * features. Mitten works extension scripting environment like Tcl, Guile,
 * Windows Scripting Host, and so on. If you wished, you may connect your
 * function written in C to Mitten operator. If you wished, you may call
 * Mitten from C.
 *
 * Revision 1.6  1998/12/02 09:45:29  monaka
 * Major bug fix. Since this version, we got Mitten's core components.
 *
 * Revision 1.5  1998/12/02 01:03:46  monaka
 * Major modified for implementing scripting language.
 * But there are a partly connection to user interface.
 *
 * Revision 1.4  1998/11/26 21:28:58  monaka
 * Bug fix for argument count.
 *
 * Revision 1.3  1998/11/26 15:24:13  monaka
 *
 * Attributes for built-in commands added.
 * It indicates function call type, the number of argument, and so on.
 *
 * Revision 1.2  1998/11/26 11:17:01  monaka
 *
 * Kanji code conversion fix. It's no change to code.
 *
 * Revision 1.1  1998/11/26 11:14:31  monaka
 *
 * Command evaluator has moved to libMitten/evaluator.c
 * This modify is for Scripting Environment but has not implemented yet.
 *
 */
#include <h/types.h>
#include <h/errno.h>
#include "CellMemory.h"
#include "mitten-command.h"

#include "MitnStack.h"
#include "CellPool.h"

ER
allocate_trad_cmdline(B *line, B *cmd, W ac, B **av)
{
  int i;
  B* pos;
  ER result;

  av[0] = line;
  strcpy(line, cmd);
  pos = line + strlen(cmd) + 1;

  for(i = ac - 1; i > 0; i--)
    {
      Cell *c;
      B str[48];
      int len;

      result = MitnStack_Pop(DATA_STACK, &c);
      if (result != E_OK)
	{
	  return result;
	}

      result = MitnCell_ToString(str, c, sizeof(str));
      if (result != E_OK)
	{
	  return result;
	}
      av[i] = pos;
      len = strlen(str);
      strcpy(pos, str);
      pos += len;
      *(pos++) = '\0';
    }

  return E_OK;
}

void
eval (B *line)
{
  ER	result;
  Cell *c;

  result = parse_line (line);
  if (result != E_OK)
    {
      printf ("%s: line parse error.\n", line);
      return;
    }

  while (1)
    {
      ER result;

      result = MitnStack_Pop(EXEC_STACK, &c);

      if (result != E_OK)
	{
	  break;
	}
      if ((c->type & CELL_BLTI) != CELL_BLTI)
	{
	  MitnStack_Push(DATA_STACK, c);
	  continue;
	}

      switch(c->type)
	{
	case CELL_MITN:
	  c->value.mitn_func ();
	  break;
	case CELL_TRAD:
	  {
	    B line[255];
	    B cmd[] = "unknown";
	    W arg_num;
	    B *av[10];
	    arg_num = c->value.trad.arg_num;
	    allocate_trad_cmdline(line, cmd, arg_num, av);
	    c->value.trad.func (arg_num, av);
	  }
	  break;
	default:
	  printf("?????");
	  break;
	}
    }
}


