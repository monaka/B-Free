/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* eventflag.h
 *
 *
 */

#ifndef __H_EVENTFLAG_H__
#define __H_EVENTFLAG_H__	1


/***************************************************************************
 *	���٥�ȥե饰����
 *
 */
typedef struct eventflag_t
{
  ID		id;			/* ���٥�ȥե饰 ID 			*/
  ATR		flgatr;			/* ���٥�ȥե饰°��			*/
  UINT		iflgptn;		/* ���٥�ȥե饰����			*/
  T_TCB		*wait_task;		/* ���٥�ȥե饰���ԤäƤ��륿����	*/

  VP		exinf;			/* ��ĥ����				*/
} T_EVENTFLAG;


#endif /* __H_EVENTFLAG_H__ */
