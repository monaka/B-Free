/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/itron-3.0/common/syscall-if.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/itron-3.0/common/syscall-if.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

/* syscall-if.c --- �����ƥॳ����Υ��󥿥ե������ؿ�
 *
 * $Revision: 1.1 $
 * $Log: syscall-if.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.11  1995-12-13 16:02:03  night
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
sys_cre_tsk (void *argp)
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
sys_del_tsk (void *argp)
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
sys_sta_tsk (void *argp)
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
sys_ext_tsk (void)
{
  ext_tsk ();
  return (E_OK);	/* �����ϡ��֤��ͤϤʤ���... */
}

/* sys_exd_tsk --- ����������λ���ơ��񸻤�������롣
 *
 */
ER
sys_exd_tsk (void)
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
sys_ter_tsk (void *argp)
{
  struct
    {
      ID	tskid;
    } *args = argp;

  return (ter_tsk (args->tskid));
}


ER
sys_dis_dsp (void *argp)
{
  return (dis_dsp ());
}

ER
sys_ena_dsp (void *argp)
{
  return (ena_dsp ());
}

ER
sys_chg_pri (void *argp)
{
  struct
    {
      ID	tskid;
      PRI	tskpri;
    } *args = argp;

  return (chg_pri (args->tskid, args->tskpri));
}

ER
sys_rot_rdq (void *argp)
{
  struct
    {
      PRI	tskpri;
    } *args = argp;

  return (rot_rdq (args->tskpri));
}

ER
sys_rel_wai (void *argp)
{
  struct
    {
      ID	tskid;
    } *args = argp;

  return (rel_wai (args->tskid));
}

ER
sys_get_tid (void *argp)
{
  struct
    {
      ID	*p_tskid;
    } *args = argp;
  ID	rid;
  ER	err;

  err = get_tid (args->p_tskid);
  return (err);
}

ER
sys_ref_tsk (void *argp)
{
  struct
    {
      T_RTSK	*pk_rtsk;
      ID	tskid;
    } *args = argp;

  return (ref_tsk (args->pk_rtsk, args->tskid));
}

ER
sys_sus_tsk (void *argp)
{
  struct 
    {
      ID taskid;
    } *args = argp;

  return (sus_tsk (args->taskid));
}


ER
sys_rsm_tsk (void *argp)
{
  struct 
    {
      ID taskid;
    } *args = argp;

  return (rsm_tsk (args->taskid));
}


ER
sys_frsm_tsk (void *argp)
{
  struct 
    {
      ID taskid;
    } *args = argp;

  return (frsm_tsk (args->taskid));
}


ER
sys_slp_tsk (void)
{
  return (slp_tsk ());
}


#ifdef notdef
ER
sys_tslp_tsk (void *argp)
{
}
#endif /* notdef */


ER
sys_wup_tsk (void *argp)
{
  struct 
    {
      ID taskid;
    } *args = argp;

  return (wup_tsk (args->taskid));
}


ER
sys_can_wup (void *argp)
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
sys_cre_sem (void *argp)
{
  struct
    {
      ID semid;
      T_CSEM *pkcsem;
    } *args = argp;

  return (cre_sem (args->semid, args->pkcsem));
}


ER
sys_del_sem (void *argp)
{
  struct
    {
      ID	semid;
    } *args = argp;

  return (del_sem (args->semid));
}


ER
sys_sig_sem (void *argp)
{
  struct 
    {
      ID	semid;
    } *args = argp;

  return (sig_sem (args->semid));
}


ER
sys_wai_sem (void *argp)
{
  struct 
    {
      ID	semid;
    } *args = argp;

  return (wai_sem (args->semid));
}


ER
sys_preq_sem (void *argp)
{
  struct 
    {
      ID	semid;
    } *args = argp;

  return (preq_sem (args->semid));
}


ER
sys_twai_sem (void *argp)
{
  struct 
    {
      ID	semid;
      TMO	tmout;
    } *args = argp;

  return (twai_sem (args->semid, args->tmout));
}


ER
sys_ref_sem (void *argp)
{
  struct
    {
      T_RSEM *pk_rsem;
      ID semid;
    } *args = argp;

  return (ref_sem (args->pk_rsem, args->semid));
}


ER
sys_cre_flg (void *argp)
{
  struct 
    {
      ID flgid;
      T_CFLG *pk_cflg;
    } *args = argp;

  return (cre_flg (args->flgid, args->pk_cflg));
}

ER	
sys_del_flg (void *argp)
{
  struct
    {
      ID flgid;
    } *args = argp;

  return (del_flg (args->flgid));
}

ER
sys_set_flg (void *argp)
{
  struct
    {
      ID flgid;
      UINT setptn;
    } *args = argp;

  return (set_flg (args->flgid, args->setptn));
}

ER
sys_clr_flg (void *argp)
{
  struct
    {
      ID flgid;
      UINT clrptn;
    } *args = argp;

  return (clr_flg (args->flgid, args->clrptn));
}

ER
sys_wai_flg (void *argp)
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
sys_pol_flg (void *argp)
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
sys_twai_flg (void *argp)
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
sys_ref_flg (void *argp)
{
  struct
    {
      T_RFLG *pk_rflg;
      ID flgid;
    } *args = argp;

  return (ref_flg (args->pk_rflg, args->flgid));
}

ER
sys_cre_mbf (void *argp)
{
  struct
    {
      ID id;
      T_CMBF *pk_cmbf;
    } *args = argp;

  return (cre_mbf (args->id, args->pk_cmbf));
}

ER
sys_del_mbf (void *argp)
{
  struct 
    {
      ID id;
    } *args = argp;
  
  return (del_mbf (args->id));
}

ER
sys_snd_mbf (void *argp)
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
sys_psnd_mbf (void *argp)
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
sys_tsnd_mbf (void *argp)
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
sys_rcv_mbf (void *argp)
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
sys_prcv_mbf (void *argp)
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
sys_trcv_mbf (void *argp)
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
sys_dis_int (void *argp)
{
  return (dis_int ());
}


ER
sys_ena_int (void *argp)
{
  return (ena_int ());
}


ER
sys_cre_mpl (void *argp)
{
  struct 
    {
      ID	mplid;
      T_CMPL	*pk_cmpl;
    } *args = argp;

  return (cre_mpl (args->mplid, args->pk_cmpl));
}

ER
sys_del_mpl (void *argp)
{
  struct
    {
      ID	mplid;
    } *args = argp;

  return (del_mpl (args->mplid));
}

ER
sys_get_blk (void *argp)
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
sys_pget_blk (void *argp)
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
sys_rel_blk (void *argp)
{
  struct
    {
      ID	mplid;
      VP	blk;
    } *args = argp;

  return (rel_blk (args->mplid, args->blk));
}


ER
sys_ref_mpl (void *argp)
{
  struct
    {
      ID	mplid;
      T_RMPL	*pk_rmpl;
    } *args = argp;

  return (ref_mpl (args->mplid, args->pk_rmpl));
}


ER
sys_tget_blk (void *argp)
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
sys_get_ver (void *argp)
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
sys_def_int (void *argp)
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
  printf ("sys_def_int: dintno = %d(0x%x), inthdr = 0x%x, intatr = %d\n",
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
sys_vcre_reg (void *argp)
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
sys_vdel_reg (void *argp)
{
  struct
    {
      ID	id;
      VP	start;
    } *args = argp;

  return (E_NOSPT);
}

/*
 *
 */
ER
sys_vmap_reg (void *argp)
{
  struct
    {
      ID	id;
      VP	start;
      W		size;
    } *args = argp;

  return vmap_reg (args->id, args->start, args->size);
}

/*
 *
 */
ER
sys_vunm_reg (void *argp)
{
  struct
    {
      ID	id;
      VP	addr;
      W		size;
    } *args = argp;

  return (E_NOSPT);
}

/*
 *
 */
ER
sys_vdup_reg (void *argp)
{
  struct
    {
      ID	src;
      ID	dst;
      VP	start;
    } *args = argp;

  return (E_NOSPT);
}

/*
 *
 */
ER
sys_vprt_reg (void *argp)
{
  struct
    {
      ID	id;
      VP	start;
      UW	prot;
    } *args = argp;

  return (E_NOSPT);
}

/*
 *
 */
ER
sys_vshr_reg (void *argp)
{
  struct
    {
      ID	src;
      ID	dst;
      VP	start;
    } *args = argp;

  return (E_NOSPT);
}

/*
 *
 */
ER
sys_vput_reg (void *argp)
{
  struct 
    {
      ID	id;
      VP	start;
      W		size;
      VP	buf;
    } *args = argp;

  return (E_NOSPT);
}

/*
 *
 */
ER
sys_vget_reg (void *argp)
{
  struct
    {
      ID	id;
      VP	start;
      W		size;
      VP	buf;
    } *args = argp;

  return (E_NOSPT);
}

/*
 *
 */
ER
sys_vsts_reg (void *argp)
{
  struct
    {
      ID	id;
      VP	start;
      UW	stat;
    } *args = argp;

  return (E_NOSPT);
}


