extern ER MitnStack_Clear (W wStkNum)
;
extern ER MitnStack_ClearToMark(W wStkNum)
;
extern ER MitnStack_CountToMark(W wStkNum, W *count)
;
extern ER MitnStack_Count(W wStkNum, W *count)
;
extern ER MitnStack_Status(void)
;
extern ER MitnStack_PushMark(W wStkNum)
;
extern ER MitnStack_Push(W wStkNum, Cell *src)
;
extern ER MitnStack_GetLinkCellFromPool(Cell **dest)
;
extern ER MitnStack_PutLinkCellToPool(Cell *src)
;
extern ER MitnStack_Pop(W wStkNum, Cell **dest)
;
extern ER MitnStack_BottomPush(W wStkNum, Cell *src)
;
extern ER MitnStack_Duplicate (W wStkNum, long num)
;
extern ER MitnStack_Roll (W wStkNum, long num)
;
