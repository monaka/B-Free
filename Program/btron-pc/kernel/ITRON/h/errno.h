/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* errno.h
 *
 */

#ifndef __ITRON_ERRNO_H__
#define __ITRON_ERRNO_H__	1


#define E_OK		(0)		/* ���ｪλ		*/

#define E_SYS		(-5)		/* �����ƥ२�顼	*/

#define E_NOMEM		(-10)		/* ������­		*/

#define E_NOSPT		(-17)		/* ̤���ݡ��ȵ�ǽ	*/
#define E_INOSPT	(-18)		/* ITRON/FILE �Ǥ�̤���ݡ��ȵ�ǽ */
#define E_RSFN		(-20)
#define E_RSATR		(-24)

#define E_PAR		(-33)		/* �������� */
#define E_ID		(-35)

#define E_NOEXS		(-52)
#define E_OBJ		(-63)

#define E_MACV		(-65)
#define E_OACV		(-66)

#define E_CTX		(-69)

#define E_QOVR		(-73)

#define E_DLT		(-81)

#define E_TMOUT		(-85)
#define E_RLWAI		(-86)

/*************** ��ĥ��ʬ **************/

#define E_DEV		(-101)

#endif /* __ITRON_ERRNO_H__ */
