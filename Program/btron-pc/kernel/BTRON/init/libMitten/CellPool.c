/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

The Mitten language has contributed to B-Free Project.

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

1996-1998 (C) Monamona Monaka.
(C) B-Free Project.

*/
#include "CellMemory.h"
#include "CellPool.h"

static ER
MitnCell_GetCell(Cell **c)
{
  ER result;
  result = MitnStack_GetLinkCellFromPool(c);
  MITN_ERROR_CHECK(result);

  return E_OK;
}


ER MitnCell_GetCellAsLong(Cell **c, long n)
{
  ER result;
  result = MitnCell_GetCell(c);
  MITN_ERROR_CHECK(result);

  (*c)->type = CELL_INT;
  (*c)->value.inum = n;

  return E_OK;
}

ER MitnCell_GetCellAsDouble(Cell **c, double n)
{
  ER result;
  result = MitnCell_GetCell(c);
  MITN_ERROR_CHECK(result);

  (*c)->type = CELL_DBL;
  (*c)->value.dnum = n;

  return E_OK;
}

ER MitnCell_GetCellAsStr(Cell **c, B* pStr)
{
  ER result;
  result = MitnCell_GetCell(c);
  MITN_ERROR_CHECK(result);

  (*c)->type = CELL_STR;
  strncpy((*c)->value.string.str, pStr, 48);
  (*c)->value.string.length = strlen(pStr);
  if ((*c)->value.string.length > 48) {
    (*c)->value.string.length = 48;
  }

  return E_OK;
}

ER MitnCell_GetCellAsName(Cell **c, B* pStr)
{
  ER result;
  result = MitnCell_GetCell(c);
  MITN_ERROR_CHECK(result);

  (*c)->type = CELL_NAME;
  (*c)->value.name.link = NULL;
  strncpy((*c)->value.name.str, pStr, 48);

  return E_OK;
}

ER MitnCell_GetCellAsTradFunc(Cell **c, ER (*f)(int argc, char* argv[]), W arg_num)
{
  ER result;
  result = MitnCell_GetCell(c);
  MITN_ERROR_CHECK(result);

  (*c)->type = CELL_TRAD;
  (*c)->value.trad.func = f;
  (*c)->value.trad.arg_num = MITN_ARG_NUM(arg_num) + 1;

  return E_OK;
}

ER MitnCell_GetCellAsMitnFunc(Cell **c, ER (*f)(void))
{
  ER result;
  result = MitnCell_GetCell(c);
  MITN_ERROR_CHECK(result);

  (*c)->type = CELL_MITN;
  (*c)->value.mitn_func = f;

  return E_OK;
}

ER MitnCell_GetCellAsMark(Cell **dest)
{
  ER result;
  result = MitnStack_GetLinkCellFromPool(dest);
  MITN_ERROR_CHECK(result);

  (*dest)->type = CELL_MARK;
  (*dest)->value.link = NULL;
  (*dest)->next = NULL;
  (*dest)->prev = NULL;

  return E_OK;
}

ER MitnCell_GetCellAsLink(Cell **dest, Cell *src)
{
  ER result;
  result = MitnStack_GetLinkCellFromPool(dest);
  MITN_ERROR_CHECK(result);

  (*dest)->type = CELL_LINK;
  (*dest)->value.link = src;
  (*dest)->next = NULL;
  (*dest)->prev = NULL;

  return E_OK;
}

static ER l2a(B *str, long lnum, W len)
{
  int i, j;
  long num;

  if (lnum < 0) {
    num = 0 - lnum;
  } else {
    num = lnum;
  }

  for(i = 0; i < len; i++) {
    str[i] = (num % 10) + '0';
    num /= 10;
    if (num == 0) {
      break;
    }
  }

  if (lnum < 0) {
    str[++i] = '-';
  }

  for(j = 0; j < (i + 1) / 2; j++) {
    char c;
    c = str[j];
    str[j] = str[i - j];
    str[i - j] = c;
  }
  str[i + 1] = '\0';

  return E_OK;
}


ER MitnCell_ToString(B *str, Cell *c, W len)
{
  ER result;
  result = E_OK;

  switch(c->type)
    {
    case CELL_NAME:
      /* same as CELL_STR */
    case CELL_STR:
      strncpy(str, c->value.string.str, len - 1);
      c->value.string.length = len - 1;
      str[len] = '\0';
      break;
    case CELL_INT:
      l2a(str, c->value.inum, len);
      break;
    default:
      result = E_NOSPT;
    }

  return result;
}

ER MitnCell_GetLongFromCell(long *n, Cell *c)
{
  if ((c->type & CELL_INT) != CELL_INT)
    {
      return E_PAR;
    }

  *n = c->value.inum;

  return E_OK;
}

ER MitnCell_CopyCell(Cell **dest, Cell *src)
{
  ER result;

  switch (src->type) {
  case CELL_MARK:
    result = MitnCell_GetCellAsMark(dest);
    break;
  case CELL_INT:
    result = MitnCell_GetCellAsLong(dest, src->value.inum);
    break;
  case CELL_DBL:
    result = E_NOSPT;
    break;
  case CELL_NAME:
    result = MitnCell_GetCellAsName(dest, src->value.name.str);
    break;
  case CELL_NIL:
    result = E_NOSPT;
    break;
  case CELL_STR:
    result = MitnCell_GetCellAsStr(dest, src->value.string.str);
    break;
  case CELL_VECT:
    result = E_NOSPT;
    break;
  case CELL_PROC:
    result = E_NOSPT;
    break;
  case CELL_TRAD:
    result = MitnCell_GetCellAsTradFunc(dest,
				    src->value.trad.func,
				    src->value.trad.arg_num);
    break;
  case CELL_MITN:
    result = MitnCell_GetCellAsMitnFunc(dest,
				    src->value.mitn_func);
    break;
  }
  MITN_ERROR_CHECK(result);

  return E_OK;
}

