/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd/ide.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 *
 * $Log: ide.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.8  1999-02-28 22:11:33  night
 * 関数 get_ide_parameter() の extern 宣言の追加。
 *
 * Revision 1.7  1998/11/20 08:02:29  monaka
 * *** empty log message ***
 *
 * Revision 1.6  1998/06/10 13:42:03  night
 * コメントの追加。
 *
 * Revision 1.5  1998/06/09 14:06:16  night
 * 構造体 ide_partition の変更。
 * HD に記録してあるパーティション情報そのままの形式に変更した。
 * (これまでは、HD に記録してある形式とは変えてあった)
 *
 * Revision 1.4  1998/03/01 05:18:21  night
 * ide_id 構造体の中に ecc_bytes という要素を追加。
 *
 * Revision 1.3  1998/02/25 13:07:12  night
 * ide_read_dump 関数のプロトタイプ宣言の追加。
 *
 * Revision 1.2  1997/08/31 14:24:49  night
 * ide ディスクの ID 情報の読み取りを行う処理のための定義を追加。
 *
 * Revision 1.1  1996/08/11  15:12:08  night
 * 最初の登録
 *
 *
 */

#ifndef __IDE_H__
#define __IDE_H__

#define IDE_BLOCK_SIZE		512
#define IDE_MAX_PARTITION	4

#define IDE_STAT_REG		0x1f7
#define IDE_COMMAND_REG		0x1f7
#define IDE_ALT_STAT_REG	0x3f6
#define IDE_CONTROL_REG		0x3f6
#define IDE_ADDR_REG		0x3f7	/* ディジタルインプットレジスタ */
#define IDE_ERROR_REG		0x1f1
#define IDE_DATA_REG		0x1f0
#define IDE_FEATURE_REG		0x1f1
#define IDE_SCOUNT_REG		0x1f2
#define IDE_SNUMBER_REG		0x1f3
#define IDE_CYL_LOW_REG		0x1f4
#define IDE_CYL_HIGH_REG	0x1f5
#define IDE_DRIVE_REG		0x1f6	/* ドライブ/ヘッドレジスタ */

#define	IDE_CMD_READ_INFO	0xec
#define IDE_CMD_READ		0x20

#define IDE_DRQ			0x08

#define HEAD(table,part)	(table[0x1bf])
#define CYLINDER(table,part)	(table[0x1c1])
#define SECTOR(table,part)	(table[0x1c0])

#define PARTITION_SIZE		(0x10)
#define PARTITION_OFFSET	(0x1be)
#define BOOT_FLAG_OFFSET	(0)
#define HEAD_OFFSET		(1)
#define SECTOR_OFFSET		(2)
#define CYLINDER_OFFSET		(3)
#define TOTAL_SECTOR_OFFSET	(12)
#define TYPE_OFFSET		(4)


struct ide_id
{
  unsigned short	config;				/*  0 */
  unsigned short	n_cylinder;			/*  1 */
  unsigned short	pad0;				/*  2 */
  unsigned short	n_head;				/*  3 */
  unsigned short	track_byte_unformat;		/*  4 */
  unsigned short	sector_byte_unformat;		/*  5 */
  unsigned short	n_sector;			/*  6 */
  unsigned short	pad1[3];			/*  7,8,9 */
  unsigned char		number[20];			/* 10 - 19 */
  unsigned short	buffer_type;			/* 20 */
  unsigned short	buffer_size;			/* 21 */
  unsigned short	ecc_bytes;			/* 22 - ecc bytes */
  unsigned char		firm_ware[8];			/* 23 - 26 */
  unsigned char		model[40];			/* 27 - 46 */
  unsigned short	pad2;
  unsigned short	double_word_io;
  unsigned short	dma_support;
  unsigned short	pad3;
  unsigned short	pio_config;
  unsigned short	dma_config;
  unsigned short	trans_mode;
  unsigned short	now_cylinder;
  unsigned short	now_head;
  unsigned short	now_sector_track;
  unsigned short	now_sector[2];
/*  unsigned short	pad4[197]; */
};


struct ide_partition
{
  unsigned char		boot_flag;
  unsigned char		begin[3];	/* header, sector, cylinder
					 * の順にならんでいる。
					 * ただし、sector の上位 2 ビットは、
					 * シリンダの上位 2 ビットを補完する。
					 */
  unsigned char		type;
  unsigned char		end[3];		/* header, sector, cylinder
					 * の順にならんでいる。
					 * ただし、sector の上位 2 ビットは、
					 * シリンダの上位 2 ビットを補完する。
					 */
  unsigned long		start;
  unsigned long		length;
};


extern void ide_init (void);
extern int ide_read (int drive, int partition, ULONG blockno, BYTE *buf, int length);
extern int test_ide (char **arg);
extern int ide_boot (char **arg);
extern void ide_read_dump (char **arg);
extern int get_ide_parameter (struct ide_id *id);

extern void ide_id (void);

#endif /* __IDE_H__ */
