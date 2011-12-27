/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/svga_cons/funcs.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/* 
 * $Log: funcs.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1999-04-11 14:41:33  kishida0
 * for debug function
 *
 * Revision 1.1  1998/12/19 07:50:25  monaka
 * Pre release version.
 *
 */

/*
 *
 */
#ifndef	__FUNCS_H__
#define	__FUNCS_H__	1

/* svga_cons.c */
extern ER	process_request (DDEV_REQ *req);
extern ER	open_svga_cons (ID caller, DDEV_OPN_REQ *packet);
extern ER	close_svga_cons (ID caller, DDEV_CLS_REQ *packet);
extern ER	read_svga_cons (ID caller, DDEV_REA_REQ *packet);
extern ER	write_svga_cons (ID caller, DDEV_WRI_REQ *packet);
extern ER	control_svga_cons (ID caller, DDEV_CTL_REQ *packet);


/* svga_conslow.c */
extern ER	probe (struct device *dev);
/*** ここに **/

/* misc.c */
extern void	init_log (void);
extern void	print_digit (UW d, UW base);
extern W	printf (B *fmt,...);
extern W	put_string (B *line, ID port);
extern W 	putc (int ch, ID port);



#endif /* __FUNCS_H__ */
