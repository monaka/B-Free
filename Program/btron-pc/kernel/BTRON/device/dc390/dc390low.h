/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /usr/local/src/master/B-Free/Program/btron-pc/kernel/BTRON/devic
e/dc390/dc390low.c,v 1.1 1999/04/26 18:33:32 kishida0 Exp $ */
static char rcs[] = "@(#)$Header: /usr/local/src/master/B-Free/Program/btron-pc/
kernel/BTRON/device/dc390/dc390low.c,v 1.1 1999/04/26 18:33:32 kishida0 Exp $";


/*
 *
 *
 */
#ifndef __DC390_H__
#define __DC390_H__


// レジスタ一覧

#define CTCREG0  SCSI-REG-BASE +0x00
#define STCREG0  SCSI-REG-BASE +0x00
#define STCREG1  SCSI-REG-BASE +0x04
#define STCREG1  SCSI-REG-BASE +0x04
#define FFREG    SCSI-REG-BASE +0x08
#define CMDREG   SCSI-REG-BASE +0x0C
#define STATREG  SCSI-REG-BASE +0x10
#define SDIDREG  SCSI-REG-BASE +0x10
#define INSTREG  SCSI-REG-BASE +0x14
#define STIMREG  SCSI-REG-BASE +0x14
#define ISREG    SCSI-REG-BASE +0x18
#define STPREG   SCSI-REG-BASE +0x18
#define CFIREG   SCSI-REG-BASE +0x1C
#define SOFREG   SCSI-REG-BASE +0x1C
#define CNTLREG1 SCSI-REG-BASE +0x20
#define CLKFREG1 SCSI-REG-BASE +0x24
#define RES0     SCSI-REG-BASE +0x18
#define CNTLREG2 SCSI-REG-BASE +0x2C
#define CNTLREG3 SCSI-REG-BASE +0x30
#define CNTLREG4 SCSI-REG-BASE +0x34
#define CTCREG2  SCSI-REG-BASE +0x38
#define STCREG2  SCSI-REG-BASE +0x38
#define RES1     SCSI-REG-BASE +0x3C




// とりあえずデバッグ用に作ってみる



#endif //__DC390_H__

/* 
 * $Log: dc390low.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1999-06-20 14:46:44  kishida0
 * write defines for debug.
 *
 * 
 *
 */
