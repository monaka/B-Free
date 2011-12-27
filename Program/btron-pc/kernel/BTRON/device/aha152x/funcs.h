/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/aha152x/funcs.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/* 
 * $Log: funcs.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1999-12-02 16:02:36  kishida0
 * Here is start.
 *
 * Revision 1.1  1999/07/06 13:53:31  kishida0
 * add debug aha152x src
 *
 *
 *
 *
 */

/*
 *
 */
#ifndef	__FUNCS_H__
#define	__FUNCS_H__	1

// aha152xlow.c
void outp(UW ind, UB data);
UW get_config_data(int iBusNum, int iDevNum, int iFuncNum, int iRegAddr);
ER probe (struct device *dev);
void start(void);
ER process_request (DDEV_REQ *req);

ER open_isa_scsi (ID caller, DDEV_OPN_REQ *packet);
ER close_isa_scsi (ID caller, DDEV_CLS_REQ *packet);
ER read_isa_scsi (ID caller, DDEV_REA_REQ *packet);
ER write_isa_scsi (ID caller, DDEV_WRI_REQ *packet);
ER control_isa_scsi (ID caller, DDEV_CTL_REQ *packet);


/* misc.c */
extern void     init_log (void);
extern void     print_digit (UW d, UW base);
extern W        printf (B *fmt,...);
extern W        put_string (B *line, ID port);
extern W        putc (int ch, ID port);








#endif /* __FUNCS_H__ */
