/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/console/device.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: device.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.1  1995-09-12 18:18:14  night
 * 最初の登録
 *
 *
 */

/* device.h --- デバイスドライバのための構造体の定義
 *
 */


#ifndef __DEVICE_H__
#define __DEVICE_H__	1

#define DRIVER_BUFSIZE	1024

typedef enum { CHAR_DEV, BLOCK_DEV } DEVICE_TYPE;

/**************************************************************************
 *	DEV_STATE	デバイスの管理情報
 */
typedef struct
{
  UW	attr;		/* デバイスの属性			*/
  UW	mode;		/* 現在のアクセスモード			*/
  W	blksz;		/* 物理ブロックサイズ(バイト数)		*/
  W	wptr;		/* 0:書き込み許可   1:書き込み禁止	*/
} DEV_STATE;

/***********************************************************************
 * DEVICE --- デバイスドライバのための情報
 *
 */
typedef struct 
{
  DEVICE_TYPE	type;			/* デバイスドライバのタイプ	  */
  TPTR		name;			/* デバイスドライバの名前	  */
  W		(*init_dev)();		/* 初期化関数へのポインタ	  */
  W		(*open_dev)();		/* opn_dev システムコールで使用	  */
  W		(*close_dev)();		/* cls_dev システムコールで使用	  */
  W		(*read_dev)();		/* rea_dev システムコールで使用	  */
  W		(*write_dev)();		/* wri_dev システムコールで使用	  */
  W		(*control_dev)();	/* ctl_dev システムコールで使用	  */
  W		(*change_access)();	/* chg_dmd システムコールで使用	  */
  W		(*dev_status)();	/* dev_sts システムコールで使用	  */
  W		(*dev_intr)();		/* 割り込みハンドラ		  */
  W		intr_vec;		/* 割り込みベクタ番号		  */
  W		intr_level;		/* 割り込みマスクレベル		  */
  UW		mode;			/* デバイスのアクセスモード       */
} DEVICE;


/*
 * ドライバへ渡すコマンド一覧
 */
enum driver_command
{
  init_dev,
  open_dev,
  read_dev,
  write_dev,
  control_dev,
};

/*
 * ドライバとのやりとりに使用するメッセージパケット
 */
struct driver_packet
{
  enum driver_command	command;
  W			rerrno;		/* 返り値 */

  W			offset;
  W			length;

  unsigned char		buf[DRIVER_BUFSIZE];
};

typedef struct driver_packet driver_packet_t;


#define DEVID(id)	(id >> 16)
#define UNIT(id)	((id >> 8) & 0xff)
#define SUBUNIT(id)	(id & 0xff)
#define MAKEDEVID(dev,unit,subunit)	((dev << 16) | (unit << 8) | subunit)

#endif __INCLUDE_DEVICE_H__
