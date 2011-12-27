/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/kernlib/libkernel.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

#ifndef __LIBKERN_LIBKERNEL_H__
#define __LIBKERN_LIBKERNEL_H__


/* binary.c */
extern void	bzero (B *bufp, W size);
extern void	bcopy (UB *buf1, UB *buf2, W size);


/* message_port.c */
extern ID	get_port (W minsize, W maxsize);
extern W	get_req (ID port, VP request, W *size);



/* port_manager.c */
extern PORT_MANAGER_ERROR	find_port (B *name, ID *rport);
extern ID			alloc_port (W size, W max_entry);


/* string.c */
#ifdef notdef
extern W		strcmp (B *s1, B *s2);
extern W		strncmp (B *s1, B *s2, W length);
extern void		strcpy (B *s1, B *s2);
#endif

extern void		strncpy (B *s1, B *s2, W size);

#ifdef notdef
extern W		strlen (char *s);
#endif



/* sys_debug.c */
extern W	dbg_printf (B *fmt, ...);



#if 0
/* sys_message.c */
extern ER	cre_mbf (ID id, T_CMBF *pk_cmbf);
extern ER	del_mbf (ID id);
extern ER	snd_mbf (ID id, INT size, VP msg);
extern ER	psnd_mbf (ID id, INT size, VP msg);
extern ER	tsnd_mbf (ID id, INT size, VP msg, TMO tmout);
extern ER	rcv_mbf (VP msg, INT *size, ID id);
extern ER	prcv_mbf (VP msg, INT *size, ID id);
extern ER	trcv_mbf (VP msg, INT *size, ID id, TMO tmout);




/* sys_task.c */
extern ER	cre_tsk (ID tskid, T_CTSK *pk_ctsk);
extern ER	vcre_tsk (T_CTSK *pk_ctsk, ID *rid);
extern ER	ext_tsk (void);
extern ER	exd_tsk (void);
extern ER	can_wup (void);
extern ER	chg_pri (void);
extern ER	dis_dsp (void);
extern ER	ena_dsp (void);
extern ER	frsm_tsk(void);
extern ER	get_tid (ID *rid);
extern ER	ref_tsk (void);
extern ER	rot_rdq (void);
extern ER	rsm_tsk (void);
extern ER	slp_tsk (void);
extern ER	sta_tsk (void);
extern ER	sus_tsk (void);
extern ER	ter_tsk (ID tskid);
extern ER	wup_tsk (void);


/* sys_virtual.c */
extern ER	vcre_reg (ID id, VP start, W min, W max, UW perm, FP handle);
extern ER	vdel_reg (ID taskid, VP start);
extern ER	vunm_reg (ID id, VP start, UW size);
extern ER	vmap_reg (ID id, VP start, UW size, W accmode);
extern ER	vget_reg (ID id, VP start, UW size, VP buf);
extern ER	vput_reg (ID id, VP start, UW size, VP buf);

extern ER	vget_phs (ID id, VP addr, UW *paddr);
#else
#include "itron_syscall_func.h"
#endif


/* malloc.c */
extern ER	init_malloc (UW free_memory_erea);
extern VP	malloc (UW size);
extern VP	calloc (UW size, UW nentry);
extern void	free (VP addr);




extern ER	sys_slp ();


extern PORT_MANAGER_ERROR	regist_port (PORT_NAME *name, ID port);



#endif /* __LIBKERN_LIBKERNEL_H__ */
