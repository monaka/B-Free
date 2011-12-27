/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

The Mitten language has contributed to B-Free Project.

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

1996-1998 (C) Monamona Monaka.
(C) B-Free Project.

*/
#include "CellMemory.h"

#include "MitnDict.h"
#include "CellPool.h"
#include "MitnStack.h"

ER MitnDict_DefineOperator(B *name, Cell *c)
{
  ER result;
  Cell *targetCell;

  result = MitnCell_GetCellAsName(&targetCell, name);
  if (result == E_OK)
    {
      targetCell->value.name.link = c;
      result = MitnStack_Push(DICT_STACK, targetCell);
    }

  return result;
}

ER MitnDict_FindCellFromDict(Cell **c, B * str)
{
  extern Cell MitnStack_Top[];
  extern Cell MitnStack_Bottom[];
  Cell *nameCell;

  for (nameCell = MitnStack_Top[DICT_STACK].next;
       nameCell != &MitnStack_Bottom[DICT_STACK];
       nameCell = nameCell->next) {
    if (nameCell->type != CELL_NAME) {
      return E_SYS;
    }
    if (strcmp (str, nameCell->value.name.str) == 0) {
      *c = nameCell->value.name.link;
      return E_OK;
    }
  }
  *c = NULL;
  return E_PAR;
}





