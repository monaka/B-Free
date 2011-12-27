/*

Copyright 1994,1995 (C) B-Free Project.


B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

*/
/* $Id: scsi.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: scsi.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.1  1995-12-05 15:18:02  night
 * 最初の登録。
 * このバージョンでは、デバイスから来たエラーのチェックを行っていないので、
 * 注意すること。
 *
 *
 */

#ifndef __SCSI_H__
#define __SCSI_H__	1

#include "h/itron.h"
#include <h/types.h>
#include <h/errno.h>
#include <pc9801/pc98.h>
#include "device.h"
#include "wd33c93.h"
#include "dma.h"

#define MAXTARGET	7
#define MAXPART		16
#define MAXCDB		50
#define SCSI_DRIVER	"driver.scsi"
#define SCSI_DMA	0

#define BLOCK_SIZE(target)	(disk_table[target].block_size)

#define SYSTEM_SCSIID	0x80000482

#define LOGICAL_TO_PHYSICAL(target,lun,part,lblock) \
	(lblock * (disk_table[target].partition_table[part].bootinfo.sec_byte / disk_table[target].block_size))

#define BYTE_TO_BLOCK(target,byte) \
        (byte / disk_table[target].block_size)

enum scsi_control
{
  SCSI_GET_INFO,
  SCSI_GET_PARTITION,
};

#ifdef DOS
struct boot
{
  UB	ver[8];
  UH	sec_byte;
  UB	cls_sec;
  UH	bootsec;	/* reserve */
  UB	nfat;
  UH	rootsize;
  UH	sec_disk;
  UB	disk_type;
  UH	fat_sec;
  UH	track_sec;
  UH	head_disk;
};
#endif /* DOS */

struct partition
{
  UW	start;
  UW	end;
  UW	size;	/* Block 単位 */
#ifdef DOS
  struct boot	bootinfo;	/* DOS */
#endif
};

struct diskparam
{
  UB	scsiid;
  UB	sec_track;
  UB	head;
  UW	cylinder;
  UW	block_size;	/* 物理ブロックサイズ */
  UB	cylinder_length;
  UW	track_zone;
  UW	rsect_zone;
  UW	rtrack_zone;
  UW	rtrack_unit;
#ifdef DOS
  UW	fat_type;	/* for DOS */
#endif
  struct partition	partition_table[MAXPART];
};

struct adaptor
{
  UW	ioaddr;
  UH	intrn;
  UH	dma_chan;
  UB	ownid;
};

extern struct adaptor	adaptor;
extern struct diskparam	disk_table[8];
extern W	scsi_bus_status;

extern W    	init_scsi(void);	/* 初期化		*/
extern W    	open_scsi();		/* オープン		*/
extern W  	close_scsi();		/* クローズ		*/
extern W    	read_scsi();		/* 読み込み		*/
extern W    	write_scsi();		/* 書き込み		*/
extern W    	control_scsi();		/* コントロール		*/

extern W	read_block_in_partition (W target, W lun, W partition, W start, W length, UB *buf);
extern W	write_block_in_partition (W target, W lun, W partition, W start, W length, UB *buf);

/* scsi.c */
extern void	busywait (W);
extern void	intr_scsi (void);
extern W	write_block (W target, W lun, UW blockno, UW length, UB *buf);

#endif /* __SCSI_H__ */
