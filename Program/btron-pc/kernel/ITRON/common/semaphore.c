/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* semaphore.c --- ���ޥե���Ϣ
 *
 */


#include "itron.h"
#include "memory.h"
#include "errno.h"
#include "func.h"
#include "ibmpc.h"
#include "semaphore.h"

static T_SEMAPHORE	semaphore_table[MAX_SEMAID + 1];

static void		twaisem_timer (VP arg);
static void del_sem_wait(ID sid, ID tid);

/********************************************************************************
 * init_semaphore --- ���ޥե��ν����
 *
 */
ER
init_semaphore (void)
{
  /* ���ߤΤȤ����⤷�ʤ� */
  return (E_OK);
}

/********************************************************************************
 * cre_sem --- ���ޥե�����
 *
 * ������
 *	semid
 *	pkcsem
 *
 * �֤��͡�
 *	ER
 *
 * ��ǽ��
 *
 */
ER
cre_sem (ID semid, T_CSEM *pkcsem)
{
  if ((semid < MIN_SEMAID) || (semid > MAX_SEMAID))
    {
      return (E_ID);
    }
  
  if (semaphore_table[semid].maxsem > 0)
    {
      return (E_OBJ);
    }
  
  if ((pkcsem->maxsem <= 0) || (pkcsem->isemcnt < 0))
    {
      return (E_PAR);
    }

  if (pkcsem->sematr != TA_TFIFO)
    {
      return (E_PAR);
    }

  semaphore_table[semid].sematr = pkcsem->sematr;
  semaphore_table[semid].isemcnt = pkcsem->isemcnt;
  semaphore_table[semid].maxsem = pkcsem->maxsem;
  semaphore_table[semid].exinf = pkcsem->exinf;
  return (E_OK);
}

/********************************************************************************
 * del_sem --- ���ޥե����
 *
 * ������
 *	semid
 *
 * �֤��͡�
 *	ER	���顼�ֹ�
 *
 * ��ǽ��
 *	�����ǻ��ꤷ�����ޥե��������롣
 */
ER
del_sem (ID semid)
{
  T_TCB	*p;
#if 0
  T_TCB *next;
#endif

  if ((semid < MIN_SEMAID) || (semid > MAX_SEMAID))
    {
      return (E_ID);
    }
  
  if (semaphore_table[semid].maxsem <= 0)
    {
      return (E_OBJ);
    }
  
#ifdef CALL_HANDLER_IN_TASK
  dis_dsp();
#else
  dis_int();
#endif
  p = semaphore_table[semid].waitlist;
  while (p != NULL)
    {
      /* ���ޥե����ԤäƤ��륿������������� */
      /* ���顼�ֹ� E_DLT ���֤롣	      */
      p->slp_err = E_DLT;
      /* ���������Ԥ����֤��顢ready ���֤ذ�ư����  */
      /* ���������������ǥ����ѥå��Ϥ�����ʤ���    */
      p->tskwait.semaph_wait = 0;
      wup_tsk (p->tskid);
      p = p->sem_next;
    }
  semaphore_table[semid].waitlist = NULL;
  semaphore_table[semid].waittail = NULL;
  semaphore_table[semid].isemcnt = 0;
#ifdef CALL_HANDLER_IN_TASK
  ena_dsp();
#else
  ena_int();
#endif
  return (E_OK);
}

/********************************************************************************
 * sig_sem --- ���ޥե����ֵ�
 *
 * ������
 *	semid
 *
 * �֤��͡�
 *	ER	���顼�ֹ�
 *
 * ��ǽ��
 *	���δؿ��ϡ������륻�ޥե��� V ư���Ԥ���
 */
ER
sig_sem (ID semid)
{
  T_TCB	*p;
#if 0
  T_TCB *next;
#endif

  if ((semid < MIN_SEMAID) || (semid > MAX_SEMAID))
    {
      return (E_ID);
    }
  
  if (semaphore_table[semid].maxsem <= 0)
    {
      return (E_OBJ);
    }

#ifdef CALL_HANDLER_IN_TASK
  dis_dsp();
#else
  dis_int();
#endif
  p = semaphore_table[semid].waitlist;
  if (p != NULL)
    {
      printk ("sig_sem: wakeup task = %d\n", p->tskid);	/* */
#ifdef notdef
      semaphore_table[semid].waitlist = del_tcb_list (semaphore_table[semid].waitlist, p);
#else
      semaphore_table[semid].waitlist = p->sem_next;
      if (p->sem_next == NULL) {
	semaphore_table[semid].waittail = NULL;
      }
      p->sem_next = NULL;
#endif
#ifdef CALL_HANDLER_IN_TASK
      ena_dsp();
#else
      ena_int();
#endif
      p->tskwait.semaph_wait = 0;
      wup_tsk (p->tskid);
    }
  else
    {
      /* ���ޥե����ԤäƤ��륿�������ʤ� */
      semaphore_table[semid].isemcnt++;
    }
#ifdef CALL_HANDLER_IN_TASK
  ena_dsp();
#else
  ena_int();
#endif
  return (E_OK);
}

/********************************************************************************
 * wai_sem --- ���ޥե��񸻳���
 *
 * ������
 *	semid	�������륻�ޥե��� ID �ֹ�
 *
 * �֤��͡�
 *	ER	���顼�ֹ�
 *
 * ��ǽ��
 *	���δؿ��ϡ������륻�ޥե��� P ư���Ԥ���
 */
ER
wai_sem (ID semid)
{
  INT wcnt;
#if 0
  T_TCB	*p;
#endif

  if ((semid < MIN_SEMAID) || (semid > MAX_SEMAID))
    {
      return (E_ID);
    }
  
  /* ��������Ƥ��ʤ����ޥե����ä� */
  if (semaphore_table[semid].maxsem <= 0)
    {
      return (E_OBJ);
    }

  /* ���Ǥ˥��ޥե���������Ƥ��륿���������롣 */
  /* �Ԥ�����˼�ʬ���Ȥ�Ĥʤ���CPU ��������� */
  if (semaphore_table[semid].isemcnt == 0)
    {
#ifdef CALL_HANDLER_IN_TASK
      dis_dsp();
#else
      dis_int();
#endif
#ifdef notdef
      printk ("wai_sem: sleep task = %d\n", run_task->tskid);	/* */
#endif
      run_task->sem_id = semid;
#ifdef notdef
      semaphore_table[semid].waitlist = add_tcb_list (semaphore_table[semid].waitlist, run_task);
#else
      run_task->sem_next = NULL;
      if (semaphore_table[semid].waittail != NULL) {
	semaphore_table[semid].waittail->sem_next = run_task;
      }
      semaphore_table[semid].waittail = run_task;
      if (semaphore_table[semid].waitlist == NULL) {
	semaphore_table[semid].waitlist = run_task;
      }
#endif
#ifdef CALL_HANDLER_IN_TASK
      ena_dsp();
#else
      ena_int();
#endif
      run_task->tskwait.semaph_wait = 1;
      can_wup(&wcnt, run_task->tskid);
      slp_tsk ();	/* �Ԥ����֤ذ�ư */
    }
  else if (semaphore_table[semid].isemcnt >= 1)
    {
#ifdef CALL_HANDLER_IN_TASK
      dis_dsp();
#else
      dis_int();
#endif
      semaphore_table[semid].isemcnt--;
#ifdef CALL_HANDLER_IN_TASK
      ena_dsp();
#else
      ena_int();
#endif
    }
  run_task->sem_id = 0;
  return (E_OK);
}

/********************************************************************************
 * preq_sem --- ���ޥե��񸻳��� (�ݡ����)
 *
 * ������
 *	semid	�������륻�ޥե��� ID �ֹ�
 *
 * �֤��͡�
 *	ER	���顼�ֹ�
 *
 * ��ǽ��
 *	���δؿ��ϡ������륻�ޥե��� P ư���Ԥ���
 */
ER
preq_sem (ID semid)
{
#if 0
  T_TCB	*p;
#endif

  if ((semid < MIN_SEMAID) || (semid > MAX_SEMAID))
    {
      return (E_ID);
    }
  
  /* ��������Ƥ��ʤ����ޥե����ä� */
  if (semaphore_table[semid].maxsem <= 0)
    {
      return (E_OBJ);
    }

  /* ���Ǥ˥��ޥե���������Ƥ��륿���������롣 */
  if (semaphore_table[semid].isemcnt >= 1)
    {
#ifdef CALL_HANDLER_IN_TASK
      dis_dsp();
#else
      dis_int();
#endif
      semaphore_table[semid].isemcnt--;
#ifdef CALL_HANDLER_IN_TASK
      ena_dsp();
#else
      ena_int();
#endif
    }
  else if (semaphore_table[semid].isemcnt == 0)
    {
      return (E_TMOUT);	
    }
  return (E_OK);
}

/********************************************************************************
 * twai_sem --- ���ޥե��񸻳��� (�����ॢ����ͭ)
 *
 * ������
 *	semid	�������륻�ޥե��� ID �ֹ�
 *	tmout	�����ॢ���Ȼ���
 *
 * �֤��͡�
 *	ER	���顼�ֹ�
 *
 * ��ǽ��
 *	���δؿ��ϡ������륻�ޥե��� P ư���Ԥ���
 */
ER
twai_sem (ID semid, TMO tmout)
{
  INT wcnt;
#if 0
  T_TCB	*p;
#endif

  if ((semid < MIN_SEMAID) || (semid > MAX_SEMAID))
    {
      return (E_ID);
    }
  
  /* ��������Ƥ��ʤ����ޥե����ä� */
  if (semaphore_table[semid].maxsem <= 0)
    {
      return (E_OBJ);
    }

  /* ���Ǥ˥��ޥե���������Ƥ��륿���������롣 */
  /* �Ԥ�����˼�ʬ���Ȥ�Ĥʤ���CPU ��������� */
  if (semaphore_table[semid].isemcnt == 0)
    {	
      set_timer (tmout, (void (*)(VP))twaisem_timer, run_task);	/* �����ޡ��򥻥å� */
#ifdef CALL_HANDLER_IN_TASK
      dis_dsp();
#else
      dis_int();
#endif
#ifdef notdef
      semaphore_table[semid].waitlist = add_tcb_list (semaphore_table[semid].waitlist, run_task);
#else
      run_task->sem_next = NULL;
      if (semaphore_table[semid].waittail != NULL) {
	semaphore_table[semid].waittail->sem_next = run_task;
      }
      semaphore_table[semid].waittail = run_task;
      if (semaphore_table[semid].waitlist == NULL) {
	semaphore_table[semid].waitlist = run_task;
      }
#endif
      run_task->sem_id = semid;
#ifdef CALL_HANDLER_IN_TASK
      ena_dsp();
#else
      ena_int();
#endif
      run_task->tskwait.semaph_wait = 1;
      can_wup(&wcnt, run_task->tskid);
      slp_tsk ();	/* �Ԥ����֤ذ�ư */
      unset_timer ((void (*)(VP))twaisem_timer, (VP)(run_task->tskid));	/* �����ޡ��򥢥󥻥å� */
    }
  else if (semaphore_table[semid].isemcnt >= 1)
    {
#ifdef CALL_HANDLER_IN_TASK
      dis_dsp();
#else
      dis_int();
#endif
      semaphore_table[semid].isemcnt--;
#ifdef CALL_HANDLER_IN_TASK
      ena_dsp();
#else
      ena_int();
#endif
    }
  run_task->sem_id = 0;
  if (run_task->slp_err) {
    if (run_task->slp_err == E_TMOUT) {
      del_sem_wait(semid, run_task->tskid);
    }
    return (run_task->slp_err);
  }
  return (E_OK);
}

/******************************************************************************
 * twaisem_timer -- twai_sem �ؿ��λ����ڤ�ΤȤ��˸ƤӤ������ؿ���
 *
 * ���δؿ��ϡ������ߤα�Ĺ���ư����
 *
 */
static void
twaisem_timer (VP arg)
{
  T_TCB	*p;
  ID semid;

  dis_int();
  p = (T_TCB *)arg;
  p->slp_err = E_TMOUT;
#ifdef notdef
  semaphore_table[p->sem_id].waitlist = del_tcb_list (semaphore_table[p->sem_id].waitlist,
						      p);
#endif
#if 0
  /* ������ʬ�ν����ϲ�������*/
  semid = p->sem_id;
  p = semaphore_table[semid].waitlist;
  semaphore_table[semid].waitlist = p->sem_next;
  if (p->sem_next == NULL) {
    semaphore_table[semid].waittail = NULL;
  }
  p->sem_next = NULL;
#endif
  ena_int();
  p->tskwait.msg_wait = 0;
  wup_tsk (p->tskid);
}

/********************************************************************************
 * ref_sem --- ���ޥե����ֻ���
 *
 */
ER
ref_sem (T_RSEM *pk_rsem, ID semid)
{
  if ((semid < MIN_SEMAID) || (semid > MAX_SEMAID))
    {
      return (E_ID);
    }
  
  /* ��������Ƥ��ʤ����ޥե����ä� */
  if (semaphore_table[semid].maxsem <= 0)
    {
      return (E_OBJ);
    }

  pk_rsem->exinf = semaphore_table[semid].exinf;
  pk_rsem->wtsk  = semaphore_table[semid].waitlist ? TRUE : FALSE;
  pk_rsem->semcnt  = semaphore_table[semid].isemcnt;
  return (E_OK);
}



/* del_task_sem -- ���ޥե����饿��������
 *
 *
 */

static void del_sem_wait(ID sid, ID tid)
{
  T_TCB *p, *q;

  if (semaphore_table[sid].maxsem <= 0) return;
#ifdef CALL_HANDLER_IN_TASK
  dis_dsp();
#else
  dis_int();
#endif
  q = NULL;
  for (p = semaphore_table[sid].waitlist; p != NULL; p = p->sem_next) {
    if (p->tskid == tid) {
      if (p->sem_next == NULL) {
	semaphore_table[sid].waittail = q;
      }
      if (q == NULL) {
	semaphore_table[sid].waitlist = p->sem_next;
      }
      else {
	q->sem_next = p->sem_next;
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

void del_task_sem(ID tid)
{
  ID sid;

  for(sid = MIN_SEMAID; sid <= MAX_SEMAID; ++sid) {
    del_sem_wait(sid, tid);
  }
}
