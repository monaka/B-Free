/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/ne2000/funcs.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/* 
 * $Log: funcs.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.6  1999-03-07 12:42:49  night
 * write_ne2000 () �Υץ�ȥ�����ʸ���ѹ���
 * select_page1 () ���ɲá�
 * set_interrupt_handler () �Υץ�ȥ�����ʸ���ѹ���
 *
 * Revision 1.5  1999/03/02 15:13:33  night
 * interrupt.c �����äƤ���ؿ��ˤĤ��Ƥ���Ͽ��Ԥä���
 *
 * extern ER	set_interrupt_handler (W intno, FP handler);
 * extern void	ne2000_interrupt_handler (void);
 *
 * Revision 1.4  1997/10/28 14:43:38  night
 * �ʲ��δؿ�������ɲá�
 *
 * extern ER	probe_1 (struct device *dev);
 * extern ER	reset_adaptor (struct device *dev);
 * extern W	read_saprom (struct device *dev);
 *
 * Revision 1.3  1997/10/28 12:43:48  night
 * probe() ��������ѹ���
 *
 * Revision 1.2  1997/05/17 10:41:15  night
 * �ե������ʸ�������ɤ� SJIS ���� EUC ���ѹ���
 *
 * Revision 1.1  1997/05/06 12:43:30  night
 * �ǽ����Ͽ
 *
 *
 */

/*
 *
 */
#ifndef	__FUNCS_H__
#define	__FUNCS_H__	1

/* ne2000.c */
extern ER	process_request (DDEV_REQ *req);
extern ER	open_ne2000 (ID caller, DDEV_OPN_REQ *packet);
extern ER	close_ne2000 (ID caller, DDEV_CLS_REQ *packet);
extern ER	read_ne2000 (ID caller, DDEV_REA_REQ *packet);
extern ER	write_ne2000 (ID caller, ID tskid, DDEV_WRI_REQ *packet);
extern ER	control_ne2000 (ID caller, DDEV_CTL_REQ *packet);


/* ne2000low.c */
extern ER	probe (struct ne2000_device *dev);
extern ER	probe_1 (struct ne2000_device *dev);
extern ER	reset_adaptor (struct ne2000_device *dev);
extern W	read_saprom (struct ne2000_device *dev);
extern void	select_page0 (struct ne2000_device *dev);
extern void	select_page1 (struct ne2000_device *dev);


/* misc.c */
extern void	init_log (void);
extern void	print_digit (UW d, UW base);
extern W	printf (B *fmt,...);
extern W	put_string (B *line, ID port);
extern W 	putc (int ch, ID port);

/* interrupt.c */
extern ER	set_interrupt_handler (struct ne2000_device *dev, W intno, FP handler);
extern void	ne2000_interrupt_handler (void);



#endif /* __FUNCS_H__ */
