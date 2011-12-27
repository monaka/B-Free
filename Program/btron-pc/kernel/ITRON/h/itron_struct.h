/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* itron_struct.h
 *
 */

#ifndef _ITRON_STRUCT_H_
#define _ITRON_STRUCT_H_	1

/* --------------------- 全体 ------------------ */

#define NADR	(-1)
#ifndef TRUE
# define TRUE	(1)
#endif
#ifndef FALSE
# define FALSE	(0)
#endif

#define TMO_POL		(0)
#define TMO_FEVR	(-1)

/* -----------------------------  機能コード ------------------------ */

#define TFN_CRE_TSN	(-17)
#define TFN_STA_TSK	(-23)

/* ... まだまだ続く ... */


/* ------------------------ タスク管理関係 --------------------- */

typedef enum
{
  kernel_mode,
  user_mode
} TSKMODE;


typedef struct t_ctsk
{
  VP		exinf;		/* 拡張情報		*/
  ATR		tskatr;		/* タスク属性		*/
  FP		startaddr;	/* タスク起動アドレス	*/
  PRI		itskpri;	/* タスク起動時優先度	*/
  INT		stksz;		/* スタックサイズ	*/
  ADDR_MAP	*addrmap;	/* アドレスマップ	*/
  				/* 仮想メモリのマップ	*/
  				/* テーブル		*/
  TSKMODE	mode;		/* タスク起動時のモード */
  				/* ユーザモードまたは、 */
  				/* タスクモードのどちら */
                                /* かとなる		*/
} T_CTSK;

#define TA_ASM	(0x00000000)	/* アセンブラによるプログラム	*/
#define TA_HLNG	(0x00000001)	/* 高級言語によるプログラム	*/

#define TSK_SELF	(0)	/* 自タスク指定			*/

#define TPRI_INI	(0)	/* タスク起動時の初期優先度を指定 
				   (chg_pri) */
#define TPRI_RUN	(0)	/* その時実行中の最高優先度を指定
				   (rot_rdq) */

/* T_RTSK
 *
 */
typedef struct t_rtsk
{
  VP	exinf;
  PRI	tskpri;			/* タスクの優先順位			*/
  UINT	tskstat;		/* タスク状態				*/
  W	total_time;		/* タスクの総使用時間			*/
} T_RTSK;

#define TTS_NON		(0x00000000)	/* NON-EXIST	*/
#define TTS_RUN		(0x00000001)	/* RUN		*/
#define TTS_RDY		(0x00000002)	/* READY	*/
#define TTS_WAI		(0x00000004)	/* WAIT		*/
#define TTS_SUS		(0x00000008)	/* SUSPEND	*/
#define TTS_WAS		(0x0000000c)	/* WAIT-SUSPEND	*/
#define TTS_DMT		(0x00000010)	/* DORMANT	*/

#define TTW_SLP		(0x00000001)	/* slp_tsk, tslp_tsk による待ち */
#define TTW_DLY 	(0x00000002)	/* dly_tsk による待ち		*/
#define TTW_NOD		(0x00000008)	/* 接続機能応答待ち		*/
#define TTW_FLG		(0x00000010)	/* wai_flg, twai_flg による待ち */
#define TTW_SEM		(0x00000020)	/* wai_sem, twai_sem による待ち */
#define TTW_MBX		(0x00000040)	/* rcv_msg, trcv_msg による待ち */
#define TTW_SMBF	(0x00000080)	/* snd_mbf, tsnd_mbf による待ち */
#define TTW_MBF		(0x00000100)	/* rcv_mbf, trcv_mbf による待ち */
#define TTW_CAL		(0x00000200)	/* ランデブ呼び出し待ち		*/
#define TTW_ACP		(0x00000400)	/* ランデブ受け付け待ち		*/
#define TTW_RDY		(0x00000800)	/* ランデブ終了待ち		*/
#define TTW_MPL		(0x00001000)	/* get_blk, tget_blk による待ち */
#define TTW_MPF		(0x00002000)	/* get_blf, tget_blf による待ち */

/* --------------------- セマフォ関係 ------------------------------------ */

#define TA_UNDEF	(0xffffffff)	/* 初期化されていないセマフォ/メッセージ */
#define TA_NULL		(0)
#define TA_TFIFO	(0x00000000)
#define TA_TPRI		(0x00000001)

#define TMBF_OS		(-4)	/* OS のエラーログ用のメッセージバッファ */
#define TMBF_DB		(-3)	/* デバッグ用のメッセージバッファ 	 */	


typedef struct t_csem 
{
  VP	exinf;
  ATR	sematr;

  INT	isemcnt;
  INT	maxsem;
} T_CSEM;

typedef struct t_rsem
{
  VP		exinf;
  BOOL_ID	wtsk;
  INT		semcnt;
} T_RSEM;

/* --------------------- イベントフラグ関係 ------------------------------ */

#define TA_WSGL		(0x00000000)   	/* 複数タスクの待ちを許さない */
#define TA_WMUL		(0x00000008)	/* 複数タスクの待ちを許す     */
#define TA_FREE		(0xffffffff)	/* 使用していないフラグ	      */

#define TWF_ANDW	(0x00000000)
#define TWF_ORW		(0x00000002)
#define TWF_CLR		(0x00000001)


/* cre_flg */
typedef struct t_cflg {
  VP 	exinf;
  ATR	flgatr;
  UINT	iflgptn;
} T_CFLG;


/* ref_flg */
typedef struct t_rflg {
  VP		exinf;
  BOOL_ID	wtsk;
  UINT		flgptn;
} T_RFLG;


/* ------------------ メッセージバッファ関係 ----------------------------- */

typedef struct t_cmbf 
{
  VP		exinf;
  ATR		mbfatr;
  INT		bufsz;
  INT		maxmsz;

} T_CMBF;

typedef struct t_rmbf
{
  VP		exinf;
  BOOL_ID	wtsk;
  BOOL_ID	stsk;
  INT		msgsz;
  INT	frbufsz;

} T_RMBF;


/* -------------------- ポート、ランデブ関係 --------------- */

typedef struct t_cpor
{
  VP		exinf;
  ATR		poratr;
  INT		maxcmsz;
  INT		maxrmsz;
} T_CPOR;


typedef struct t_rpor
{
  VP		exinf;
  BOOL_ID	wtsk;
  BOOL_ID	stsk;
} T_RPOR;


/* ------------------- 割込み管理関係 ------------------ */

typedef struct t_dint
{
  ATR		intatr;
  FP		inthdr;
} T_DINT;

#define ATR_TRAP	(1)
#define ATR_FAULT	(2)
#define ATR_INTR	(3)


/* ------------------- メモリプール関係 ----------------- */

typedef struct t_cmpl
{
  VP		exinf;
  ATR		mplatr;
  INT		mplsz;
  INT		align;		/* アライメントサイズ */
} T_CMPL;


#define TMPL_SYS		(0)


typedef struct t_rmpl
{
  VP		exinf;
  BOOL_ID	wtsk;
  INT		frsz;
  INT		maxsz;
} T_RMPL;


typedef struct t_cmpf
{
  VP		exinf;
  ATR		mpfatr;
  INT		mpfcnt;
  INT		bufsz;
} T_CMPF;

typedef struct t_rmpf
{
  VP		exinf;
  BOOL_ID	wtsk;
  INT		frbcnt;
} T_RMPF;


/* -------------------- 時間管理関係 ------------------ */

typedef unsigned long	DLYTIME;

typedef struct t_systime
{
  H	utime;
  UW	ltime;
} SYSTIME, CYCTIME, ALMTIME;


typedef struct t_dcyc
{
  VP		exinf;
  ATR		cycatr;
  FP		cychdr;
  UINT		cycact;
  CYCTIME	cyctim;
} T_DCYC;


#define TCY_OFF		(0x00000000)
#define TCY_ON		(0x00000001)
#define TCY_INI		(0x00000002)


typedef struct t_rccy
{
  VP		exinf;
  CYCTIME	lfttim;
  UINT		cycact;
} T_RCYC;


typedef struct t_dalm
{
  VP		exinf;
  ATR		almatr;
  FP		almhdr;
  UINT		tmmode;
  ALMTIME	almtim;
} T_DALM;


#define TTM_ABS		(0x00000000)
#define TTM_REL		(0x00000001)


typedef struct t_ralm
{
  VP		exinf;
  ALMTIME	lfttim;
} T_RALM;


/* ------------------- システム管理関係 ------------------- */

typedef struct t_ver
{
  UH	maker;
  UH	id;
  UH	spver;
  UH	prver;
  UH	prno[4];
  UH	cpu;
  UH	var;
} T_VER;

typedef struct t_rsys
{
  INT	sysstat;
} T_RSYS;

#define TSS_TSK		(0)
#define TSS_DDSP	(1)
#define TSS_LOC		(3)
#define TSS_INDP	(4)


typedef struct t_rcfg
{
} T_RCFG;


typedef struct t_dsvc
{
  ATR	svcatr;
  FP	svchdr;
} T_DSVC;


typedef struct t_dexc
{
  ATR	excatr;
  FP	exchdr;
} T_DEXC;


#define ROOTFS_INFO	(1)



/* ------------------ ネットワーク管理関係 ------------- */

#define	TND_SELF	(0)
#define TND_OTHR	(-1)


#endif /* _ITRON_STRUCT_H_ */

