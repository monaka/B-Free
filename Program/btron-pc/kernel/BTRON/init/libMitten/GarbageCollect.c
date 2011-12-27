/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

The Mitten language has contributed to B-Free Project.

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

1996-1998 (C) Monamona Monaka.
(C) B-Free Project.

*/
#include "CellMemory.h"
#include "mitten-command.h"

#include "GarbageCollect.h"
#include "MitnStack.h"

void
MitnGC_MarkRecursive(Cell *c)
{
  Cell *traceCell;

  if (c == NULL) {
    /* undef な operator のばあい。*/
    return;
  }
  if ((c->type & CELL_BLTI) == CELL_BLTI) {
    c->mark = 1;
    return;
  }

  for(traceCell = c;
      traceCell != NULL;
      traceCell = traceCell->next) {

    if (traceCell->mark == 1) {
      /* needless to mark. */
      break;
    }

    traceCell->mark = 1;

    if (traceCell->type == CELL_NAME) {
      MitnGC_MarkRecursive(traceCell->value.name.link);
    } else if ((traceCell->type & CELL_COBJ) == CELL_COBJ) {
      MitnGC_MarkRecursive(traceCell->value.link);
    }
  }
}

void
MitnGC_Mark()
{
  extern Cell MitnStack_Top[];
  int i;
  for(i = 0; i < STACK_MAX; i++)
    {
      MitnGC_MarkRecursive(MitnStack_Top[i].next);
    }
}

static void
MitnGC_ClearMark()
{
  extern Cell MitnStack_Top[];
  extern Cell MitnStack_Bottom[];
  int i;

  for(i = 0; i < STACK_MAX; i++)
    {
      MitnStack_Top[i].mark = 0;
      MitnStack_Bottom[i].mark = 0;
    }

  /* mark reset */
  for(i = 0; i < CELLMEMORY_MAX; i++)
    {
      MitnCellMemory[i].mark = 0;
    }
}

static void
MitnGC_Sweep()
{
  int i;
  for(i = 0;i  <  CELLMEMORY_MAX; i++)
    {
      Cell *c;
      c = &MitnCellMemory[i];

      if (c->mark == 0)
	{
	  /* もし cell の中で２次的に malloc しているセルがあれば 開放する。 */
	  if ((c->type & CELL_STR) == CELL_STR)
	    {
	    }
	  
	  MitnStack_PutLinkCellToPool(c);
	}
    }	  
}

ER MitnStack_GarbageCollect(void)
{
/*printf("GC was started : ClearMark-");*/
  MitnGC_ClearMark();
/*printf("Mark-");*/
  MitnGC_Mark();
/*printf("Sweep-");*/
  MitnGC_Sweep();
/*printf("Done\r\n");*/

  return E_OK;
}

