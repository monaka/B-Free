/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/h/syscall.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 */

#ifndef __H_SYSCALL_H__
#define __H_SYSCALL_H__	1


/* システムコール番号の定義 
 */
#define SYS_CRE_TSK	(1)
#define	SYS_DEL_TSK	(2)
#define	SYS_STA_TSK	(3)
#define	SYS_EXT_TSK	(4)
#define	SYS_EXD_TSK	(5)
#define	SYS_TER_TSK	(6)
#define	SYS_DIS_DSP	(7)
#define	SYS_ENA_DSP	(8)
#define	SYS_CHG_PRI	(9)
#define	SYS_ROT_RDQ	(10)
#define	SYS_REL_WAI	(11)
#define	SYS_GET_TID	(12)
#define	SYS_REF_TSK	(13)
#define	SYS_SUS_TSK	(14)
#define	SYS_RSM_TSK	(15)
#define	SYS_FRSM_TSK	(16)
#define	SYS_SLP_TSK	(17)
#define	SYS_TSLP_TSK	(18)
#define	SYS_WUP_TSK	(19)
#define SYS_CAN_WUP	(20)
#define SYS_CRE_SEM	(21)
#define SYS_DEL_SEM	(22)
#define SYS_SIG_SEM	(23)
#define SYS_WAI_SEM	(24)
#define SYS_PREQ_SEM	(25)
#define SYS_TWAI_SEM	(26)
#define SYS_REF_SEM	(27)
#define SYS_CRE_FLG   	(28)
#define SYS_DEL_FLG   	(29)
#define SYS_SET_FLG   	(30)
#define SYS_CLR_FLG  	(31)
#define SYS_WAI_FLG   	(32)
#define SYS_POL_FLG   	(33)
#define SYS_TWAI_FLG   	(34)
#define SYS_REF_FLG   	(35)
#define SYS_CRE_MBF	(36)
#define SYS_DEL_MBF	(37)
#define SYS_SND_MBF	(38)
#define SYS_PSND_MBF	(39)
#define SYS_TSND_MBF	(40)
#define SYS_RCV_MBF	(41)
#define SYS_PRCV_MBF	(42)
#define SYS_TRCV_MBF	(43)
#define SYS_DIS_INT   	(44)
#define SYS_ENA_INT   	(45)
#define SYS_CRE_MPL   	(46)
#define SYS_DEL_MPL   	(47)
#define SYS_GET_BLK   	(48)
#define SYS_PGET_BLK   	(49)
#define SYS_REL_BLK   	(50)
#define SYS_REF_BLK   	(51)
#define SYS_TGET_BLK   	(52)
#define SYS_GET_VER   	(53)

#define SYS_DLY_TSK	(60)
#define SYS_DEF_CYC	(61)
#define SYS_ACT_CYC	(62)
#define SYS_REF_CYC	(63)
#define SYS_DEF_ALM	(64)
#define SYS_REF_ALM	(65)

#define SYS_DEF_INT	(67)

#define SYS_VSYS_INF	(99)
#define SYS_DBG_PUTS	(100)

#define SYS_VCRE_REG	(101)
#define SYS_VDEL_REG	(102)
#define SYS_VMAP_REG	(103)
#define SYS_VUNM_REG	(104)
#define SYS_VDUP_REG	(105)
#define SYS_VPRT_REG	(106)
#define SYS_VSHR_REG	(107)
#define SYS_VPUT_REG	(108)
#define SYS_VGET_REG	(109)
#define SYS_VSTS_REG	(110)

#define SYS_VGET_PHS	(111)

#define SYS_VGET_CSL	(120)
#define SYS_VSET_CNS	(121)
#define SYS_VSYS_MSC	(122)
#define SYS_VCPY_STK	(123)
#define SYS_VSET_CTX	(124)

/* タスク管理システムコール
 */
extern ER	sys_cre_tsk (void *argp);		/*   1 */
extern ER	sys_del_tsk (void *argp);		/*   2 */
extern ER	sys_sta_tsk (void *argp);		/*   3 */
extern ER	sys_ext_tsk (void *argp);		/*   4 */
extern ER	sys_exd_tsk (void *argp);		/*   6 */
extern ER	sys_ter_tsk (void *argp);		/*   6 */
extern ER	sys_dis_dsp (void *argp);		/*   7 */
extern ER	sys_ena_dsp (void *argp);		/*   8 */
extern ER	sys_chg_pri (void *argp);		/*   9 */
extern ER	sys_rot_rdq (void *argp);		/*  10 */
extern ER	sys_rel_wai (void *argp);		/*  11 */
extern ER	sys_get_tid (void *argp);		/*  12 */
extern ER	sys_ref_tsk (void *argp);		/*  13 */
extern ER	sys_sus_tsk (void *argp);		/*  14 */
extern ER	sys_rsm_tsk (void *argp);		/*  15 */
extern ER	sys_frsm_tsk (void *argp);	/*  16 */
extern ER	sys_slp_tsk (void *argp);		/*  17 */
#ifdef notdef
extern ER	sys_tslp_tsk (void *argp);	/*  18 */
#endif /* notdef */
extern ER	sys_wup_tsk (void *argp);		/*  19 */
extern ER	sys_can_wup (void *argp);		/*  20 */
extern ER	sys_cre_sem (void *argp);		/*  21 */
extern ER	sys_del_sem (void *argp);		/*  22 */
extern ER       sys_sig_sem (void *argp);		/*  23 */
extern ER	sys_wai_sem (void *argp);		/*  24 */
extern ER	sys_preq_sem (void *argp);	/*  25 */
extern ER	sys_twai_sem (void *argp);	/*  26 */
extern ER	sys_ref_sem (void *argp);		/*  27 */
extern ER	sys_cre_flg (void *argp);		/*  28 */
extern ER	sys_del_flg (void *argp);		/*  29 */
extern ER	sys_set_flg (void *argp);		/*  30 */
extern ER	sys_clr_flg (void *argp);		/*  31 */
extern ER	sys_wai_flg (void *argp);		/*  32 */
extern ER	sys_pol_flg (void *argp);		/*  33 */
extern ER	sys_twai_flg (void *argp);	/*  34 */
extern ER	sys_ref_flg (void *argp);		/*  35 */
extern ER	sys_cre_mbf (void *argp);		/*  36 */
extern ER	sys_del_mbf (void *argp);		/*  37 */
extern ER	sys_snd_mbf (void *argp);		/*  38 */
extern ER	sys_psnd_mbf (void *argp);	/*  39 */
extern ER	sys_tsnd_mbf (void *argp);	/*  40 */
extern ER	sys_rcv_mbf (void *argp);		/*  41 */
extern ER	sys_prcv_mbf (void *argp);	/*  42 */
extern ER	sys_trcv_mbf (void *argp);	/*  43 */
extern ER	sys_dis_int (void *argp);		/*  44 */
extern ER	sys_ena_int (void *argp);		/*  45 */
extern ER	sys_cre_mpl (void *argp);		/*  46 */
extern ER	sys_del_mpl (void *argp);		/*  47 */
extern ER	sys_get_blk (void *argp);		/*  48 */
extern ER	sys_pget_blk (void *argp);	/*  49 */
extern ER	sys_rel_blk (void *argp);		/*  50 */
extern ER	sys_ref_mpl (void *argp);		/*  51 */
extern ER	sys_tget_blk (void *argp);	/*  52 */
extern ER	sys_get_ver (void *argp);		/*  53 */

extern ER	sys_def_int (void *argp);		/*  67 */

/* common/dbg_functions.c */
extern ER	sys_vsys_inf (void *argp);	/* 99 */
extern ER	sys_dbg_puts (void *args);	/* 100 */

/* 仮想メモリ管理用システムコール */
extern ER	sys_vcre_reg (void *argp);
extern ER	sys_vdel_reg (void *argp);
extern ER	sys_vmap_reg (void *argp);
extern ER	sys_vunm_reg (void *argp);
extern ER	sys_vdup_reg (void *argp);
extern ER	sys_vprt_reg (void *argp);
extern ER	sys_vshr_reg (void *argp);
extern ER	sys_vput_reg (void *argp);
extern ER	sys_vget_reg (void *argp);
extern ER	sys_vsts_reg (void *argp);
extern ER	sys_vget_phs (void *argp);

/* 時間管理用システムコール */
extern ER	sys_dly_tsk (void *argp);
extern ER	sys_def_cyc (void *argp);
extern ER	sys_act_cyc (void *argp);
extern ER	sys_ref_cyc (void *argp);
extern ER	sys_def_alm (void *argp);
extern ER	sys_ref_alm (void *argp);

/* その他のシステムコール */
extern ER	sys_vget_csl (void *argp);
extern ER	sys_vset_cns (void *argp);
extern ER	sys_vsys_msc (void *argp);
extern ER	sys_vcpy_stk (void *argp);
extern ER	sys_vset_ctx (void *argp);

#endif /* __H_SYSCALL_H__ */
