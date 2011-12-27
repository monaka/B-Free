
#ifndef __ITRON_SYSCALL_FUNC_H__
#define __ITRON_SYSCALL_FUNC_H__

/* システムコール関数の定義 */
/* --------------------------------------------------------- */
/* タスク管理機能                                            */
/* --------------------------------------------------------- */

extern ER	cre_tsk (ID tskid, T_CTSK *pk_ctsk);
extern ER	del_tsk (ID tskid);
extern ER	sta_tsk (ID tskid, INT stacd);
extern void	ext_tsk (void);
extern void	exd_tsk (void);
extern ER	ter_tsk (ID tskid);
extern ER	dis_dsp (void);
extern ER	ena_dsp (void);
extern ER	chg_pri (ID tskid, PRI tskpri);
extern ER	rot_rdq (PRI tskpri);
extern ER	rel_wai (ID tskid);
extern ER	get_tid (ID *p_tskid);
extern ER	ref_tsk (T_RTSK *pk_rtsk, ID tskid);
extern ER	slp_tsk (void);
extern ER	tslp_tsk (TMO timeout);
extern ER	wup_tsk (ID taskid);
extern ER	can_wup (INT *p_wupcnt, ID taskid);
extern ER	ref_tsk (T_RTSK *stat, ID taskid);
extern ER	dly_tsk (DLYTIME dlytime);
extern ER	def_cyc (HNO cycno, T_DCYC *pk_dcyc);

extern ER	sus_tsk (ID taskid);
extern ER	rsm_tsk (ID taskid);
extern ER	frsm_tsk (ID taskid);

/* B-Free Extention */
extern ER new_task (T_CTSK *pk_ctsk, ID *rid, BOOL run_flag);
extern ER vcre_tsk (T_CTSK *pk_ctsk, ID *rid);

/* --------------------------------------------------------- */
/* セマフォ用関数                                            */
/* --------------------------------------------------------- */
extern ER	cre_sem (ID semid, T_CSEM *pk_csem);
extern ER	del_sem (ID semid);
extern ER       sig_sem (ID semid);
extern ER	wai_sem (ID semid);
extern ER	preq_sem (ID semid);
extern ER	twai_sem (ID semid, TMO tmout);
extern ER	ref_sem (T_RSEM *pk_rsem, ID semid);


/* --------------------------------------------------------- */
/* イベントフラグ用関数                                      */
/* --------------------------------------------------------- */
extern ER	cre_flg (ID flgid, T_CFLG *pk_cflg);
extern ER	del_flg (ID flgid);
extern ER	set_flg (ID flgid, UINT setptn);
extern ER	clr_flg (ID flgid, UINT clrptn);
extern ER	wai_flg (UINT *p_flgptn, ID flgid, UINT waiptn, UINT wfmode);
extern ER	pol_flg (UINT *p_flgptn, ID flgid, UINT waiptn, UINT wfmode);
extern ER	twai_flg (UINT *p_flgptn, ID flgid, UINT waiptn, UINT wfmode, TMO tmout);
extern ER	ref_flg (T_RFLG *pk_rflg, ID flgid);

/* --------------------------------------------------------- */
/* メッセージバッファ用関数                                  */
/* --------------------------------------------------------- */
extern ER	init_msgbuf (void);
extern ER	cre_mbf (ID id, T_CMBF *pk_cmbf);
extern ER	del_mbf (ID id);
extern ER	snd_mbf (ID id, INT size, VP msg);
extern ER	psnd_mbf (ID id, INT size, VP msg);
extern ER	tsnd_mbf (ID id, INT size, VP msg, TMO tmout);
extern ER	rcv_mbf (VP msg, INT *size, ID id);
extern ER	prcv_mbf (VP msg, INT *size, ID id);
extern ER	trcv_mbf (VP msg, INT *size, ID id, TMO tmout);

/* --------------------------------------------------------- */
/* Interrupt                                                 */
/* --------------------------------------------------------- */
extern void ret_int(void);
extern void ret_wup(ID tskid);
extern ER loc_cpu(void);
extern ER unl_cpu(void);
#if 0
extern ER dis_int(/*UINT eintno*/);
extern ER ena_int(/*UINT eintno*/);
#else
extern ER dis_int(void);
extern ER ena_int(void);
#endif

/* --------------------------------------------------------- */
/* Memory                                                    */
/* --------------------------------------------------------- */
extern ER cre_mpl (ID mplid, T_CMPL *pk_cmpl);
extern ER del_mpl (ID mplid);
extern ER get_blk (VP *pblk, ID mplid, INT blksz);
extern ER pget_blk (VP *pblk, ID mplid, INT blksz);
extern ER tget_blk (VP *pblk, ID mplid, INT blksz, TMO tmout);
extern ER rel_blk (ID mplid, VP blk);
extern ER ref_mpl (ID mplid, T_RMPL *pk_rmpl);

extern ER vcre_reg (ID	id, VP start, W min, W max, UW perm, FP handle);
extern ER vdel_reg (ID id, VP start);
extern ER vmap_reg (ID id, VP start, UW size, W accmode);
extern ER vunm_reg (ID id, VP addr, UW size);
extern ER vdup_reg (ID src, ID dst, VP start);
extern ER vprt_reg (ID id, VP start, UW prot);
extern ER vshr_reg (ID src, ID dst, VP start);
extern ER vget_reg (ID id, VP start, UW size, VP buf);
extern ER vput_reg (ID id, VP start, UW size, VP buf);
extern ER vsts_reg (ID id, VP start, VP stat);
extern ER vget_phs (ID id, VP addr, UW *paddr);

/* --------------------------------------------------------- */
/* 時間管理                                                    */
/* --------------------------------------------------------- */

extern ER dly_tsk(DLYTIME dlytim);
extern ER def_cyc(HNO cycno, T_DCYC *pk_dcyc);
extern ER act_cyc(HNO cycno, UINT cycact);
extern ER ref_cyc(T_RCYC *pk_rcyc, HNO cycno);
extern ER def_alm(HNO almo, T_DALM *pk_dalm);
extern ER ref_alm(T_RALM *pk_ralm, HNO almno);

/* --------------------------------------------------------- */
/* MISC                                                      */
/* --------------------------------------------------------- */
extern ER lod_low (ID task, B *name);

extern ER vget_csl (W *x, W *y);
extern ER vset_cns (ID driver);
extern ER vsys_msc (W cmd, VP argp);
extern ER vsys_inf (W func, W sub, VP argp);
extern ER vcpy_stk (ID src, W esp, W ebp, ID dst);
extern ER vset_ctx (ID tid, W eip);

#endif /*__ITRON_SYSCALL_FUNC_H__*/
