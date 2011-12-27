/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* func.h - 大域関数の定義
 *
 */

#ifndef __ITRON_FUNC_H__
#define __ITRON_FUNC_H__	1


#include "types.h"
#include "itron.h"
#include "task.h"

/* main.c */
extern ER	main (void);

/* printf.c */
extern W	printf (B *, ...);
extern W	vprintf (B *, VP);

/* memory.c */
extern void	init_memory (void);
extern W	alloc_memory (W size);
extern ER	free_memory (W index, W size);

/* pmemory.c */
extern void	pmem_init (void);
extern VP	palloc (W size);
extern ER	pfree (VP p, W size);

/* console.c */
extern void	simple_init_console (void);
extern BOOL	init_crt (W);		/* 初期化			*/
extern W	intr_crt ();		/* 割り込みハンドラ		*/

extern void	putchar (TCODE ch);	/* １文字出力			*/
extern void	write_cr (void);
extern void	write_tab (void);
extern void	console_clear (void);

/* timer.c */
extern void	start_interval (void);
extern void	intr_interval (void);
extern void	init_timer (void);
extern void	set_timer (W time, FP func, void *argp);
extern ER	unset_timer (FP func, VP arg);

/* fault.c */
extern W	init_interrupt (void);
extern void	reset_intr_mask (W intn);
extern void	interupt (W intn);
extern void	trap (W intn);
extern void	page_fault (W errcode, W eip, W cs, W eflags, W esp, W ss);
extern void	fault (W intn, void *argptr);
extern W	wait_int (W *flag);
extern void	set_idt (UW entry, UW selector, UW offset, UW type, UW dpl);
extern void	protect_fault (UW errno, UW eip, UW tmp);
extern ER  	set_interrupt_entry (W intno, FP func, ATR attr);

/* misc.c */
extern UH	halfword_swap (UH w);
extern VP	bcopy (VP src, VP dest, W count);
extern void	bzero (VP src, W count);
extern INT	strlen (B *buf);
extern W	strncmp (B *s1, B *s2, W size);

/* virtual_memory.c */
extern ADDR_MAP	dup_vmem_table (ADDR_MAP dest);
extern ER	relase_vmap (ADDR_MAP dest);

/* task.c */
extern void	init_task1 (void);
extern void	init_task_state_segment (W index, T_I386_CONTEXT *tss);
extern T_TCB	*add_tcb_list (T_TCB *list, T_TCB *new);
extern T_TCB	*ins_tcb_list (T_TCB *list, T_TCB *new);
extern T_TCB	*del_tcb_list (T_TCB *list, T_TCB *del);
extern ER	task_switch (BOOL save_nowtask);
extern void	print_task_list (void);
extern T_TCB	*get_tskp (ID tskid);

/* tss.c */
extern void	print_task_desc (TASK_DESC *desc);


/* kalloc.c */
extern void	init_kalloc (void);
extern VP	kalloc (W size);
extern void	kfree (VP area, W size);

/* version.c */
extern ER	get_ver (T_VER *ver);

/* システムコール関数の定義 
 */
/* --------------------------------------------------------- */
/* タスク管理機能                                            */
/* --------------------------------------------------------- */

extern ER	cre_tsk (ID tskid, T_CTSK *pk_ctsk);
extern ER	del_tsk (ID tskid);
extern ER	sta_tsk (ID tskid, INT stacd);
extern void	ext_tsk (void);
extern void	exd_tsk (void);
extern ER	ter_tsk (ID tskid);
extern ER	dis_dsp ();
extern ER	ena_dsp ();
extern ER	chg_pri (ID tskid, PRI tskpri);
extern ER	rot_rdq (PRI tskpri);
extern ER	rel_wai (ID tskid);
extern ER	get_tid (ID *p_tskid);
extern ER	ref_tsk (T_RTSK *pk_rtsk, ID tskid);
extern ER	slp_tsk ();
extern ER	tslp_tsk (TMO timeout);
extern ER	wup_tsk (ID task);
extern ER	can_wup (INT *p_wupcnt, ID taskid);
extern ER	ref_tsk (T_RTSK *stat, ID taskid);
extern ER	dly_tsk (DLYTIME dlytime);
extern ER	def_cyc (HNO cycno, T_DCYC *pk_dcyc);

extern ER	sus_tsk ();
extern ER	rsm_tsk ();
extern ER	frsm_tsk ();
extern ER	slp_tsk ();
extern ER	tslp_tsk ();
extern ER	wup_tsk ();
extern ER	can_wup ();


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
/* テスト用タスク                                            */
/* --------------------------------------------------------- */
extern void	tsk2_func (void);
extern void	tsk3_func (void);

/* --------------------------------------------------------- */
/* システム管理用タスク                                      */
/* --------------------------------------------------------- */
extern void	sys_init_task (void);

/* --------------------------------------------------------- */
/* デバッグ用タスク                                          */
/* --------------------------------------------------------- */
extern void	debugger (void);

/* --------------------------------------------------------- */
/* RS232C デバイス管理タスク				     */
/* --------------------------------------------------------- */
extern void	rs232c_task (void);
extern void	keyboard_task (void);

#endif /* __ITRON_FUNC_H__ */
