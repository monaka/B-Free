/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* syscall.c
 *
 * システムコール処理を行う。
 *
 * $Revision: 1.1 $
 *
 */

static char rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/common/syscall.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

#include "itron.h"
#include "errno.h"
#include "syscall.h"
#include "func.h"

#define DEF_SYSCALL(x,n)	{ #x, sys_ ## x, n }
#define DEF_NOSYSCALL		{ "nosys", nodef, 0 }

extern ER	nosyscall (VP argp);
static ER	nodef (VP argp);

struct t_syscall
{
  B	*name;		/* システムコール名	*/
  ER	(*func)(VP argp);	/* 関数へのポインタ	*/
  W	argn;		/* 引数の数		*/
};

/* システムコールテーブル
 */  
struct t_syscall	syscall_table[] =
{
  DEF_NOSYSCALL,		/*    0 */

  /* タスク管理システムコール */
  DEF_SYSCALL (cre_tsk, 2),	/*    1 */
  DEF_SYSCALL (del_tsk, 1),	/*    2 */
  DEF_SYSCALL (sta_tsk, 2),	/*    3 */
  DEF_SYSCALL (ext_tsk, 0),     /*    4 */
  DEF_SYSCALL (exd_tsk, 0),     /*    5 */
  DEF_SYSCALL (ter_tsk, 1),     /*    6 */
  DEF_SYSCALL (dis_dsp, 0),	/*    7 */
  DEF_SYSCALL (ena_dsp, 0),	/*    8 */
  DEF_SYSCALL (chg_pri, 2),	/*    9 */
  DEF_SYSCALL (rot_rdq, 1),	/*   10 */
  DEF_SYSCALL (rel_wai, 1),	/*   11 */
  DEF_SYSCALL (get_tid, 1),	/*   12 */
  DEF_SYSCALL (ref_tsk, 2),	/*   13 */

  /* タスク附属同期機能 */
  DEF_SYSCALL (sus_tsk, 1),    	/*   14 */
  DEF_SYSCALL (rsm_tsk, 1),	/*   15 */
  DEF_SYSCALL (frsm_tsk, 1),	/*   16 */
  DEF_SYSCALL (slp_tsk, 0),	/*   17 */
  DEF_NOSYSCALL,		/*   18; tslp_tsk */
  DEF_SYSCALL (wup_tsk, 1),	/*   19 */
  DEF_SYSCALL (can_wup, 2),	/*   20 */
  
  /* 同期・通信機構 */
  /* セマフォ */
  DEF_SYSCALL (cre_sem, 2),	/*   21 */
  DEF_SYSCALL (del_sem, 1),	/*   22 */
  DEF_SYSCALL (sig_sem, 1),	/*   23 */
  DEF_SYSCALL (wai_sem, 1),	/*   24 */
  DEF_SYSCALL (preq_sem, 1),	/*   25 */
  DEF_SYSCALL (twai_sem, 2),	/*   26 */
  DEF_SYSCALL (ref_sem, 2),	/*   27 */

  /* イベントフラグ */
  DEF_SYSCALL (cre_flg, 2),	/*   28 */
  DEF_SYSCALL (del_flg, 1),	/*   29 */
  DEF_SYSCALL (set_flg, 2),	/*   30 */
  DEF_SYSCALL (clr_flg, 2),     /*   31 */
  DEF_SYSCALL (wai_flg, 4),	/*   32 */
  DEF_SYSCALL (pol_flg, 4),	/*   33 */
  DEF_SYSCALL (twai_flg, 5),	/*   34 */
  DEF_SYSCALL (ref_flg, 2),	/*   35 */

  /* メッセージバッファ */
  DEF_SYSCALL (cre_mbf, 2),	/*   36	*/
  DEF_SYSCALL (del_mbf, 1),	/*   37 */
  DEF_SYSCALL (snd_mbf, 3),	/*   38 */
  DEF_SYSCALL (psnd_mbf, 3),	/*   39 */
  DEF_SYSCALL (tsnd_mbf, 4),	/*   40 */
  DEF_SYSCALL (rcv_mbf, 3),	/*   41 */
  DEF_SYSCALL (prcv_mbf, 3),	/*   42 */
  DEF_SYSCALL (trcv_mbf, 4),	/*   43 */

  /* 割りこみ管理 */
  DEF_SYSCALL (dis_int, 0),	/*   44 */
  DEF_SYSCALL (ena_int, 0),	/*   45 */

  /* メモリ管理 */
  DEF_SYSCALL (cre_mpl, 2),	/*   46 */
  DEF_SYSCALL (del_mpl, 1),	/*   47 */
  DEF_SYSCALL (get_blk, 3),	/*   48 */
  DEF_SYSCALL (pget_blk, 3),	/*   49 */
  DEF_SYSCALL (rel_blk, 2),	/*   50 */
  DEF_SYSCALL (ref_mpl, 3),	/*   51 */
  DEF_SYSCALL (tget_blk, 2),	/*   52 */

  /* システム管理 */
  DEF_SYSCALL (get_ver, 1),	/*   53 */
  DEF_NOSYSCALL,		/*   54 ref_sys */
  DEF_NOSYSCALL,		/*   55 ref_cfg */
  DEF_NOSYSCALL,		/*   56 def_svc */
  DEF_NOSYSCALL,		/*   57 def_exc */

  /* 時間管理機能 */
  DEF_NOSYSCALL,		/*   58 set_tim */
  DEF_NOSYSCALL,		/*   59 get_tim */
  DEF_SYSCALL (dly_tsk, 1),	/*   60 dly_tsk */
  DEF_SYSCALL (def_cyc, 2),	/*   61 def_cyc */
  DEF_SYSCALL (act_cyc, 2),	/*   62 act_cyc */
  DEF_SYSCALL (ref_cyc, 2),	/*   63 ref_cyc */
  DEF_SYSCALL (def_alm, 2),	/*   64 def_alm */
  DEF_SYSCALL (ref_alm, 2),	/*   65 ref_alm */
  DEF_NOSYSCALL,		/*   66 ret_tmr */

  DEF_SYSCALL (def_int, 2),	/*   67 */
  DEF_NOSYSCALL,		/*   68 */
  DEF_NOSYSCALL,		/*   69 */
  DEF_NOSYSCALL,		/*   70 */
  DEF_NOSYSCALL,		/*   71 */
  DEF_NOSYSCALL,		/*   72 */
  DEF_NOSYSCALL,		/*   73 */
  DEF_NOSYSCALL,		/*   74 */
  DEF_NOSYSCALL,		/*   75 */
  DEF_NOSYSCALL,		/*   76 */
  DEF_NOSYSCALL,		/*   77 */
  DEF_NOSYSCALL,		/*   78 */
  DEF_NOSYSCALL,		/*   79 */
  DEF_NOSYSCALL,		/*   80 */
  DEF_NOSYSCALL,		/*   81 */
  DEF_NOSYSCALL,		/*   82 */
  DEF_NOSYSCALL,		/*   83 */
  DEF_NOSYSCALL,		/*   84 */
  DEF_NOSYSCALL,		/*   85 */
  DEF_NOSYSCALL,		/*   86 */
  DEF_NOSYSCALL,		/*   87 */
  DEF_NOSYSCALL,		/*   88 */
  DEF_NOSYSCALL,		/*   89 */
  DEF_NOSYSCALL,		/*   90 */
  DEF_NOSYSCALL,		/*   91 */
  DEF_NOSYSCALL,		/*   92 */
  DEF_NOSYSCALL,		/*   93 */
  DEF_NOSYSCALL,		/*   94 */
  DEF_NOSYSCALL,		/*   95 */
  DEF_NOSYSCALL,		/*   96 */
  DEF_NOSYSCALL,		/*   97 */
  DEF_NOSYSCALL,		/*   98 */

  DEF_SYSCALL (vsys_inf, 3),	/*   99 */
  DEF_SYSCALL (dbg_puts, 1),	/*  100 */

  /* 仮想メモリ管理システムコール */	
  DEF_SYSCALL (vcre_reg, 6),	/*  101 */
  DEF_SYSCALL (vdel_reg, 2),	/*  102 */
  DEF_SYSCALL (vmap_reg, 3),	/*  103 */
  DEF_SYSCALL (vunm_reg, 3),	/*  104 */
  DEF_SYSCALL (vdup_reg, 3),	/*  105 */
  DEF_SYSCALL (vprt_reg, 3),	/*  106 */
  DEF_SYSCALL (vshr_reg, 3),	/*  107 */
  DEF_SYSCALL (vput_reg, 4),	/*  108 */
  DEF_SYSCALL (vget_reg, 4),	/*  109 */
  DEF_SYSCALL (vsts_reg, 3),	/*  110 */
  DEF_SYSCALL (vget_phs, 3),	/*  111 */
  DEF_NOSYSCALL,		/*  112 */
  DEF_NOSYSCALL,		/*  113 */
  DEF_NOSYSCALL,		/*  114 */
  DEF_NOSYSCALL,		/*  115 */
  DEF_NOSYSCALL,		/*  116 */
  DEF_NOSYSCALL,		/*  117 */
  DEF_NOSYSCALL,		/*  118 */
  DEF_NOSYSCALL,		/*  119 */

  /* その他のシステムコール */
  DEF_SYSCALL (vget_csl, 2),	/*  120 */
  DEF_SYSCALL (vset_cns, 1),	/*  121 */
  DEF_SYSCALL (vsys_msc, 2),	/*  122 */
  DEF_SYSCALL (vcpy_stk, 4),	/*  123 */
  DEF_SYSCALL (vset_ctx, 2),	/*  124 */
  DEF_NOSYSCALL,		/*  125 */
  DEF_NOSYSCALL,		/*  126 */
  DEF_NOSYSCALL,		/*  127 */
  DEF_NOSYSCALL,		/*  128 */
  DEF_NOSYSCALL,		/*  129 */
  DEF_NOSYSCALL,		/*  130 */
  DEF_NOSYSCALL,		/*  131 */
  DEF_NOSYSCALL,		/*  132 */
  DEF_NOSYSCALL,		/*  133 */
  DEF_NOSYSCALL,		/*  134 */
  DEF_NOSYSCALL,		/*  135 */
  DEF_NOSYSCALL,		/*  136 */
  DEF_NOSYSCALL,		/*  137 */
  DEF_NOSYSCALL,		/*  138 */
  DEF_NOSYSCALL,		/*  139 */
  DEF_NOSYSCALL,		/*  140 */
  DEF_NOSYSCALL,		/*  141 */
  DEF_NOSYSCALL,		/*  142 */
  DEF_NOSYSCALL,		/*  143 */
  DEF_NOSYSCALL,		/*  144 */
  DEF_NOSYSCALL,		/*  145 */
  DEF_NOSYSCALL,		/*  146 */
  DEF_NOSYSCALL,		/*  147 */
  DEF_NOSYSCALL,		/*  148 */
  DEF_NOSYSCALL,		/*  149 */
  DEF_NOSYSCALL,		/*  150 */
};

W	nsyscall = sizeof (syscall_table) / sizeof (syscall_table[0]);


/**************************************************************************
 *	syscall --- システムコールの処理部分
 *
 *	呼び出し関数: interrupt (machine/interrupt.s)
 *
 *	システムコールの要求をユーザスタックより取り出し、適当に処理を振り
 *	分ける。
 *
 *	引数:
 *		sysno		システムコール番号
 *		arg_addr	ユーザスタックの top の番地
 */
W
syscall (UW sysno, W *arg_addr)
{
#if 0
  W	i;
  FP	 func;
#endif
  W	errno;

/* システムコール番号のチェック 
 */
  if ((sysno <= 0) || (sysno > nsyscall))
    {
      return (E_OBJ);
    }

  errno = (syscall_table[sysno].func)(arg_addr);

  /* 
   * システムコール処理用のデバッグ文。
   */
#ifdef notdef  
  printk ("%s, %d, syscall: called. (errno = %d)\n", 
	  __FILE__, __LINE__, errno);		/* R.Naitoh */
#endif /* notdef */

  return (errno);
}

/* --------------------------------------------------------------------- */
static ER
nodef (VP argp)
{
  return (E_NOSPT);
}

