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
#include "pc98.h"
#include "semaphore.h"

static T_SEMAPHORE	semaphore_table[NSEMAPHORE];

static void		twaisem_timer (VP arg);

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
  T_TCB	*p, *next;

  if ((semid < MIN_SEMAID) || (semid > MAX_SEMAID))
    {
      return (E_ID);
    }
  
  if (semaphore_table[semid].maxsem <= 0)
    {
      return (E_OBJ);
    }
  
  dis_dsp ();
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
  semaphore_table[semid].isemcnt = 0;
  ena_dsp ();
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
  T_TCB	*p, *next;

  if ((semid < MIN_SEMAID) || (semid > MAX_SEMAID))
    {
      return (E_ID);
    }
  
  if (semaphore_table[semid].maxsem <= 0)
    {
      return (E_OBJ);
    }

  dis_int ();
  p = semaphore_table[semid].waitlist;
  if (p != NULL)
    {
      printf ("sig_sem: wakeup task = %d\n", p->tskid);	/* */
      semaphore_table[semid].waitlist = del_tcb_list (semaphore_table[semid].waitlist, p);
      ena_int ();
      p->tskwait.semaph_wait = 0;
      wup_tsk (p->tskid);
    }
  else
    {
      /* ���ޥե����ԤäƤ��륿�������ʤ� */
      semaphore_table[semid].isemcnt++;
    }
  ena_int ();
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
  T_TCB	*p;

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
      dis_int ();
      printf ("wai_sem: sleep task = %d\n", run_task->tskid);	/* */
      run_task->sem_id = semid;
      semaphore_table[semid].waitlist = add_tcb_list (semaphore_table[semid].waitlist, run_task);
      ena_int ();
      run_task->tskwait.semaph_wait = 1;
      slp_tsk ();	/* �Ԥ����֤ذ�ư */
    }
  else if (semaphore_table[semid].isemcnt >= 1)
    {
      dis_int ();
      semaphore_table[semid].isemcnt--;
      ena_int ();
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
  T_TCB	*p;

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
      dis_int ();
      semaphore_table[semid].isemcnt--;
      ena_int ();
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
  T_TCB	*p;

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
      set_timer (tmout, twaisem_timer, run_task);	/* �����ޡ��򥻥å� */
      dis_int ();
      semaphore_table[semid].waitlist = add_tcb_list (semaphore_table[semid].waitlist, run_task);
      run_task->sem_id = semid;
      ena_int ();
      run_task->tskwait.semaph_wait = 1;
      slp_tsk ();	/* �Ԥ����֤ذ�ư */
      unset_timer (twaisem_timer, (VP)(run_task->tskid));	/* �����ޡ��򥢥󥻥å� */
    }
  else if (semaphore_table[semid].isemcnt >= 1)
    {
      dis_int ();
      semaphore_table[semid].isemcnt--;
      ena_int ();
    }
  run_task->sem_id = 0;
  if (run_task->slp_err)
    return (run_task->slp_err);
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

  dis_int ();
  p = (T_TCB *)arg;
  p->slp_err = E_TMOUT;
  semaphore_table[p->sem_id].waitlist = del_tcb_list (semaphore_table[p->sem_id].waitlist,
						      p);
  ena_int ();
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



