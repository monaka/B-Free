/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* io.c --- �ǥХ��� IO �˴ؤ��륳��ȥ��륿�
 *
 * �����ؿ���
 *	init_io 	�ɡ��ϥǥХ���������ǽ�ν����
 *	def_dev 	�ǥХ�������Ͽ
 *	get_ioport 	�ǥХ����ѤΥ�å������ݡ��Ȥγ���
 *	io_request 	�ǥХ������Ф����������׵������
 *	io_response 	��Ʊ���⡼�ɤΥǥХ��������ϤΤȤ��ˣɣϤΥ쥹
 *			�ݥ󥹤�����Ȥ�
 *
 *
 *
 */

#include "itron.h"
#include "errno.h"
#include "task.h"
#include "misc.h"
#include "func.h"
#include "interrupt.h"
#include "../h/message.h"
#include "../io/io.h"

#define MAX_QUEUE	10


/**********************************************************************
 * io_device ��¤��
 *
 */
struct io_device
{
  ID			id;
  ID			portid;		/* IO �ǥХ����إ�å��������� */
					/* �뤿��Υ�å������ݡ��� ID */
  IO_TYPE		type;
  B			name[MAX_DEVICE_NAME];
};

struct io_device	device_table[MAX_DEVICE];

static ID	get_ioport (void);



/*************************************************************************
 * init_io --- �ɡ��ϥǥХ���������ǽ�ν����
 *
 * ������
 *	�ʤ�
 *
 * �֤��͡�
 *	���顼�ֹ�
 *
 * ��ǽ��
 *	�ǥХ����ơ��֥�ν����
 *
 */
ER
init_io (void)
{
   W	i;

  for (i = 0; i < MAX_DEVICE; i++)
    {
      device_table[i].id = UNDEF_DEVICE;
    }
  return (E_OK);
}

/************************************************************************
 * def_dev -- �ǥХ�������Ͽ
 *
 * ������
 *	name	�ǥХ���̾
 *	type	�ǥХ���������
 *	id	�ǥХ��� ID 
 *	rid	�ǥХ��� ID (�֤���)
 *
 * �֤��͡�
 *	���顼�ֹ�
 *
 * ��ǽ��
 *	
 *
 *
 */
ER
def_dev (B *name, IO_TYPE type, ID id, ID *rid)
{
  W	i;
  ID	port;

  if (id == ANY_DEVICE)
    {
      for (i = 0; i < MAX_DEVICE; i++)
	{
	  if (device_table[i].id == UNDEF_DEVICE)
	    {
	      id = i;
	      break;
	    }
	}
      if (id == ANY_DEVICE)
	return (E_NOMEM);
    }

  if (device_table[id].id != UNDEF_DEVICE)
    return (E_ID);

  port = get_ioport ();
  if (port <= 0)
    {
      return (E_NOMEM);
    }
  device_table[id].id   = id;
  device_table[id].type = type;
  device_table[id].portid = port;
  bcopy (name, device_table[id].name, strlen (name) + 1);
  *rid = id;

  return (E_OK);
}


/************************************************************************
 * ref_dev -- �ǥХ����λ���
 *
 * ������
 *	name	�ǥХ���̾
 *	type	�ǥХ���������
 *	rid	�ǥХ��� ID (�֤���)
 *
 * �֤��͡�
 *	���顼�ֹ�
 *	E_PAR	�ǥХ��������פ�����
 *	E_OBJ	���ꤵ�줿�ǥХ�����¸�ߤ��ʤ�
 *
 * ��ǽ��
 *	�ǥХ���̾�ȥǥХ��������פ���ǥХ��� ID ���֤���
 *
 *
 */
ER
ref_dev (B *name, IO_TYPE type, ID *rid)
{
  W	i;

  if ((type != CHAR) && (type != BLOCK))
    {
      return (E_PAR);
    }

  for (i = 0; i < MAX_DEVICE; i++)
    {
      if (device_table[i].id != UNDEF_DEVICE)
	{
	  if ((strncmp (name, device_table[i].name, MAX_DEVICE_NAME) == 0) &&
	      (type == device_table[i].type))
	    {
	      *rid = device_table[i].id;
	      return (E_OK);
	    }
	}
    }

  return (E_OBJ);
}


/***********************************************************************
 * get_ioport --- �ǥХ����ѤΥ�å������ݡ��Ȥγ���
 *
 * ������
 *	�ʤ�
 *
 * �֤��͡�
 *	�������	���������ǥХ����ѤΥ�å������ݡ��� ID
 *	�۾����	NULL (0)
 *
 * ��ǽ��
 *	�ǥХ����ѤΥ�å������Хåե��ݡ��Ȥ򥢥����Ȥ��롣
 *	ITRON �Υ�å������Хåե��ˤϥ�å������������λ��꤬ɬ�פ���
 *      ��å������������ϡ�T_IO_REQUESTQ �Ȥ�����
 *	�ʤ����Хåե��Ȥ��� ��å����� 10 ��ʬ����ݤ�����
 */
static ID
get_ioport (void)
{
  W		id;
  T_CMBF	pk_cmbf;
  ER		err;

  pk_cmbf.mbfatr = TA_TFIFO;
  pk_cmbf.bufsz = sizeof (T_IO_REQUEST) * MAX_QUEUE;
  pk_cmbf.maxmsz = sizeof (T_IO_REQUEST);
  for (id = MIN_USERMBFID; id <= MAX_USERMBFID; id++)
    {
      err = cre_mbf (id, &pk_cmbf);
      if (err == E_OK)		/* ��å������ݡ��Ȥ����ݤǤ�����*/
	{
	  break;
	}
      else if (err != E_OBJ)	/*  ID �����ʳ��Υ��顼�Ǽ���: ���顼 */
	{			/*  �꥿����			      */
	  return (NULL);
	}
    }

  if (id > MAX_USERMBFID)
    {
      return (NULL);
    }
  return (id);
}


/*************************************************************************
 * get_req --- �ǥХ����ɥ饤��¦�ǤΥ�å����������դ�
 *
 * ������
 *	device
 *	req
 *
 * �֤��͡�
 *	���顼�ֹ�
 *
 * ��ǽ��
 *
 *
 */
ER
get_ioreq (ID device, T_IO_REQUEST *req)
{
  ER	err;
  INT		rsize;

  if ((device < MIN_DEVID) || (device > MAX_DEVID))
    {
      return (E_ID);
    }

  if (device_table[device].id == UNDEF_DEVICE)
    {
      return (E_ID);
    }

  err = rcv_mbf ((VP)req, &rsize, device_table[device].portid);
  if (err != E_OK)
    {
      return (err);
    }
  return (E_OK);
}


/*************************************************************************
 *
 *
 *
 *
 */
ER
put_res (ID device, T_IO_REQUEST *req, T_IO_RESPONSE *res)
{
  ER	err;

  err = snd_mbf (req->resport, sizeof (T_IO_RESPONSE), res);
  return (err);
}



/*************************************************************************
 * io_request --- �ǥХ������Ф����������׵������
 *
 * ������
 *	device	�׵������ǥХ����Σɣ�
 *	req	�׵�ѥ��å�
 *	res	�����ѥ��å� (��Ʊ���⡼�ɤΤȤ��ˤϻ��Ѥ��ʤ�)
 *	mode	Ʊ��/��Ʊ���⡼�ɤλ���
 *		IO_SYNC		Ʊ���⡼��
 *		IO_ASYNC	��Ʊ���⡼��
 *
 * �֤��͡�
 *	���顼�ֹ桧	���Υ��顼�ֹ�ϥǥХ������׵�����������Ǥ���
 *			���ɤ����������Τ餻���Τǡ��ɡ��Ͻ������Τ�
 *			���ޤ��Ǥ������ɤ����ϼ����ѥ��åȤ���򸫤뤷
 *			���ʤ���
 *	E_OK	���ｪλ
 *
 * ��ǽ��
 *	�׵�ѥ��åȤ˻��ꤵ�줿�ǥХ������Ф��ơ��������׵�����롣
 *	
 *
 */
ER
io_request (ID device, T_IO_REQUEST *req, T_IO_RESPONSE *res, W mode)
{
  ID	res_port;
  ER	err;
  INT	size;

  res_port = req->resport;
  printk ("io_request: command = %d\n", req->command);
  err = snd_mbf (device_table[device].portid, sizeof (T_IO_REQUEST), req);
  if (err != E_OK)		/* ��å����������˼��� */
    {
      return (err);
    }

  if (mode == IO_SYNC)	/* Ʊ���⡼�ɤλ����쥹�ݥ󥹤��֤�Τ��Ԥ� */
    {
      err = rcv_mbf (&req, &size, res_port);
      if (err != E_OK)
	{
	  return (err);
	}
      if (size != sizeof (T_IO_RESPONSE))
	{
	  return (E_OBJ);
	}
    }

  return (E_OK);
}


/*************************************************************************
 * io_response ---	��Ʊ���⡼�ɤΥǥХ��������ϤΤȤ��ˣɣϤΥ쥹
 *			�ݥ󥹤�����Ȥ�
 *
 */
ER
io_response (ID device, T_IO_REQUEST *req, T_IO_RESPONSE *res)
{
  ER	err;
  INT	size;

  err = rcv_mbf (&req, &size, req->resport);
  if (err != E_OK)
    {
      return (err);
    }
  if (size != sizeof (T_IO_RESPONSE))
    {
      return (E_OBJ);
    }
  return (E_OK);
}
