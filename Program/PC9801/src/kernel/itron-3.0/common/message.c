/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* message.c --- ��å������Хåե�����
 *
 *
 */


#include "itron.h"
#include "config.h"
#include "memory.h"
#include "errno.h"
#include "func.h"
#include "pc98.h"
#include "message.h"


static T_MSGHEAD		message_table[NMSGBUF];
static T_MSG			msgbuf[MAX_MSGENTRY];
static T_MSG			*free_msg;



/****************************************************************************
 * alloc_msg
 */
static T_MSG *
alloc_msg (void)
{
  T_MSG	*p;

  if (free_msg == NULL)
    {
      return (NULL);
    }
  
  p = free_msg;
  free_msg = free_msg->next;
  return (p);
}

/****************************************************************************
 * dealloc_msg
 */
static void
dealloc_msg (T_MSG *p)
{
  p->next = free_msg;
  free_msg = p;
}

/***************************************************************************
 * add_msg_list --- ��å������ꥹ�Ȥ˥�å��������ɲä��롣
 *
 */
static void
add_msg_list (T_MSGHEAD *list, T_MSG *newmsg)
{
  dis_int ();
  if (list->last_message_ptr == NULL)
    {
      list->message_ptr = list->last_message_ptr = newmsg;
    }
  else
    {
      list->message_ptr->next = newmsg;
      list->message_ptr = newmsg;
    }
  ena_int ();
}

/***************************************************************************
 * get_msg --- ��å������ꥹ�Ȥ����å���������Ф�
 *
 */
static T_MSG *
get_msg (T_MSGHEAD *list)
{
  T_MSG *p;

  if (list->message_ptr == NULL)
    return (NULL);

  dis_int ();
  p = list->message_ptr;
  if (p == list->last_message_ptr)
    {
      list->message_ptr = list->last_message_ptr = NULL;
    }
  else
    {
      list->message_ptr = p->next;
    }
  ena_int ();
  return (p);
}

/****************************************************************************
 * init_msgbuf --- ��å������Хåե���ǽ�ν����
 *
 * ������
 *	�ʤ�
 *
 * �֤��͡�
 *	���顼�ֹ�
 */
ER
init_msgbuf (void)
{
  T_MSG	*p;
  INT	i;

  free_msg = &msgbuf[0];
  for (i = 0; i < MAX_MSGENTRY; i++)
    {
      msgbuf[i].next = &msgbuf[i + 1];
    }
  msgbuf[MAX_MSGENTRY].next = NULL;

  for (i = 0; i < NMSGBUF; i++)
    {
      message_table[i].mbfatr = TA_UNDEF;
    }
  return (E_OK);
}


/****************************************************************************
 * cre_mbf -- ��å������Хåե�����
 *
 * ������
 *	id	��å������Хåե� ID
 *	pk_cmbf	��å������Хåե���������
 *		mbfatr	��å�����°�� (TA_FIFO ����)
 *		bufsz	�Хåե�������
 *		maxmsz	��å������κ���Ĺ
 *
 * �֤��͡�
 *	ER	���顼�ֹ�
 *		E_OK	���ｪλ
 *		E_ID	��å����� ID ����
 *		E_OBJ	Ʊ��ID �Υ�å����������Ǥ�¸�ߤ���
 *		E_PAR	�ѥ�᡼������
 *
 * ��ǽ��
 *	��å������Хåե����������롣
 */
ER
cre_mbf (ID id, T_CMBF *pk_cmbf)
{
  if ((id < MIN_MSGBUF) || (id > MAX_MSGBUF))
    {
      return (E_ID);
    }
  if ((pk_cmbf->mbfatr != TA_TFIFO) || (pk_cmbf->bufsz <= 0) || (pk_cmbf->maxmsz <= 0))
    {
      return (E_PAR);
    }
  if (message_table[id].mbfatr != TA_UNDEF)
    {
/*      printf ("cre_msg: message_table[id].mbfatr = %d\n", message_table[id].mbfatr);	/* */
      return (E_OBJ);
    }

  message_table[id].total_size = pk_cmbf->bufsz;
  message_table[id].message_ptr = NULL;
  message_table[id].msgsz = pk_cmbf->maxmsz;
  message_table[id].bufsz = pk_cmbf->bufsz;
  message_table[id].mbfatr = pk_cmbf->mbfatr;
  return (E_OK);
}

/**************************************************************************************
 * del_mbf --- ��å������Хåե���������
 *
 * ������
 *	id	��å������Хåե� ID
 *
 * �֤��͡�
 *	ER	���顼�ֹ�
 *		E_OK	���ｪλ
 *		E_ID	��å����� ID ����
 *		E_NOEXS ��å������Хåե���¸�ߤ��ʤ�
 *
 * ��ǽ��
 *	���� ID �ǻ��ꤷ����å������Хåե��������롣
 *
 */
ER
del_mbf (ID id)
{
  T_TCB	*p;
  T_MSG	*msgp, *tmpp;

  if ((id < MIN_MSGBUF) || (id > MAX_MSGBUF))
    {
      return (E_ID);
    }
  if (message_table[id].mbfatr == TA_UNDEF)
    {
      return (E_NOEXS);
    }

  dis_int ();
  for (msgp = message_table[id].message_ptr; msgp != NULL; msgp = tmpp)
    {
      kfree (msgp->buf, msgp->size);
      tmpp = msgp->next;
      msgp->next = free_msg;
      free_msg = msgp;
    }

  /* �����Ԥ����������Ф��� E_DLT ���֤� */
  for (p = message_table[id].wait_recvtask; p != NULL; p = p->msg_next)
    {
      p->slp_err = E_DLT;
      p->tskwait.msg_wait = 0;
      wup_tsk (p->tskid);
    }
  message_table[id].wait_recvtask = NULL;

  /* �����Ԥ����������Ф��� E_DLT ���֤� */
  for (p = message_table[id].wait_sendtask; p != NULL; p = p->msg_next)
    {
      p->slp_err = E_DLT;
      p->tskwait.msg_wait = 0;
      wup_tsk (p->tskid);
    }
  message_table[id].wait_sendtask = NULL;
  message_table[id].mbfatr = TA_UNDEF;
  ena_int (); 
  return (E_OK);
}

/*********************************************************************************
 * snd_mbf --- ��å������Хåե�������
 *
 * ������
 *	id	��å������Хåե� ID
 *	size	������å������Υ�����
 *	msg	������å���������Ƭ���ɥ쥹
 *
 * �֤��͡�
 *	ER	���顼�ֹ�
 *
 * ��ǽ��
 *	
 */
ER
snd_mbf (ID id, INT size, VP msg)
{
  T_MSG	*newmsg;
  VP	buf;

  if ((id < MIN_MSGBUF) || (id > MAX_MSGBUF))
    {
      return (E_ID);
    }
  if (message_table[id].mbfatr == TA_UNDEF)
    {
      return (E_NOEXS);
    }

retry:
  newmsg = alloc_msg ();
  if ((newmsg == NULL) || (message_table[id].total_size < size))
    {
      /* ��å������Хåե����������ȤǤ��ʤ��ä��� */
      /* �����Ԥ��ꥹ�Ȥ����졢sleep ���롣	      */
      message_table[id].wait_sendtask = add_tcb_list (message_table[id].wait_sendtask,
						      run_task);
      run_task->tskwait.msg_wait = 1;
      slp_tsk ();
      goto retry;
    }

  buf = kalloc (size);
  if (buf == NULL)
    {
      dealloc_msg (newmsg);
      return (E_OBJ);
    }

  newmsg->buf = buf;
  newmsg->size = size;
  add_msg_list (&message_table[id], newmsg);
  bcopy (msg, buf, size);
  if (message_table[id].wait_recvtask)
    {
      T_TCB *p;

      /* �⤷�������Ԥ�������������С������ wakeup ���� */
      p = message_table[id].wait_recvtask;
      message_table[id].wait_recvtask = del_tcb_list (message_table[id].wait_recvtask, p);
      p->tskwait.msg_wait = 0;
      wup_tsk (p->tskid);
    }

  message_table[id].total_size -= size;
  return (E_OK);
}

/*********************************************************************************
 * snd_mbf --- ��å������Хåե�������
 *
 * ������
 *	id	��å������Хåե� ID
 *	size	������å������Υ�����
 *	msg	������å���������Ƭ���ɥ쥹
 *
 * �֤��͡�
 *	ER	���顼�ֹ�
 *
 * ��ǽ��
 *	
 */
ER
snd_mbf2 (ID id, INT size, VP msg)
{
  T_MSG	*newmsg;
  VP	buf;

  if ((id < MIN_MSGBUF) || (id > MAX_MSGBUF))
    {
      return (E_ID);
    }
  if (message_table[id].mbfatr == TA_UNDEF)
    {
      return (E_NOEXS);
    }

retry:
  newmsg = alloc_msg ();
  if ((newmsg == NULL) || (message_table[id].total_size < size))
    {
      /* ��å������Хåե����������ȤǤ��ʤ��ä��� */
      /* �����Ԥ��ꥹ�Ȥ����졢sleep ���롣	      */
      message_table[id].wait_sendtask = add_tcb_list (message_table[id].wait_sendtask,
						      run_task);
      run_task->tskwait.msg_wait = 1;
      slp_tsk ();
      goto retry;
    }

  buf = kalloc (size);
  if (buf == NULL)
    {
      printf ("snd_mbf2: can not allocate memory.\n");	/* */
      dealloc_msg (newmsg);
      return (E_OBJ);
    }

  newmsg->buf = buf;
  newmsg->size = size;
  add_msg_list (&message_table[id], newmsg);
  bcopy (msg, buf, size);

  if (message_table[id].wait_recvtask)
    {
      T_TCB *p;

      /* �⤷�������Ԥ�������������С������ wakeup ���� */
      p = message_table[id].wait_recvtask;
      message_table[id].wait_recvtask = del_tcb_list (message_table[id].wait_recvtask, p);
      p->tskwait.msg_wait = 0;
      printf ("snd_mbf2: wait task.(ID = %d\n", p->tskid);	/** */
      wup_tsk (p->tskid);
    }
  else
    {
      printf ("snd_mbf2: not wait task.\n");	/** */
    }

  message_table[id].total_size -= size;
  return (E_OK);
}


/*********************************************************************************
 * psnd_mbf --- ��å������Хåե�������(�ݡ����)
 *
 * ������
 *	id	��å������Хåե� ID
 *	size	������å������Υ�����
 *	msg	������å���������Ƭ���ɥ쥹
 *
 * �֤��͡�
 *	ER	���顼�ֹ�
 *
 * ��ǽ��
 *	
 */
ER
psnd_mbf (ID id, INT size, VP msg)
{
  T_MSG	*newmsg;
  VP	buf;

  if ((id < MIN_MSGBUF) || (id > MAX_MSGBUF))
    {
      return (E_ID);
    }
  if (message_table[id].mbfatr == TA_UNDEF)
    {
      return (E_NOEXS);
    }

  newmsg = alloc_msg ();
  if ((newmsg == NULL) || (message_table[id].total_size < size))
    {
      /* ��å������Хåե����������ȤǤ��ʤ��ä��� */
      /* E_TMOUT �Υ��顼���֤�			      */
      return (E_TMOUT);
    }

  buf = kalloc (size);
  if (buf == NULL)
    {
      dealloc_msg (newmsg);
      return (E_TMOUT);
    }

  newmsg->buf = buf;
  newmsg->size = size;
  add_msg_list (&message_table[id], newmsg);
  bcopy (msg, buf, size);
  if (message_table[id].wait_recvtask)
    {
      T_TCB *p;

      /* �⤷�������Ԥ�������������С������ wakeup ���� */
      p = message_table[id].wait_recvtask;
      message_table[id].wait_recvtask = del_tcb_list (message_table[id].wait_recvtask, p);
      p->tskwait.msg_wait = 0;
      wup_tsk (p->tskid);
    }

  message_table[id].total_size -= size;
  return (E_OK);
}

/*********************************************************************************
 * tsnd_func --- �����ॢ���Ȼ��֤��᤮�����˼¹Ԥ���ؿ�
 *
 * ������
 *	�����ॢ���Ȥ���������
 *
 * �֤��͡�
 *	�ʤ�
 *
 * ��ǽ��
 *	�ܴؿ��ϡ�tsnd_mbf �ˤ�äƥ�å������Хåե��Ԥ����Ƥ�����������
 *	�����ॢ���Ȥ������˸ƤӤ�����롣
 */
static void
tsnd_func (VP p)
{
  T_TCB *taskp;

  dis_int ();
  taskp = (T_TCB *)p;
  taskp->slp_err = E_TMOUT;
  taskp->tskwait.msg_wait = 0;
  wup_tsk (taskp->tskid);
  ena_int ();
}
/*********************************************************************************
 * tsnd_mbf --- ��å������Хåե�������(�����ॢ���ȵ�ǽ�Ĥ�)
 *
 * ������
 *	id	��å������Хåե� ID
 *	size	������å������Υ�����
 *	msg	������å���������Ƭ���ɥ쥹
 *	tmout	�����ॢ������
 *
 * �֤��͡�
 *	ER	���顼�ֹ�
 *
 * ��ǽ��
 *	
 */
ER
tsnd_mbf (ID id, INT size, VP msg, TMO tmout)
{
  T_MSG	*newmsg;
  VP	buf;

  if ((id < MIN_MSGBUF) || (id > MAX_MSGBUF))
    {
      return (E_ID);
    }
  if (message_table[id].mbfatr == TA_UNDEF)
    {
      return (E_NOEXS);
    }

retry:
  newmsg = alloc_msg ();
  if ((newmsg == NULL) || (message_table[id].total_size < size))
    {
      /* ��å������Хåե����������ȤǤ��ʤ��ä��� */
      /* �����Ԥ��ꥹ�Ȥ����졢sleep ���롣	      */
      set_timer (tmout, tsnd_func, run_task);
      message_table[id].wait_sendtask = add_tcb_list (message_table[id].wait_sendtask,
						      run_task);
      run_task->tskwait.msg_wait = 1;
      slp_tsk ();
      unset_timer (tsnd_func, run_task);
      if (run_task->slp_err)
	return (run_task->slp_err);
      goto retry;
    }

  buf = kalloc (size);
  if (buf == NULL)
    {
      dealloc_msg (newmsg);
      return (E_OBJ);
    }

  newmsg->buf = buf;
  newmsg->size = size;
  add_msg_list (&message_table[id], newmsg);
  bcopy (msg, buf, size);
  if (message_table[id].wait_recvtask)
    {
      T_TCB *p;

      /* �⤷�������Ԥ�������������С������ wakeup ���� */
      p = message_table[id].wait_recvtask;
      message_table[id].wait_recvtask = del_tcb_list (message_table[id].wait_recvtask, p);
      p->tskwait.msg_wait = 0;
      wup_tsk (p->tskid);
    }

  message_table[id].total_size -= size;
  return (E_OK);
}

/*********************************************************************************
 * rcv_mbf --- ��å������Хåե������������
 *
 * ������
 *	msg	����������å�������������ΰ�
 *	id	��å������Хåե� ID
 *	size	����������å������Υ�����(�֤���)
 *
 * �֤��͡�
 *	ER	���顼�ֹ�
 *
 * ��ǽ��
 *
 */
ER
rcv_mbf (VP msg, INT *size, ID id)
{
  T_MSG	*newmsgp;

  if ((id < MIN_MSGBUF) || (id > MAX_MSGBUF))
    {
      return (E_ID);
    }
  if (message_table[id].mbfatr == TA_UNDEF)
    {
      return (E_NOEXS);
    }

/*  printf ("(1)\n");	/* */

  /* ��å�������¸�ߤ��ʤ� --> sleep ���롣 */
  if (message_table[id].message_ptr == NULL)
    {
      message_table[id].wait_recvtask = add_tcb_list (message_table[id].wait_recvtask,
						      run_task);
      run_task->tskwait.msg_wait = 1;
      slp_tsk ();
    }


  /* ��å������ꥹ�Ȥ���1�ĥ���ȥ����Ф� */
  newmsgp = get_msg (&message_table[id]);
  /* ��å����������Ƥ򥳥ԡ����� */
  bcopy ((VP)newmsgp->buf, msg, newmsgp->size);
  *size = (INT)newmsgp->size;
  kfree (newmsgp->buf, *size);
  dealloc_msg (newmsgp);
  message_table[id].total_size += *size;
  return (E_OK);
}

/*********************************************************************************
 * prcv_mbf --- ��å������Хåե������������ (�ݡ����)
 *
 * ������
 *	msg	����������å�������������ΰ�
 *	id	��å������Хåե� ID
 *	size	����������å������Υ�����(�֤���)
 *
 * �֤��͡�
 *	ER	���顼�ֹ�
 *
 * ��ǽ��
 *
 */
ER
prcv_mbf (VP msg, INT *size, ID id)
{
  T_MSG	*newmsgp;

  if ((id < MIN_MSGBUF) || (id > MAX_MSGBUF))
    {
      return (E_ID);
    }
  if (message_table[id].mbfatr == TA_UNDEF)
    {
      return (E_NOEXS);
    }

  /* ��å�������¸�ߤ��ʤ� --> ���顼�꥿���󤹤� */
  if (message_table[id].message_ptr == NULL)
    {
      return (E_TMOUT);
    }

  /* ��å������ꥹ�Ȥ���1�ĥ���ȥ����Ф� */
  newmsgp = get_msg (&message_table[id]);

  /* ��å����������Ƥ򥳥ԡ����� */
  bcopy (newmsgp->buf, msg, newmsgp->size);
  *size = newmsgp->size;
 
  kfree (newmsgp->buf, *size);
  dealloc_msg (newmsgp);
  message_table[id].total_size += *size;
  return (E_OK);
}

/*********************************************************************************
 * trcv_func --- �����Ԥ��ξ��Υ����ॢ���ȴؿ�
 *
 */
static void
trcv_func (VP p)
{
  T_TCB *taskp;

  dis_int ();
  taskp = (T_TCB *)p;
  taskp->slp_err = E_TMOUT;
  taskp->tskwait.msg_wait = 0;
  wup_tsk (taskp->tskid);
  ena_int ();
}


/*********************************************************************************
 * trcv_mbf --- ��å������Хåե������������(�����ॢ����ͭ)
 *
 * ������
 *	msg	����������å�������������ΰ�
 *	id	��å������Хåե� ID
 *	size	����������å������Υ�����(�֤���)
 *
 * �֤��͡�
 *	ER	���顼�ֹ�
 *
 * ��ǽ��
 *
 */
ER
trcv_mbf (VP msg, INT *size, ID id, TMO tmout)
{
  T_MSG	*newmsgp;

  if ((id < MIN_MSGBUF) || (id > MAX_MSGBUF))
    {
      return (E_ID);
    }
  if (message_table[id].mbfatr == TA_UNDEF)
    {
      return (E_NOEXS);
    }

retry:
  /* ��å�������¸�ߤ��ʤ� --> sleep ���롣 */
  if (message_table[id].message_ptr == NULL)
    {
      dis_int ();
      set_timer (tmout, trcv_func, run_task);
      message_table[id].wait_recvtask = add_tcb_list (message_table[id].wait_recvtask,
						      run_task);
      ena_int ();
      run_task->tskwait.msg_wait = 1;
      slp_tsk ();
      unset_timer (tsnd_func, run_task);
      if (run_task->slp_err)
	return (run_task->slp_err);
      goto retry;
    }

  /* ��å������ꥹ�Ȥ���1�ĥ���ȥ����Ф� */
  newmsgp = get_msg (&message_table[id]);

  /* ��å����������Ƥ򥳥ԡ����� */
  bcopy (newmsgp->buf, msg, newmsgp->size);
  *size = newmsgp->size;
 
  kfree (newmsgp->buf, *size);
  dealloc_msg (newmsgp);
  message_table[id].total_size += *size;
  return (E_OK);
}

/****************************************************************************************
 * ref_mbf --- ��å������Хåե����ֻ���
 *
 * ������
 *	pk_rmbf	��å������Хåե����� (�֤���)
 *	id	��å����� ID
 */
ER
ref_mbf (T_RMBF *pk_rmbf, ID id)
{
  if ((id < MIN_MSGBUF) || (id > MAX_MSGBUF))
    {
      return (E_ID);
    }
  if (message_table[id].mbfatr == TA_UNDEF)
    {
      return (E_NOEXS);
    }

  if (message_table[id].wait_recvtask != NULL) 
    pk_rmbf->wtsk = TRUE;
  else
    pk_rmbf->wtsk = FALSE;

  if (message_table[id].wait_sendtask != NULL) 
    pk_rmbf->stsk = TRUE;
  else
    pk_rmbf->stsk = FALSE;
  pk_rmbf->frbufsz = message_table[id].total_size;
  return (E_OK);
}
