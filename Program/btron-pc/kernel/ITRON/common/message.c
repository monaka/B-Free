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
#include "ibmpc.h"
#include "message.h"


static T_MSGHEAD		message_table[MAX_MSGBUF + 1];
static T_MSG			msgbuf[MAX_MSGENTRY];
static T_MSG			*free_msg;

static void del_sendtask(ID mid, ID tid);
static void del_recvtask(ID mid, ID tid);



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
  
#ifdef CALL_HANDLER_IN_TASK
  dis_dsp();
#else
  dis_int();
#endif

  p = free_msg;
  free_msg = free_msg->next;

#ifdef CALL_HANDLER_IN_TASK
  ena_dsp();
#else
  ena_int();
#endif
  return (p);
}

/****************************************************************************
 * dealloc_msg
 */
static void
dealloc_msg (T_MSG *p)
{
  if (p == NULL) return;
#ifdef CALL_HANDLER_IN_TASK
  dis_dsp();
#else
  dis_int();
#endif

  p->next = free_msg;
  free_msg = p;

#ifdef CALL_HANDLER_IN_TASK
  ena_dsp();
#else
  ena_int();
#endif
}

/***************************************************************************
 * add_msg_list --- ��å������ꥹ�Ȥ˥�å��������ɲä��롣
 *                  ��å������ꥹ�Ȥ� FIFO (���塼) 
 *
 */
static void
add_msg_list (T_MSGHEAD *list, T_MSG *newmsg)
{
#ifdef CALL_HANDLER_IN_TASK
  dis_dsp();
#else
  dis_int();
#endif
  if (list->last_message_ptr == NULL)
    {
      list->message_ptr = list->last_message_ptr = newmsg;
    }
  else
    {
      list->last_message_ptr->next = newmsg;
      list->last_message_ptr = newmsg;
      newmsg->next = NULL;
    }
#ifdef CALL_HANDLER_IN_TASK
  ena_dsp();
#else
  ena_int();
#endif
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

#ifdef CALL_HANDLER_IN_TASK
  dis_dsp();
#else
  dis_int();
#endif
  p = list->message_ptr;
  if (p == list->last_message_ptr)
    {
      list->message_ptr = list->last_message_ptr = NULL;
    }
  else
    {
      list->message_ptr = p->next;
    }
#ifdef CALL_HANDLER_IN_TASK
  ena_dsp();
#else
  ena_int();
#endif
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
#if 0
  T_MSG	*p;
#endif
  INT	i;

  free_msg = &msgbuf[0];
  for (i = 0; i < MAX_MSGENTRY-1; i++)
    {
      msgbuf[i].next = &msgbuf[i + 1];
    }
  msgbuf[MAX_MSGENTRY-1].next = NULL;

  for (i = 0; i <= MAX_MSGBUF; i++)
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
#if 0
      printk ("cre_msg: message_table[id].mbfatr = %d\n", message_table[id].mbfatr);	/* */
#endif
      return (E_OBJ);
    }

#ifdef CALL_HANDLER_IN_TASK
  dis_dsp();
#else
  dis_int();
#endif
  message_table[id].total_size = pk_cmbf->bufsz;
  message_table[id].message_ptr = NULL;
  message_table[id].msgsz = pk_cmbf->maxmsz;
  message_table[id].bufsz = pk_cmbf->bufsz;
  message_table[id].mbfatr = pk_cmbf->mbfatr;
#ifdef CALL_HANDLER_IN_TASK
  ena_dsp();
#else
  ena_int();
#endif
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

  for (msgp = message_table[id].message_ptr; msgp != NULL; msgp = tmpp)
    {
      kfree (msgp->buf, msgp->size);
      tmpp = msgp->next;
      dealloc_msg(msgp);
    }

  /* �����Ԥ����������Ф��� E_DLT ���֤� */
  for (p = message_table[id].wait_recvtask; p != NULL; p = p->msg_next)
    {
      p->slp_err = E_DLT;
      p->tskwait.msg_wait = 0;
      wup_tsk (p->tskid);
    }
#ifdef CALL_HANDLER_IN_TASK
  dis_dsp();
#else
  dis_int();
#endif
  message_table[id].wait_recvtask = NULL;
  message_table[id].wait_recvtail = NULL;
#ifdef CALL_HANDLER_IN_TASK
  ena_dsp();
#else
  ena_int();
#endif

  /* �����Ԥ����������Ф��� E_DLT ���֤� */
  for (p = message_table[id].wait_sendtask; p != NULL; p = p->msg_next)
    {
      p->slp_err = E_DLT;
      p->tskwait.msg_wait = 0;
      wup_tsk (p->tskid);
    }
#ifdef CALL_HANDLER_IN_TASK
  dis_dsp();
#else
  dis_int();
#endif
  message_table[id].wait_sendtask = NULL;
  message_table[id].wait_sendtail = NULL;
  message_table[id].mbfatr = TA_UNDEF;
#ifdef CALL_HANDLER_IN_TASK
  ena_dsp(); 
#else
  ena_int(); 
#endif
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
  INT wcnt;
  BOOL tsksw = FALSE;

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
#ifdef notdef
      message_table[id].wait_sendtask = add_tcb_list (message_table[id].wait_sendtask,
						      run_task);
#else
#ifdef CALL_HANDLER_IN_TASK
      dis_dsp();
#else
      dis_int();
#endif
      if (message_table[id].wait_sendtail != run_task) {
	run_task->msg_next = NULL;
	if (message_table[id].wait_sendtail != NULL) {
	  message_table[id].wait_sendtail->msg_next = run_task;
	}
	message_table[id].wait_sendtail = run_task;
	if (message_table[id].wait_sendtask == NULL) {
	  message_table[id].wait_sendtask = run_task;
	}
      }
#ifdef CALL_HANDLER_IN_TASK
      ena_dsp();
#else
      ena_int();
#endif
#endif
#ifdef notdef
      if (run_task->tskid == 23) {
	printk("\nsnd_mbf sleep %d\n", run_task->tskid);
      }
#endif
      run_task->tskwait.msg_wait = 1;
      can_wup(&wcnt, run_task->tskid);
      slp_tsk ();
#ifdef notdef
      if (run_task->tskid == 23) {
	printk("\nsnd_mbf waked up %d\n", run_task->tskid);
      }
#endif
      dealloc_msg (newmsg);
      if (run_task->slp_err) {
	return (run_task->slp_err);
      }
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
  message_table[id].total_size -= size;
  bcopy (msg, buf, size);

  if (message_table[id].wait_recvtask)
    {
      T_TCB *p;

      /* �⤷�������Ԥ�������������С������ wakeup ���� */
      p = message_table[id].wait_recvtask;
#ifdef notdef
      message_table[id].wait_recvtask = del_tcb_list (message_table[id].wait_recvtask, p);
#else
#ifdef CALL_HANDLER_IN_TASK
      dis_dsp();
#else
      dis_int();
#endif
      message_table[id].wait_recvtask = p->msg_next;
      if (p->msg_next == NULL) {
	message_table[id].wait_recvtail = NULL;
      }
      p->msg_next = NULL;
#ifdef CALL_HANDLER_IN_TASK
      ena_dsp();
#else
      ena_int();
#endif
#endif
      p->tskwait.msg_wait = 0;
#ifdef notdef
      if (p->tskid == 23) {
	printk("\nsnd_mbf wake up %d(%d)\n", p->tskid, run_task->tskid);
      }
#endif
      tsksw = TRUE;
      wup_tsk (p->tskid);
    }

#ifdef notdef
  if (tsksw == TRUE)
    task_switch(TRUE);
#endif
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
  INT wcnt;

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
#ifdef notdef
      message_table[id].wait_sendtask = add_tcb_list (message_table[id].wait_sendtask,
						      run_task);
#else
#ifdef CALL_HANDLER_IN_TASK
      dis_dsp();
#else
      dis_int();
#endif
      if (message_table[id].wait_sendtail != run_task) {
	run_task->msg_next = NULL;
	if (message_table[id].wait_sendtail != NULL) {
	  message_table[id].wait_sendtail->msg_next = run_task;
	}
	message_table[id].wait_sendtail = run_task;
	if (message_table[id].wait_sendtask == NULL) {
	  message_table[id].wait_sendtask = run_task;
	}
      }
#ifdef CALL_HANDLER_IN_TASK
      ena_dsp();
#else
      ena_int();
#endif
#endif
      run_task->tskwait.msg_wait = 1;
      can_wup(&wcnt, run_task->tskid);
      slp_tsk ();
      dealloc_msg (newmsg);
      if (run_task->slp_err) {
	return (run_task->slp_err);
      }
      goto retry;
    }

  buf = kalloc (size);
  if (buf == NULL)
    {
      printk ("snd_mbf2: can not allocate memory.\n");	/* */
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
#ifdef notdef
      message_table[id].wait_recvtask = del_tcb_list (message_table[id].wait_recvtask, p);
#else
#ifdef CALL_HANDLER_IN_TASK
      dis_dsp();
#else
      dis_int();
#endif
      message_table[id].wait_recvtask = p->msg_next;
      if (p->msg_next == NULL) {
	message_table[id].wait_recvtail = NULL;
      }
      p->msg_next = NULL;
#ifdef CALL_HANDLER_IN_TASK
      ena_dsp();
#else
      ena_int();
#endif
#endif
      p->tskwait.msg_wait = 0;
      printk ("snd_mbf2: wait task.(ID = %d\n", p->tskid);	/** */
      wup_tsk (p->tskid);
    }
  else
    {
      printk ("snd_mbf2: not wait task.\n");	/** */
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
  BOOL tsksw = FALSE;

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
  message_table[id].total_size -= size;
  bcopy (msg, buf, size);

  if (message_table[id].wait_recvtask)
    {
      T_TCB *p;

      /* �⤷�������Ԥ�������������С������ wakeup ���� */
      p = message_table[id].wait_recvtask;
#ifdef notdef
      message_table[id].wait_recvtask = del_tcb_list (message_table[id].wait_recvtask, p);
#else
#ifdef CALL_HANDLER_IN_TASK
      dis_dsp();
#else
      dis_int();
#endif
      message_table[id].wait_recvtask = p->msg_next;
      if (p->msg_next == NULL) {
	message_table[id].wait_recvtail = NULL;
      }
      p->msg_next = NULL;
#ifdef CALL_HANDLER_IN_TASK
      ena_dsp();
#else
      ena_int();
#endif
#endif
      p->tskwait.msg_wait = 0;
      tsksw = TRUE;
      wup_tsk (p->tskid);
    }

#ifdef notdef
  if (tsksw == TRUE)
    task_switch(TRUE);
#endif
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

  taskp = (T_TCB *)p;
#ifdef CALL_HANDLER_IN_TASK
  dis_dsp();
#else
  dis_int ();
#endif
  taskp->slp_err = E_TMOUT;
  taskp->tskwait.msg_wait = 0;
#ifdef CALL_HANDLER_IN_TASK
  ena_dsp();
#else
  ena_int ();
#endif
  wup_tsk (taskp->tskid);
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
  INT wcnt;
  BOOL tsksw = FALSE;

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
      set_timer (tmout, (void (*)(VP))tsnd_func, run_task);
#ifdef notdef
      message_table[id].wait_sendtask = add_tcb_list (message_table[id].wait_sendtask,
						      run_task);
#else
#ifdef CALL_HANDLER_IN_TASK
      dis_dsp();
#else
      dis_int();
#endif
      if (message_table[id].wait_sendtail != run_task) {
	run_task->msg_next = NULL;
	if (message_table[id].wait_sendtail != NULL) {
	  message_table[id].wait_sendtail->msg_next = run_task;
	}
	message_table[id].wait_sendtail = run_task;
	if (message_table[id].wait_sendtask == NULL) {
	  message_table[id].wait_sendtask = run_task;
	}
      }
#ifdef CALL_HANDLER_IN_TASK
      ena_dsp();
#else
      ena_int();
#endif
#endif
#ifdef notdef
      if (run_task->tskid == 23) {
	printk("\ntsnd_mbf sleep %d\n", run_task->tskid);
      }
#endif
      run_task->tskwait.msg_wait = 1;
      can_wup(&wcnt, run_task->tskid);
      slp_tsk ();
      unset_timer ((void (*)(VP))tsnd_func, run_task);
#ifdef notdef
      if (run_task->tskid == 23) {
	printk("\ntsnd_mbf waked up %d %d\n",
	       run_task->tskid, run_task->slp_err);
      }
#endif
      dealloc_msg (newmsg);
      if (run_task->slp_err) {
	if (run_task->slp_err == E_TMOUT) {
	  del_sendtask(id, run_task->tskid);
	}
	return (run_task->slp_err);
      }
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
  message_table[id].total_size -= size;
  bcopy (msg, buf, size);

  if (message_table[id].wait_recvtask)
    {
      T_TCB *p;

      /* �⤷�������Ԥ�������������С������ wakeup ���� */
      p = message_table[id].wait_recvtask;
#ifdef notdef
      message_table[id].wait_recvtask = del_tcb_list (message_table[id].wait_recvtask, p);
#else
#ifdef CALL_HANDLER_IN_TASK
      dis_dsp();
#else
      dis_int();
#endif
      message_table[id].wait_recvtask = p->msg_next;
      if (p->msg_next == NULL) {
	message_table[id].wait_recvtail = NULL;
      }
      p->msg_next = NULL;
#ifdef CALL_HANDLER_IN_TASK
      ena_dsp();
#else
      ena_int();
#endif
#endif
      p->tskwait.msg_wait = 0;
#ifdef notdef
      if (p->tskid == 23) {
	printk("\ntsnd_mbf wake up %d(%d)\n", p->tskid, run_task->tskid);
      }
#endif
      tsksw = TRUE;
      wup_tsk (p->tskid);
    }

#ifdef notdef
  if (tsksw == TRUE)
    task_switch(TRUE);
#endif
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
  INT wcnt;

  if ((id < MIN_MSGBUF) || (id > MAX_MSGBUF))
    {
      return (E_ID);
    }
  if (message_table[id].mbfatr == TA_UNDEF)
    {
      return (E_NOEXS);
    }

#if 0
  printk ("(1)\n");	/* */
#endif

  /* ��å�������¸�ߤ��ʤ� --> sleep ���롣 */
 retry:
  if (message_table[id].message_ptr == NULL)
    {
#ifdef notdef
      message_table[id].wait_recvtask = add_tcb_list (message_table[id].wait_recvtask,
						      run_task);
#else
#ifdef CALL_HANDLER_IN_TASK
      dis_dsp();
#else
      dis_int();
#endif
      run_task->msg_next = NULL;
      if (message_table[id].wait_recvtail != NULL) {
	message_table[id].wait_recvtail->msg_next = run_task;
      }
      message_table[id].wait_recvtail = run_task;
      if (message_table[id].wait_recvtask == NULL) {
	message_table[id].wait_recvtask = run_task;
      }
#ifdef CALL_HANDLER_IN_TASK
      ena_dsp();
#else
      ena_int();
#endif
#endif
#ifdef notdef
      if (run_task->tskid == 23) {
	printk("\nrcv_mbf sleep %d\n", run_task->tskid);
      }
#endif
      run_task->tskwait.msg_wait = 1;
      can_wup(&wcnt, run_task->tskid);
      slp_tsk ();
#ifdef notdef
      if (run_task->tskid == 23) {
	printk("\nrcv_mbf waked up %d\n", run_task->tskid);
      }
#endif
      if (run_task->slp_err) {
	return (run_task->slp_err);
      }
      goto retry;
    }


  /* ��å������ꥹ�Ȥ���1�ĥ���ȥ����Ф� */
  newmsgp = get_msg (&message_table[id]);
  /* ��å����������Ƥ򥳥ԡ����� */
  bcopy ((VP)newmsgp->buf, msg, newmsgp->size);
  *size = (INT)newmsgp->size;
  kfree (newmsgp->buf, *size);
  dealloc_msg (newmsgp);
  message_table[id].total_size += *size;

  /* �����Ԥ��Υ������������ wakeup ���� */
  if (message_table[id].wait_sendtask) {
    T_TCB *p;

    p = message_table[id].wait_sendtask;
#ifdef CALL_HANDLER_IN_TASK
    dis_dsp();
#else
    dis_int();
#endif
    message_table[id].wait_sendtask = p->msg_next;
    if (p->msg_next == NULL) {
      message_table[id].wait_sendtail = NULL;
    }
    p->msg_next = NULL;
#ifdef CALL_HANDLER_IN_TASK
    ena_dsp();
#else
    ena_int();
#endif
    p->tskwait.msg_wait = 0;
#ifdef notdef
    if (p->tskid == 23) {
      printk("\nrcv_mbf wake up %d(%d)\n", p->tskid, run_task->tskid);
    }
#endif
    wup_tsk (p->tskid);
  }
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

  /* �����Ԥ��Υ������������ wakeup ���� */
  if (message_table[id].wait_sendtask) {
    T_TCB *p;

    p = message_table[id].wait_sendtask;
#ifdef CALL_HANDLER_IN_TASK
    dis_dsp();
#else
    dis_int();
#endif
    message_table[id].wait_sendtask = p->msg_next;
    if (p->msg_next == NULL) {
      message_table[id].wait_sendtail = NULL;
    }
    p->msg_next = NULL;
#ifdef CALL_HANDLER_IN_TASK
    ena_dsp();
#else
    ena_int();
#endif
    p->tskwait.msg_wait = 0;
    wup_tsk (p->tskid);
  }
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

  taskp = (T_TCB *)p;
#ifdef notdef
  printk("\ntrcv_func %d\n", taskp->tskid);
#endif
#ifdef CALL_HANDLER_IN_TASK
  dis_dsp();
#else
  dis_int();
#endif
  taskp->slp_err = E_TMOUT;
  taskp->tskwait.msg_wait = 0;
#ifdef CALL_HANDLER_IN_TASK
  ena_dsp();
#else
  ena_int();
#endif
  wup_tsk (taskp->tskid);
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
  INT wcnt;

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
      set_timer (tmout, (void (*)(VP))trcv_func, run_task);
#ifdef notdef
      message_table[id].wait_recvtask = add_tcb_list (message_table[id].wait_recvtask,
						      run_task);
#else
#ifdef CALL_HANDLER_IN_TASK
      dis_dsp();
#else
      dis_int();
#endif
      if (message_table[id].wait_recvtail != run_task) {
	run_task->msg_next = NULL;
	if (message_table[id].wait_recvtail != NULL) {
	  message_table[id].wait_recvtail->msg_next = run_task;
	}
	message_table[id].wait_recvtail = run_task;
	if (message_table[id].wait_recvtask == NULL) {
	  message_table[id].wait_recvtask = run_task;
	}
      }
#ifdef CALL_HANDLER_IN_TASK
      ena_dsp();
#else
      ena_int();
#endif
#endif
#ifdef notdef
      printk("\ntrcv_mbf sleep #%d %d\n", id, run_task->tskid);
#endif
      run_task->tskwait.msg_wait = 1;
      can_wup(&wcnt, run_task->tskid);
      slp_tsk ();
      unset_timer ((void (*)(VP))trcv_func, run_task);
#ifdef notdef
      printk("\ntrcv_mbf waked up #%d %d E(%d)\n",
	     id, run_task->tskid, run_task->slp_err);
#endif
      if (run_task->slp_err) {
	if (run_task->slp_err == E_TMOUT) {
	  del_recvtask(id, run_task->tskid);
	}
	return (run_task->slp_err);
      }
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

  /* �����Ԥ��Υ������������ wakeup ���� */
  if (message_table[id].wait_sendtask) {
    T_TCB *p;

    p = message_table[id].wait_sendtask;
#ifdef CALL_HANDLER_IN_TASK
    dis_dsp();
#else
    dis_int();
#endif
    message_table[id].wait_sendtask = p->msg_next;
    if (p->msg_next == NULL) {
      message_table[id].wait_sendtail = NULL;
    }
    p->msg_next = NULL;
#ifdef CALL_HANDLER_IN_TASK
    ena_dsp();
#else
    ena_int();
#endif
    p->tskwait.msg_wait = 0;
    wup_tsk (p->tskid);
  }
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

/* del_task_mbf -- ��å������Хåե����饿��������
 *
 *
 */

static void del_recvtask(ID mid, ID tid)
{
  T_TCB *p, *q;

  /* �⤷�������Ԥ��������� tid �Ǥ���С���������� */
  if (message_table[mid].mbfatr == TA_UNDEF) return;
#ifdef CALL_HANDLER_IN_TASK
  dis_dsp();
#else
  dis_int();
#endif
  q = NULL;
  for (p = message_table[mid].wait_recvtask; p != NULL; p = p->msg_next) {
    if (p->tskid == tid) {
      if (p->msg_next == NULL) {
	message_table[mid].wait_recvtail = q;
      }
      if (q == NULL) {
	message_table[mid].wait_recvtask = p->msg_next;
      }
      else {
	q->msg_next = p->msg_next;
      }
    }
    else {
      q = p;
    }
  }
#ifdef CALL_HANDLER_IN_TASK
  ena_dsp();
#else
  ena_int();
#endif
}

static void del_sendtask(ID mid, ID tid)
{
  T_TCB *p, *q;

  if (message_table[mid].mbfatr == TA_UNDEF) return;
  /* �⤷�������Ԥ��������� tid �Ǥ���С���������� */
#ifdef CALL_HANDLER_IN_TASK
  dis_dsp();
#else
  dis_int();
#endif
  q = NULL;
  for (p = message_table[mid].wait_sendtask; p != NULL; p = p->msg_next) {
    if (p->tskid == tid) {
      if (p->msg_next == NULL) {
	message_table[mid].wait_sendtail = q;
      }
      if (q == NULL) {
	message_table[mid].wait_sendtask = p->msg_next;
      }
      else {
	q->msg_next = p->msg_next;
      }
    }
    else {
      q = p;
    }
  }
#ifdef CALL_HANDLER_IN_TASK
  ena_dsp();
#else
  ena_int();
#endif
}

void del_task_mbf(ID tid)
{
  ID mid;

  for(mid = MIN_MSGBUF; mid <= MAX_MSGBUF; ++mid) {
    /* �⤷�������Ԥ��������� tid �Ǥ���С���������� */
    del_recvtask(mid, tid);

    /* �⤷�������Ԥ��������� tid �Ǥ���С���������� */
    del_sendtask(mid, tid);
  }
}
