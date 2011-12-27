/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

The Mitten language has contributed to B-Free Project.

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

1996-1998 (C) Monamona Monaka.
(C) B-Free Project.

*/
#include <errno.h>
#include "CellMemory.h"
#include "mitten-command.h"

#include "CellPool.h"
#include "MitnStack.h"

Cell MitnStack_Bottom[STACK_MAX];
Cell MitnStack_Top[STACK_MAX];

Cell MitnCellMemory[CELLMEMORY_MAX];

void MitnStack_Init (void)
{
  int i;
  for(i = 0; i < STACK_MAX; i++) {
    MitnStack_Clear(i);
  }

  for(i = 0; i < CELLMEMORY_MAX; i++)
    {
      MitnStack_PutLinkCellToPool (&MitnCellMemory[i]);
    }
}

ER MitnStack_Clear (W wStkNum)
{
  if (wStkNum < 0 || wStkNum >= STACK_MAX)
    {
      return E_PAR;
    }

  MitnStack_Top[wStkNum].type = CELL_LINK;
  MitnStack_Top[wStkNum].value.link = NULL;
  MitnStack_Top[wStkNum].prev = NULL;
  MitnStack_Top[wStkNum].next = &MitnStack_Bottom[wStkNum];
    
  MitnStack_Bottom[wStkNum].type = CELL_LINK;
  MitnStack_Bottom[wStkNum].prev = &MitnStack_Top[wStkNum];
  MitnStack_Bottom[wStkNum].next = NULL;
  MitnStack_Bottom[wStkNum].value.link = NULL;

  return E_OK;
}

ER MitnStack_ClearToMark(W wStkNum)
{
  ER result;
  Cell *c;
  
  if (wStkNum < 0 || wStkNum > STACK_NUM)
    {
      return E_PAR;
    }

  while (1) {
    result = MitnStack_Pop(DATA_STACK, &c);
    MITN_ERROR_CHECK(result);

    if (c->type == CELL_MARK) {
     break;
    }
  }

  return E_OK;
}

ER MitnStack_CountToMark(W wStkNum, W *count)
{
  Cell *c;
  int i;

  if (wStkNum < 0 || wStkNum > STACK_NUM)
    {
      return E_PAR;
    }

  /* mark reset */
  for(i = 0; i < CELLMEMORY_MAX; i++)
    {
      MitnCellMemory[i].mark = 0;
    }
  *count = 0;
  for(c = MitnStack_Top[wStkNum].next;
      c->type != CELL_MARK;
      c = c->next ) 
    {
      if (c == &MitnStack_Bottom[wStkNum])
	{
	  return E_PAR;
	}

      if (c->mark == 1)
	{
	  return E_SYS;
	}
      c->mark = 1;
      (*count)++;
    }

  return E_OK;
}

ER MitnStack_Count(W wStkNum, W *count)
{
  Cell *c;
  int i;

  if (wStkNum < 0 || wStkNum > STACK_NUM)
    {
      return E_PAR;
    }

  /* mark reset */
  for(i = 0; i < CELLMEMORY_MAX; i++)
    {
      MitnCellMemory[i].mark = 0;
    }
  *count = 0;
  for(c = MitnStack_Top[wStkNum].next;
      c != &MitnStack_Bottom[wStkNum];
      c = c->next ) 
    {
      if (c->mark == 1)
	{
	  return E_SYS;
	}
      c->mark = 1;
      (*count)++;
    }

  return E_OK;
}  

ER MitnStack_Status(void)
{
  int i;
  W count;

  /* mark reset */
  for(i = 0; i < CELLMEMORY_MAX; i++)
    {
      MitnCellMemory[i].mark = 0;
    }
  
  for(i = 0; i < STACK_MAX; i++) {
    ER status;

    status = MitnStack_Count(i, &count);
    MITN_ERROR_CHECK(status);

    printf("Stack No.==%d Count==%d\n", i, count);
  }

  return E_OK;
}

ER MitnStack_PushMark(W wStkNum)
{
  Cell *dest;
  ER result;

  if (wStkNum < 0 || wStkNum > STACK_NUM)
    {
      return E_PAR;
    }

  result = MitnCell_GetCellAsMark(&dest);
  MITN_ERROR_CHECK(result);

  dest->next = MitnStack_Top[wStkNum].next;
  dest->prev = &MitnStack_Top[wStkNum];
  MitnStack_Top[wStkNum].next->prev = dest;
  MitnStack_Top[wStkNum].next = dest;

  return E_OK;
}
  
ER MitnStack_Push(W wStkNum, Cell *src)
{
  Cell *dest;
  ER result;

  if (wStkNum < 0 || wStkNum > STACK_NUM)
    {
      return E_PAR;
    }

  if (src == NULL)
    {
      return E_PAR;
    }

  if ((src->type & CELL_COBJ) == CELL_COBJ) {
    result = MitnCell_GetCellAsLink(&dest, src);
    MITN_ERROR_CHECK(result);
  } else {
    dest = src;
  }

  dest->next = MitnStack_Top[wStkNum].next;
  dest->prev = &MitnStack_Top[wStkNum];
  MitnStack_Top[wStkNum].next->prev = dest;
  MitnStack_Top[wStkNum].next = dest;

  return E_OK;
}

ER MitnStack_GetLinkCellFromPool(Cell **dest)
{
  Cell *c;

  c = MitnStack_Top[CELL_POOL].next;
  
  if (c == &MitnStack_Bottom[CELL_POOL])
    {
      return E_SYS;
    }

  *dest = c;
  c->next->prev = &MitnStack_Top[CELL_POOL];
  MitnStack_Top[CELL_POOL].next = c->next;
  c->next = NULL;
  c->prev = NULL;
  c->value.link = NULL;

  return E_OK;
}
  
ER MitnStack_PutLinkCellToPool(Cell *src)
{
  src->type = CELL_LINK;
  src->next = MitnStack_Top[CELL_POOL].next;
  src->prev = &MitnStack_Top[CELL_POOL];
  src->value.link = NULL;
  MitnStack_Top[CELL_POOL].next->prev = src;
  MitnStack_Top[CELL_POOL].next = src;

  return E_OK;
}

ER MitnStack_Pop(W wStkNum, Cell **dest)
{
  Cell *c;

  if (wStkNum < 0 || wStkNum > STACK_NUM)
    {
      return E_PAR;
    }

  c = MitnStack_Top[wStkNum].next;
  
  if (c == NULL) {
    return E_SYS;
  }

  if (c == &MitnStack_Bottom[wStkNum]) {
    return E_PAR;
  }
  
  if ((c->type & CELL_SOBJ) == CELL_SOBJ) {
    *dest = c;
  } else {
    *dest = c->value.link;
  }

  c->next->prev = NULL;
  c->next->prev = &MitnStack_Top[wStkNum];
  MitnStack_Top[wStkNum].next = c->next;

  c->next = NULL;
  c->prev = NULL;

  return E_OK;
}

ER MitnStack_BottomPush(W wStkNum, Cell *src)
{
  Cell *dest;
  ER result;

  if (wStkNum < 0 || wStkNum > STACK_NUM)
    {
      return E_PAR;
    }

  if (src == NULL)
    {
      return E_PAR;
    }

  if ((src->type & CELL_COBJ) == CELL_COBJ) {
    result = MitnCell_GetCellAsLink(&dest, src);
    MITN_ERROR_CHECK(result);
  } else {
    dest = src;
  }

  dest->next = &MitnStack_Bottom[wStkNum];
  dest->prev = MitnStack_Bottom[wStkNum].prev;

  MitnStack_Bottom[wStkNum].prev->next = dest;
  MitnStack_Bottom[wStkNum].prev = dest;

  return E_OK;
}

ER MitnStack_Duplicate (W wStkNum, long num)
{
  long i;
  Cell *p;

  for(p = MitnStack_Top[wStkNum].next, i = 1;
      i < num;
      p = p->next, i++)
    {
      if (p == &MitnStack_Bottom[wStkNum])
	{
	  return E_PAR;
	}
    }
  for(i = 0;
      i < num;
      i++, p = p->prev)
    {
      ER result;
      Cell *dest;
      result = MitnCell_CopyCell(&dest, p);
      result = MitnStack_Push(wStkNum, dest);
      MITN_ERROR_CHECK(result);
    }

  return E_OK;
}


ER MitnStack_Roll (W wStkNum, long num)
{
  ER result;
  long i;
  Cell *p;

  for(p = MitnStack_Top[wStkNum].next, i = 1;
      i < num;
      p = p->next, i++)
    {
      if (p == &MitnStack_Bottom[wStkNum])
	{
	  return E_PAR;
	}
    }
  p->prev->next = p->next;
  p->next->prev = p->prev;
  result = MitnStack_Push(wStkNum, p);
  MITN_ERROR_CHECK(result);

  return E_OK;
}


#ifdef _DEBUG_ON_UNIX
W isnum (W ch)
{
  if (ch == '-') {
    return TRUE;
  }

  ch -= '0';

  if (ch < 0 || ch > 9)
    {
      return FALSE;
    }

  return TRUE;
}

void main(int argc, char *argv[])
{
  Cell *c;
  ER result;
  char *str;

  if(argc != 2) {
    printf("error");
    return;
  }
  str = argv[1];

  MitnStack_Init();
  InitMittenHook();

  MitnStack_Status();
  MitnStack_GarbageCollect();
  MitnStack_Status();

  eval(str);

  MitnStack_Status();
  MitnStack_GarbageCollect();
  MitnStack_Status();
  MitnStack_GarbageCollect();
  MitnStack_Status();

  eval(str);

  MitnStack_Status();
  MitnStack_GarbageCollect();
  MitnStack_Status();

  printf("%s\n", str);
}
#endif

