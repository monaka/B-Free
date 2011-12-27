/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* itron_struct.h
 *
 */

#ifndef _ITRON_STRUCT_H_
#define _ITRON_STRUCT_H_	1

/* --------------------- ���� ------------------ */

#define NADR	(-1)
#ifndef TRUE
# define TRUE	(1)
#endif
#ifndef FALSE
# define FALSE	(0)
#endif

#define TMO_POL		(0)
#define TMO_FEVR	(-1)

/* -----------------------------  ��ǽ������ ------------------------ */

#define TFN_CRE_TSN	(-17)
#define TFN_STA_TSK	(-23)

/* ... �ޤ��ޤ�³�� ... */


/* ------------------------ �����������ط� --------------------- */

typedef enum
{
  kernel_mode,
  user_mode
} TSKMODE;


typedef struct t_ctsk
{
  VP		exinf;		/* ��ĥ����		*/
  ATR		tskatr;		/* ������°��		*/
  FP		startaddr;	/* ��������ư���ɥ쥹	*/
  PRI		itskpri;	/* ��������ư��ͥ����	*/
  INT		stksz;		/* �����å�������	*/
  ADDR_MAP	*addrmap;	/* ���ɥ쥹�ޥå�	*/
  				/* ���ۥ���Υޥå�	*/
  				/* �ơ��֥�		*/
  TSKMODE	mode;		/* ��������ư���Υ⡼�� */
  				/* �桼���⡼�ɤޤ��ϡ� */
  				/* �������⡼�ɤΤɤ��� */
                                /* ���Ȥʤ�		*/
} T_CTSK;

#define TA_ASM	(0x00000000)	/* ������֥�ˤ��ץ����	*/
#define TA_HLNG	(0x00000001)	/* ������ˤ��ץ����	*/

#define TSK_SELF	(0)	/* ������������			*/

#define TPRI_INI	(0)	/* ��������ư���ν��ͥ���٤���� 
				   (chg_pri) */
#define TPRI_RUN	(0)	/* ���λ��¹���κǹ�ͥ���٤����
				   (rot_rdq) */

/* T_RTSK
 *
 */
typedef struct t_rtsk
{
  VP	exinf;
  PRI	tskpri;			/* ��������ͥ����			*/
  UINT	tskstat;		/* ����������				*/
  W	total_time;		/* ������������ѻ���			*/
} T_RTSK;

#define TTS_NON		(0x00000000)	/* NON-EXIST	*/
#define TTS_RUN		(0x00000001)	/* RUN		*/
#define TTS_RDY		(0x00000002)	/* READY	*/
#define TTS_WAI		(0x00000004)	/* WAIT		*/
#define TTS_SUS		(0x00000008)	/* SUSPEND	*/
#define TTS_WAS		(0x0000000c)	/* WAIT-SUSPEND	*/
#define TTS_DMT		(0x00000010)	/* DORMANT	*/

#define TTW_SLP		(0x00000001)	/* slp_tsk, tslp_tsk �ˤ���Ԥ� */
#define TTW_DLY 	(0x00000002)	/* dly_tsk �ˤ���Ԥ�		*/
#define TTW_NOD		(0x00000008)	/* ��³��ǽ�����Ԥ�		*/
#define TTW_FLG		(0x00000010)	/* wai_flg, twai_flg �ˤ���Ԥ� */
#define TTW_SEM		(0x00000020)	/* wai_sem, twai_sem �ˤ���Ԥ� */
#define TTW_MBX		(0x00000040)	/* rcv_msg, trcv_msg �ˤ���Ԥ� */
#define TTW_SMBF	(0x00000080)	/* snd_mbf, tsnd_mbf �ˤ���Ԥ� */
#define TTW_MBF		(0x00000100)	/* rcv_mbf, trcv_mbf �ˤ���Ԥ� */
#define TTW_CAL		(0x00000200)	/* ���ǥָƤӽФ��Ԥ�		*/
#define TTW_ACP		(0x00000400)	/* ���ǥּ����դ��Ԥ�		*/
#define TTW_RDY		(0x00000800)	/* ���ǥֽ�λ�Ԥ�		*/
#define TTW_MPL		(0x00001000)	/* get_blk, tget_blk �ˤ���Ԥ� */
#define TTW_MPF		(0x00002000)	/* get_blf, tget_blf �ˤ���Ԥ� */

/* --------------------- ���ޥե��ط� ------------------------------------ */

#define TA_UNDEF	(0xffffffff)	/* ���������Ƥ��ʤ����ޥե�/��å����� */
#define TA_NULL		(0)
#define TA_TFIFO	(0x00000000)
#define TA_TPRI		(0x00000001)

#define TMBF_OS		(-4)	/* OS �Υ��顼���ѤΥ�å������Хåե� */
#define TMBF_DB		(-3)	/* �ǥХå��ѤΥ�å������Хåե� 	 */	


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

/* --------------------- ���٥�ȥե饰�ط� ------------------------------ */

#define TA_WSGL		(0x00000000)   	/* ʣ�����������Ԥ�������ʤ� */
#define TA_WMUL		(0x00000008)	/* ʣ�����������Ԥ������     */
#define TA_FREE		(0xffffffff)	/* ���Ѥ��Ƥ��ʤ��ե饰	      */

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


/* ------------------ ��å������Хåե��ط� ----------------------------- */

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


/* -------------------- �ݡ��ȡ����ǥִط� --------------- */

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


/* ------------------- ����ߴ����ط� ------------------ */

typedef struct t_dint
{
  ATR		intatr;
  FP		inthdr;
} T_DINT;

#define ATR_TRAP	(1)
#define ATR_FAULT	(2)
#define ATR_INTR	(3)


/* ------------------- ����ס���ط� ----------------- */

typedef struct t_cmpl
{
  VP		exinf;
  ATR		mplatr;
  INT		mplsz;
  INT		align;		/* ���饤���ȥ����� */
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


/* -------------------- ���ִ����ط� ------------------ */

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


/* ------------------- �����ƥ�����ط� ------------------- */

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



/* ------------------ �ͥåȥ�������ط� ------------- */

#define	TND_SELF	(0)
#define TND_OTHR	(-1)


#endif /* _ITRON_STRUCT_H_ */

