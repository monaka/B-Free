/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/console/device.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: device.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.1  1995-09-12 18:18:14  night
 * �ǽ����Ͽ
 *
 *
 */

/* device.h --- �ǥХ����ɥ饤�ФΤ���ι�¤�Τ����
 *
 */


#ifndef __DEVICE_H__
#define __DEVICE_H__	1

#define DRIVER_BUFSIZE	1024

typedef enum { CHAR_DEV, BLOCK_DEV } DEVICE_TYPE;

/**************************************************************************
 *	DEV_STATE	�ǥХ����δ�������
 */
typedef struct
{
  UW	attr;		/* �ǥХ�����°��			*/
  UW	mode;		/* ���ߤΥ��������⡼��			*/
  W	blksz;		/* ʪ���֥�å�������(�Х��ȿ�)		*/
  W	wptr;		/* 0:�񤭹��ߵ���   1:�񤭹��߶ػ�	*/
} DEV_STATE;

/***********************************************************************
 * DEVICE --- �ǥХ����ɥ饤�ФΤ���ξ���
 *
 */
typedef struct 
{
  DEVICE_TYPE	type;			/* �ǥХ����ɥ饤�ФΥ�����	  */
  TPTR		name;			/* �ǥХ����ɥ饤�Ф�̾��	  */
  W		(*init_dev)();		/* ������ؿ��ؤΥݥ���	  */
  W		(*open_dev)();		/* opn_dev �����ƥॳ����ǻ���	  */
  W		(*close_dev)();		/* cls_dev �����ƥॳ����ǻ���	  */
  W		(*read_dev)();		/* rea_dev �����ƥॳ����ǻ���	  */
  W		(*write_dev)();		/* wri_dev �����ƥॳ����ǻ���	  */
  W		(*control_dev)();	/* ctl_dev �����ƥॳ����ǻ���	  */
  W		(*change_access)();	/* chg_dmd �����ƥॳ����ǻ���	  */
  W		(*dev_status)();	/* dev_sts �����ƥॳ����ǻ���	  */
  W		(*dev_intr)();		/* �����ߥϥ�ɥ�		  */
  W		intr_vec;		/* �����ߥ٥����ֹ�		  */
  W		intr_level;		/* �����ߥޥ�����٥�		  */
  UW		mode;			/* �ǥХ����Υ��������⡼��       */
} DEVICE;


/*
 * �ɥ饤�Ф��Ϥ����ޥ�ɰ���
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
 * �ɥ饤�ФȤΤ��Ȥ�˻��Ѥ����å������ѥ��å�
 */
struct driver_packet
{
  enum driver_command	command;
  W			rerrno;		/* �֤��� */

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
