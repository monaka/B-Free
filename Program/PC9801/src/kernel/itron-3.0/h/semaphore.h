/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* semaphore.h
 *
 */

#ifndef __H_SEMAPHORE_H__
#define __H_SEMAPHORE_H__	1

/* ���ޥե���¤��:
 *
 */
typedef struct semaphore_t
{
  T_TCB		*waitlist;
  ATR		sematr;
  INT		isemcnt;
  INT		maxsem;
  VP		exinf;
} T_SEMAPHORE;


#endif /* __H_SEMAPHORE_H__ */
