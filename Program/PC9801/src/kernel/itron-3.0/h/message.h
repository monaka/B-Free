/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* message.h --- ��å������Хåե�����
 *
 *
 */

#ifndef __H_MESSAGE_H__
#define __H_MESSAGE_H__		1

/*******************************************************************************
 * ��å������Хåե��Υإå�
 *
 */
typedef struct message_head_t
{
  ATR			mbfatr;
  int			total_size;	/* �����Хåե��Υ����� */
  struct message_t	*message_ptr;
  struct message_t	*last_message_ptr;
  T_TCB			*wait_sendtask;	/* ��å��������ԤäƤ��륿����(����¦) */
  T_TCB			*wait_recvtask;	/* ��å��������ԤäƤ��륿����(����¦) */
  INT			bufsz;		/* �Хåե��κ���Ĺ	*/
  INT			msgsz;		/* ��å������κ���Ĺ	*/
} T_MSGHEAD;



/*******************************************************************************
 * ��å���������ȥ�
 *
 *	��å������Ϥ��Υѥ��åȤ˵ͤ���롣
 *
 */
typedef struct message_t
{
  INT			size;
  struct message_t	*next;
  VP			buf;
} T_MSG;


#endif /* __H_MESSAGE_H__ */
