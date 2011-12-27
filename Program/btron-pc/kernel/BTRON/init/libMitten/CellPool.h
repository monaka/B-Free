extern ER MitnCell_GetCellAsLong(Cell **c, long n)
;
extern ER MitnCell_GetCellAsDouble(Cell **c, double n)
;
extern ER MitnCell_GetCellAsStr(Cell **c, B* pStr)
;
extern ER MitnCell_GetCellAsName(Cell **c, B* pStr)
;
extern ER MitnCell_GetCellAsTradFunc(Cell **c, ER (*f)(int argc, char* argv[]), W arg_num)
;
extern ER MitnCell_GetCellAsMitnFunc(Cell **c, ER (*f)(void))
;
extern ER MitnCell_GetCellAsMark(Cell **dest)
;
extern ER MitnCell_GetCellAsLink(Cell **dest, Cell *src)
;
extern ER MitnCell_ToString(B *str, Cell *c, W len)
;
extern ER MitnCell_GetLongFromCell(long *n, Cell *c)
;
extern ER MitnCell_CopyCell(Cell **dest, Cell *src)
;
