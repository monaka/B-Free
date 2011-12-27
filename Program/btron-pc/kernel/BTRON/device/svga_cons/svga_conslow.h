/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 *   svga_cons �ǻ��Ѥ���ƴؿ��� prototype �� extern �����
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/svga_cons/svga_conslow.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 *
 */

#define __SVGA_CONSLOW_H__


#ifdef __SVGA_CONS_C__
/* svga_cons.c proto type define */

void start(void);
static void init_driver (void)
static void main_loop (void)
ER process_request (DDEV_REQ *req)
ER open_svga_cons (ID caller, DDEV_OPN_REQ *packet)
ER close_svga_cons (ID caller, DDEV_CLS_REQ *packet)
ER read_svga_cons (ID caller, DDEV_REA_REQ *packet)
ER write_svga_cons (ID caller, DDEV_WRI_REQ *packet)
ER control_svga_cons (ID caller, DDEV_CTL_REQ *packet)


#else
extern void start(void);
extern ER process_request (DDEV_REQ *req)
extern ER open_svga_cons (ID caller, DDEV_OPN_REQ *packet)
extern ER close_svga_cons (ID caller, DDEV_CLS_REQ *packet)
extern ER read_svga_cons (ID caller, DDEV_REA_REQ *packet)
extern ER write_svga_cons (ID caller, DDEV_WRI_REQ *packet)
extern ER control_svga_cons (ID caller, DDEV_CTL_REQ *packet)

#endif

