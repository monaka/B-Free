/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/common/syscall-if.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/common/syscall-if.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/* syscall-if.c --- �����ƥॳ����Υ��󥿥ե������ؿ�
 *
 * $Revision: 1.1 $
 * $Log: syscall-if.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.24  2000-07-31 18:55:47  kishida0
 * add debug I/F
 *
 * Revision 1.23  2000/04/03 14:34:45  naniwa
 * to call timer handler in task
 *
 * Revision 1.22  2000/02/06 09:10:55  naniwa
 * minor fix
 *
 * Revision 1.21  1999/11/14 14:53:36  naniwa
 * add time management function
 *
 * Revision 1.20  1999/11/10 10:29:06  naniwa
 * to support execve, etc
 *
 * Revision 1.19  1999/07/30 08:18:29  naniwa
 * add vcpy_stk()
 *
 * Revision 1.18  1999/07/24 04:33:23  naniwa
 * add pmemstat
 *
 * Revision 1.17  1999/07/09 08:18:10  naniwa
 * modified sys_vsys_msc
 *
 * Revision 1.16  1999/04/18 17:48:31  monaka
 * Port-manager and libkernel.a is moved to ITRON. I guess it is reasonable. At least they should not be in BTRON/.
 *
 * Revision 1.15  1999/04/12 15:29:15  monaka
 * pointers to void are renamed to VP.
 *
 * Revision 1.14  1999/04/12 14:51:48  monaka
 * Added Redirection to virtual memory functions.
 *
 * Revision 1.13  1999/04/12 13:29:23  monaka
 * printf() is renamed to printk().
 *
 * Revision 1.12  1999/03/16 13:02:52  monaka
 * Modifies for source cleaning. Most of these are for avoid gcc's -Wall message.
 *
 * Revision 1.11  1998/02/25 12:40:31  night
 * vmap_reg () �ΰ����ο����ҤȤ����������Ȥˤ���ѹ���
 *
 * Revision 1.10  1998/02/16 14:15:19  night
 * sys_vget_phs() �ؿ����ɲá�
 * ���δؿ��ϡ������ǻ��ꤷ�����ۥ��ɥ쥹�˥ޥåԥ󥰤���Ƥ���
 * ʪ���ڡ����Υ��ɥ쥹���֤���
 *
 * Revision 1.9  1997/08/31 14:11:46  night
 * lowlib �ط��ν������ɲ�
 *
 * Revision 1.8  1997/07/02 13:24:47  night
 * vput_reg �����Τ���Ͽ
 *
 * Revision 1.7  1997/05/12 14:36:02  night
 * vget_reg �����ƥॳ���뤬�ƤФ줿���� vget_reg() �μ��Τ�Ƥ֤褦����
 * ����
 * ���ޤǤϡ�E_NOSPT �Υ��顼���֤��Ƥ�����
 *
 * Revision 1.6  1997/03/25 13:30:47  night
 * �ؿ��Υץ�ȥ�����������ɲä���Ӱ�����������ν���
 *
 * Revision 1.5  1996/11/07  15:42:38  night
 * vset_cns �ΰ����λ�����ˡ���ְ�äƤ����Τǽ���������
 *
 * Revision 1.4  1996/11/07  12:41:28  night
 * �ؿ� sys_vsys_msc () ���ɲá�
 * ���δؿ��ϥ����ƥॳ���� vsys_msc �˻��Ѥ��롣
 *
 * Revision 1.3  1996/11/06  12:41:00  night
 * sys_vset_cns() ���ɲá�
 *
 * Revision 1.2  1996/09/11  18:27:12  night
 * sys_vget_csl() ���ɲá�
 * ���δؿ��ϡ����ߤΥ���������֤�ƤӽФ������֤������ƥॳ���롣
 *
 * Revision 1.1  1996/07/22  13:39:17  night
 * IBM PC �� ITRON �κǽ����Ͽ
 *
 * Revision 1.11  1995/12/13 16:02:03  night
 * vcre_reg, vmap_reg �Υ����ƥॳ������б�����ؿ���ƤӽФ��褦��
 * ����������
 *
 * Revision 1.10  1995/12/13 15:24:10  night
 * get_tid �����ƥॳ����μ¹Ի��˽��Ϥ��Ƥ����ǥХå��Ѥ� print ʸ���
 * ��������
 *
 * Revision 1.9  1995/10/01  12:57:18  night
 * def_int �����ƥॳ����¹Ի��Υ���å��������ɲá�
 *
 * Revision 1.8  1995/09/21  15:51:11  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.7  1995/09/19  18:03:35  night
 * �����ߥϥ�ɥ����Ͽ���륷���ƥॳ���� sys_def_int() ���ѹ���
 * ��Ͽ�Τ���δؿ��ƤӽФ��� set_idt() ���� set_interrupt_entry() ���ѹ�
 * ������
 *
 * Revision 1.6  1995/03/18  13:59:40  night
 * def_int �����ƥॳ����Τ����������ɲá�
 *
 * Revision 1.5  1995/02/26  13:56:43  night
 * ���ۥ�������ط��Υ����ƥॳ���륤�󥿥ե������ؿ����ɲá�
 * �ʲ��δؿ����ɲä�����
 *
 * 	sys_vcre_reg, sys_vdel_reg, sys_vmap_reg, sys_vunm_reg,
 * 	sys_vdup_reg, sys_vprt_reg, sys_vshr_reg, sys_vput_reg,
 * 	sys_vget_reg, sys_vsts_reg
 *
 * Revision 1.4  1995/01/18  15:12:49  night
 * �ʲ��δؿ����ɲá�
 *
 *   sys_dis_int (), sys_ena_int (), sys_cre_mpl (), sys_del_mpl (),
 *   sys_get_blk (), sys_pget_blk (), sys_rel_blk (), sys_ref_mpl (),
 *   sys_tget_blk (), sys_get_ver ()
 *
 * Revision 1.3  1995/01/10  13:50:50  night
 * �ʲ��Υ����ƥॳ���륤�󥿥ե������ؿ����ɲá�
 *
 * sys_cre_flg, sys_del_flg, sys_set_flg, sys_clr_flg,
 * sys_wai_flg, sys_pol_flg, sys_twai_flg, sys_ref_flg,
 * sys_cre_mbf, sys_del_mbf, sys_snd_mbf, sys_psnd_mbf,
 * sys_tsnd_mbf, sys_rcv_mbf, sys_prcv_mbf, sys_trcv_mbf
 *
 * Revision 1.2  1994/11/19  14:45:17  night
 * �ʲ��Υ����ƥॳ���륤�󥿥ե������Ѵؿ����ɲá�
 *
 *   sys_sus_tsk,sys_rsm_tsk,sys_frsm_tsk,sys_slp_tsk,
 *   sys_wup_tsk,sys_can_wup,sys_cre_sem,sys_del_sem,
 *   sys_sig_sem,sys_wai_sem,sys_preq_sem,sys_twai_sem,
 *   sys_ref_sem
 *
 * �����δؿ��ϳ����ߥ����å����饷���ƥॳ����ΰ�������Ф���
 * �ºݤ˽����򤹤�ؿ� ('sys_' ��������ؿ�) ��ƤӽФ���
 * ����Ū�ˤϲ��Τ褦�ʷ����δؿ��Ȥʤ롣
 *
 *  ER
 *  sys_foo (VOID *stackp)
 *  {
 *    struct arg
 *      {
 *        W	bar;
 *        W	baz;
 *      } *argp = (struct arg *)stackp;
 *
 *    return (foo (argp->bar, argp->baz));
 *  }
 *
 * Revision 1.1  1994/09/20  16:46:19  night
 * �����������ؿ��������
 *
 *
 * ������
 * �ܥե�����˴ޤޤ�Ƥ���ؿ��ϡ��桼����٥뤫�饷���ƥॳ���뤬
 * ȯ�Ԥ��줿���˥����ƥॳ����ؿ��Ȥ��ƸƤФ�롣
 * �����δؿ��ϡ������Υ����å���Ԥ����ºݤν����򤹤�ؿ���ƤӽФ���
 *
 * �����ƥॳ���륤�󥿥ե������ؿ��ϡ������Ȥ��ƥ桼�������å�����Ƭ���Ϥ�
 * ��餦��
 *
 */

#include "itron.h"
#include "errno.h"
#include "func.h"
#include "syscall.h"
#include "interrupt.h"
#include "../kernlib/device.h"

/* ----------------------------------------------------------------------- *
 * �������ط������ƥॳ����                                                *
 * ----------------------------------------------------------------------- */

/* sys_cre_tsk --- ������������
 *
 * ����: tskid 		�������륿������ID
 *	 pk_ctsk	�������륿������°������
 *			tskatr		������°��
 *			startaddr	��������ư���ɥ쥹
 *			itskpri		��������ư��ͥ����
 *			stksz		�����å�������
 *			addrmap		���ɥ쥹�ޥå�
 *	
 */
ER
sys_cre_tsk (VP argp)
{
  struct 
    {
      ID	tskid;
      T_CTSK	*tskpkt;
    } *args = argp;

  return (cre_tsk (args->tskid, args->tskpkt));
}

/* sys_del_tsk --- ���ꤷ������������
 *
 * ������tskid	������륿������ ID
 *
 */
ER
sys_del_tsk (VP argp)
{
  struct 
    {
      ID	tskid;
    } *args = argp;

  return (del_tsk (args->tskid));
}


/* sys_sta_tsk --- �������ξ��֤���Ф�
 *
 * ������tskid	���֤���Ф������� ID
 *	 stacd  ���֤���Ф��ΰ�
 *
 */
ER
sys_sta_tsk (VP argp)
{
  struct
    {
      ID	tskid;
      INT	stacd;
    } *args = argp;

  return (sta_tsk (args->tskid, args->stacd));
}

/* sys_ext_tsk --- ����������λ����
 *
 */
ER
sys_ext_tsk (VP argp)
{
  ext_tsk ();
  return (E_OK);	/* �����ϡ��֤��ͤϤʤ���... */
}

/* sys_exd_tsk --- ����������λ���ơ��񸻤�������롣
 *
 */
ER
sys_exd_tsk (VP argp)
{
  exd_tsk ();
  return (E_OK);	/* �����ϡ��֤��ͤϤʤ���... */
}

/* sys_ter_tsk --- ���ꤷ����������λ����
 *
 * ������tskid	��λ���륿������ ID
 *
 */
ER
sys_ter_tsk (VP argp)
{
  struct
    {
      ID	tskid;
    } *args = argp;

  return (ter_tsk (args->tskid));
}


ER
sys_dis_dsp (VP argp)
{
  return (dis_dsp ());
}

ER
sys_ena_dsp (VP argp)
{
  return (ena_dsp ());
}

ER
sys_chg_pri (VP argp)
{
  struct
    {
      ID	tskid;
      PRI	tskpri;
    } *args = argp;

  return (chg_pri (args->tskid, args->tskpri));
}

ER
sys_rot_rdq (VP argp)
{
  struct
    {
      PRI	tskpri;
    } *args = argp;

  return (rot_rdq (args->tskpri));
}

ER
sys_rel_wai (VP argp)
{
  struct
    {
      ID	tskid;
    } *args = argp;

  return (rel_wai (args->tskid));
}

ER
sys_get_tid (VP argp)
{
  struct
    {
      ID	*p_tskid;
    } *args = argp;
  ID	rid;
  ER	err;

  err = get_tid (&rid);
  if (err == E_OK)
    err = vput_reg (run_task->tskid, args->p_tskid, sizeof(ID), &rid);
  return (err);
}

ER
sys_ref_tsk (VP argp)
{
  struct
    {
      T_RTSK	*pk_rtsk;
      ID	tskid;
    } *args = argp;

  return (ref_tsk (args->pk_rtsk, args->tskid));
}

ER
sys_sus_tsk (VP argp)
{
  struct 
    {
      ID taskid;
    } *args = argp;

  return (sus_tsk (args->taskid));
}


ER
sys_rsm_tsk (VP argp)
{
  struct 
    {
      ID taskid;
    } *args = argp;

  return (rsm_tsk (args->taskid));
}


ER
sys_frsm_tsk (VP argp)
{
  struct 
    {
      ID taskid;
    } *args = argp;

  return (frsm_tsk (args->taskid));
}


ER
sys_slp_tsk (VP argp)
{
  return (slp_tsk ());
}


#ifdef notdef
ER
sys_tslp_tsk (VP argp)
{
}
#endif /* notdef */


ER
sys_wup_tsk (VP argp)
{
  struct 
    {
      ID taskid;
    } *args = argp;

  return (wup_tsk (args->taskid));
}


ER
sys_can_wup (VP argp)
{
  struct
    {
      INT *p_wupcnt;
      ID taskid;
    } *args = argp;

  return (can_wup (args->p_wupcnt, args->taskid));
}


/* ----------------------------------------------------------------------- *
 * ���������̿������ƥॳ����                                              *
 * ----------------------------------------------------------------------- */
ER
sys_cre_sem (VP argp)
{
  struct
    {
      ID semid;
      T_CSEM *pkcsem;
    } *args = argp;

  return (cre_sem (args->semid, args->pkcsem));
}


ER
sys_del_sem (VP argp)
{
  struct
    {
      ID	semid;
    } *args = argp;

  return (del_sem (args->semid));
}


ER
sys_sig_sem (VP argp)
{
  struct 
    {
      ID	semid;
    } *args = argp;

  return (sig_sem (args->semid));
}


ER
sys_wai_sem (VP argp)
{
  struct 
    {
      ID	semid;
    } *args = argp;

  return (wai_sem (args->semid));
}


ER
sys_preq_sem (VP argp)
{
  struct 
    {
      ID	semid;
    } *args = argp;

  return (preq_sem (args->semid));
}


ER
sys_twai_sem (VP argp)
{
  struct 
    {
      ID	semid;
      TMO	tmout;
    } *args = argp;

  return (twai_sem (args->semid, args->tmout));
}


ER
sys_ref_sem (VP argp)
{
  struct
    {
      T_RSEM *pk_rsem;
      ID semid;
    } *args = argp;

  return (ref_sem (args->pk_rsem, args->semid));
}


ER
sys_cre_flg (VP argp)
{
  struct 
    {
      ID flgid;
      T_CFLG *pk_cflg;
    } *args = argp;

  return (cre_flg (args->flgid, args->pk_cflg));
}

ER	
sys_del_flg (VP argp)
{
  struct
    {
      ID flgid;
    } *args = argp;

  return (del_flg (args->flgid));
}

ER
sys_set_flg (VP argp)
{
  struct
    {
      ID flgid;
      UINT setptn;
    } *args = argp;

  return (set_flg (args->flgid, args->setptn));
}

ER
sys_clr_flg (VP argp)
{
  struct
    {
      ID flgid;
      UINT clrptn;
    } *args = argp;

  return (clr_flg (args->flgid, args->clrptn));
}

ER
sys_wai_flg (VP argp)
{
  struct
    {
      UINT *p_flgptn;
      ID flgid;
      UINT waiptn;
      UINT wfmode;
    } *args = argp;

  return (wai_flg (args->p_flgptn, args->flgid, args->waiptn, args->wfmode));
}

ER
sys_pol_flg (VP argp)
{
  struct 
    {
      UINT *p_flgptn;
      ID flgid;
      UINT waiptn;
      UINT wfmode;
    } *args = argp;

  return (pol_flg (args->p_flgptn, args->flgid, args->waiptn, args->wfmode));
}

ER
sys_twai_flg (VP argp)
{
  struct
    {
      UINT *p_flgptn;
      ID flgid;
      UINT waiptn;
      UINT wfmode;
      TMO tmout;
    } *args = argp;

  return (twai_flg (args->p_flgptn, args->flgid, args->waiptn, args->wfmode, args->tmout));
}

ER
sys_ref_flg (VP argp)
{
  struct
    {
      T_RFLG *pk_rflg;
      ID flgid;
    } *args = argp;

  return (ref_flg (args->pk_rflg, args->flgid));
}

ER
sys_cre_mbf (VP argp)
{
  struct
    {
      ID id;
      T_CMBF *pk_cmbf;
    } *args = argp;

  return (cre_mbf (args->id, args->pk_cmbf));
}

ER
sys_del_mbf (VP argp)
{
  struct 
    {
      ID id;
    } *args = argp;
  
  return (del_mbf (args->id));
}

ER
sys_snd_mbf (VP argp)
{
  struct
    {
      ID id;
      INT size;
      VP msg;
    } *args = argp;

  return (snd_mbf (args->id, args->size, args->msg));
}

ER
sys_psnd_mbf (VP argp)
{
  struct
    {
      ID id;
      INT size;
      VP msg;
    } *args = argp;

  return (psnd_mbf (args->id, args->size, args->msg));
}

ER
sys_tsnd_mbf (VP argp)
{
  struct
    {
      ID id;
      INT size;
      VP msg;
      TMO tmout;
    } *args = argp;

  return (tsnd_mbf (args->id, args->size, args->msg, args->tmout));
}

ER
sys_rcv_mbf (VP argp)
{
  struct
    {
      VP msg;
      INT *size;
      ID id;
    } *args = argp;

  return (rcv_mbf (args->msg, args->size, args->id));
}

ER
sys_prcv_mbf (VP argp)
{
  struct
    {
      VP msg;
      INT *size;
      ID id;
    } *args = argp;

  return (prcv_mbf (args->msg, args->size, args->id));
}

ER
sys_trcv_mbf (VP argp)
{
  struct
    {
      VP msg;
      INT *size;
      ID id;
      TMO tmout;
    } *args = argp;

  return (trcv_mbf (args->msg, args->size, args->id, args->tmout));
}


ER
sys_dis_int (VP argp)
{
  return (dis_int ());
}


ER
sys_ena_int (VP argp)
{
  return (ena_int ());
}


ER
sys_cre_mpl (VP argp)
{
  struct 
    {
      ID	mplid;
      T_CMPL	*pk_cmpl;
    } *args = argp;

  return (cre_mpl (args->mplid, args->pk_cmpl));
}

ER
sys_del_mpl (VP argp)
{
  struct
    {
      ID	mplid;
    } *args = argp;

  return (del_mpl (args->mplid));
}

ER
sys_get_blk (VP argp)
{
  struct
    {
      VP	pblk;
      ID	mplid;
      INT	blksz;
    } *args = argp;

  return (get_blk (args->pblk, args->mplid, args->blksz));
}

ER
sys_pget_blk (VP argp)
{
  struct
    {
      VP	pblk;
      ID	mplid;
      INT	blksz;
    } *args = argp;

  return (pget_blk (args->pblk, args->mplid, args->blksz));
}

ER
sys_rel_blk (VP argp)
{
  struct
    {
      ID	mplid;
      VP	blk;
    } *args = argp;

  return (rel_blk (args->mplid, args->blk));
}


ER
sys_ref_mpl (VP argp)
{
  struct
    {
      ID	mplid;
      T_RMPL	*pk_rmpl;
    } *args = argp;

  return (ref_mpl (args->mplid, args->pk_rmpl));
}


ER
sys_tget_blk (VP argp)
{
  struct
    {
      VP	p_blk;
      ID	mplid;
      INT	blksz;
      TMO	tmout;
    } *args = argp;

  return (tget_blk (args->p_blk, args->mplid, args->blksz, args->tmout));
}

ER
sys_get_ver (VP argp)
{
  struct
    {
      T_VER	*ver;
    } *args = argp;

  return (get_ver (args->ver));
}


/*
 * �����ߥϥ�ɥ��������롣
 *
 * ���δؿ��Ǥϡ����ΤȤ��� CPU ��٥� 0/�ȥ�å� �����γ����ߤȤ���
 * �ϥ�ɥ�����ꤷ�Ƥ��롣
 *
 */
ER
sys_def_int (VP argp)
{
  struct a
    {
      UINT	dintno;
      T_DINT	*pk_dint;
    } *args = (struct a *)argp;

  if ((args->dintno < 0) || (args->dintno > 128))
    {
      return (E_PAR);
    }
#ifdef notdef
  printk ("sys_def_int: dintno = %d(0x%x), inthdr = 0x%x, intatr = %d\n",
	  args->dintno,
	  args->dintno,
	  args->pk_dint->inthdr,
	  args->pk_dint->intatr);
#endif

  set_interrupt_entry (args->dintno,
		       args->pk_dint->inthdr,
		       args->pk_dint->intatr);
  return (E_OK);
}

/*
 * ���ۥ�������δؿ��� 
 */

/*
 *
 */
ER
sys_vcre_reg (VP argp)
{
  struct
    {
      ID	id;
      VP	start;
      W		min;
      W		max;
      UW	perm;
      FP	handle;
    } *args = argp;

  return vcre_reg (args->id, args->start, args->min, args->max, args->perm, args->handle);
}

/*
 *
 */
ER
sys_vdel_reg (VP argp)
{
  struct
    {
      ID	id;
      VP	start;
    } *args = argp;

  return vdel_reg (args->id, args->start);
}

/*
 *
 */
ER
sys_vmap_reg (VP argp)
{
  struct
    {
      ID	id;
      VP	start;
      W		size;
      W		accmode;
    } *args = argp;

  return vmap_reg (args->id, args->start, args->size, args->accmode);
}

/*
 *
 */
ER
sys_vunm_reg (VP argp)
{
  struct
    {
      ID	id;
      VP	addr;
      W		size;
    } *args = argp;

  return vunm_reg (args->id, args->addr, args->size);
}

/*
 *
 */
ER
sys_vdup_reg (VP argp)
{
  struct
    {
      ID	src;
      ID	dst;
      VP	start;
    } *args = argp;

  return vdup_reg (args->src, args->dst, args->start);
}

/*
 *
 */
ER
sys_vprt_reg (VP argp)
{
  struct
    {
      ID	id;
      VP	start;
      UW	prot;
    } *args = argp;

  return vprt_reg (args->id, args->start, args->prot);
}

/*
 *
 */
ER
sys_vshr_reg (VP argp)
{
  struct
    {
      ID	src;
      ID	dst;
      VP	start;
    } *args = argp;

  return vshr_reg (args->src, args->dst, args->start);
}

/*
 *
 */
ER
sys_vput_reg (VP argp)
{
  struct 
    {
      ID	id;
      VP	start;
      W		size;
      VP	buf;
    } *args = argp;

  return vput_reg (args->id, args->start, args->size, args->buf);
}

/*
 *
 */
ER
sys_vget_reg (VP argp)
{
  struct
    {
      ID	id;
      VP	start;
      W		size;
      VP	buf;
    } *args = argp;
  

  return vget_reg (args->id, args->start, args->size, args->buf);
}

/*
 *
 */
ER
sys_vsts_reg (VP argp)
{
  struct
    {
      ID	id;
      VP	start;
      VP	stat;
    } *args = argp;

  return vsts_reg (args->id, args->start, args->stat);
}



/*
 *
 */
ER
sys_vget_phs (VP argp)
{
  struct
    {
      ID	id;
      VP	addr;
      UW	*paddr;
    } *args = argp;
  UW	physical_addr;

  physical_addr = vtor (args->id, (UW) args->addr);
  if (physical_addr == NULL)
    {
      return (E_OBJ);
    }
  *(args->paddr) = physical_addr;
  return (E_OK);
}


/*
 * ���ִ����δؿ��� 
 */

ER
sys_dly_tsk (VP argp)
{
  struct 
    {
      DLYTIME dlytim;
    } *args = argp;

  return (dly_tsk (args->dlytim));
}

ER
sys_def_cyc (VP argp)
{
  struct 
    {
      HNO cycno;
      T_DCYC *pk_dcyc;
    } *args = argp;

  return (def_cyc (args->cycno, args->pk_dcyc));
}

ER
sys_act_cyc (VP argp)
{
  struct 
    {
      HNO cycno;
      UINT cycact;
    } *args = argp;

  return (act_cyc (args->cycno, args->cycact));
}

ER
sys_ref_cyc (VP argp)
{
  struct 
    {
      T_RCYC *pk_rcyc;
      HNO cycno;
    } *args = argp;

  return (ref_cyc (args->pk_rcyc, args->cycno));
}

ER
sys_def_alm (VP argp)
{
  struct 
    {
      HNO almo;
      T_DALM *pk_dalm;
    } *args = argp;

  return (def_alm (args->almo, args->pk_dalm));
}

ER
sys_ref_alm (VP argp)
{
  struct 
    {
      T_RALM *pk_ralm;
      HNO almo;
    } *args = argp;

  return (ref_alm (args->pk_ralm, args->almo));
}


/*
 * ����¾�δؿ��� 
 */
/*
 *
 */
ER
sys_vget_csl (VP argp)
{
  struct
    {
      W	*x;
      W *y;
    } *args = argp;

  return (get_cursor_position (args->x, args->y));
}

/* sys_vset_cns
 *
 */
ER
sys_vset_cns (VP argp)
{
  extern ID	console_driver;
  extern ID	console_recv;
  struct
    {
      ID driver_id;
    } *args = argp;


  printk ("sys_vset_cns: driver id = %d\n", args->driver_id);
  if (args->driver_id > 0)
    {
      console_recv = get_port (sizeof (DDEV_RES), sizeof (DDEV_RES));
      if (console_recv <= 0)
	{
	  printk ("Init: Cannot allocate port\n");
	  return (E_OBJ);
	}

      printk ("console_recv = %d\n", console_recv);
      console_driver = args->driver_id;
      return (E_OK);
    }
  return (E_ID);
}

/* sys_vsys_msc - miscellaneous function
 *
 */
ER
sys_vsys_msc (VP argp)
{
  struct
    {
      W cmd;
      VP arg;
    } *args = argp;

  printk ("vsys_msc: cmd = %d\n", args->cmd);

  switch (args->cmd)
    {
    default:
      return (E_ID);
      
    case 1:
      /* fall down */
      falldown (args->arg);
      dis_int ();
      for (;;)
	;
      /* NOT REACHED */

    case 2:
      /* reset */
      for (;;)
	{
	  while ((inb (0x64) & 0x02) == 0x02)
	    ;
	  outb (0x64, 0xf0);
	}
      /* NOT REACHED */

    case 3:
      /* load lowlib */
      return (load_lowlib (args->arg));
      break;

    case 4:
      /* load lowlib */
      return (unload_lowlib (args->arg));
      break;

    case 5:
      /* status lowlib */
      return (stat_lowlib (args->arg));
      break;

    case 6:
      /* print_task_list */
      print_task_list();
      break;

    case 7:
      /* pmemstat */
      pmemstat();
      break;

    case 9998:
      pmemdump(args->arg);
      break;
    case 9999:
      /* debugger(); */
      break;
    }

  return (E_OK);
}

/* sys_vcpy_stk - copy task stack 
 *
 */
ER 
sys_vcpy_stk (VP argp)
{
  struct {
    ID src;
    W  esp;
    W  ebp;
    ID dst;
  } *args = argp;

  return vcpy_stk(args->src, args->esp, args->ebp, args->dst);
}

/* sys_vset_cxt - set task context
 *
 */
ER 
sys_vset_ctx (VP argp)
{
  struct {
    ID tid;
    W  eip;
  } *args = argp;

  return vset_ctx(args->tid, args->eip);
}
