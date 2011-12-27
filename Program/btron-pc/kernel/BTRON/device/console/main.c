/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/console/main.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/console/main.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";
static char revision[] = "$Revision: 1.1 $";

/*
 * $Log: main.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.10  2000-07-09 16:44:47  kishida0
 * fix some warning (sample for other devices)
 *
 * Revision 1.9  2000/02/07 18:04:35  kishida0
 * minor fix
 *
 * Revision 1.8  2000/01/29 16:15:12  naniwa
 * to work with POSIX fcntl
 *
 * Revision 1.7  1999/03/21 12:49:23  night
 * ϲ��(naniwa@mechgw.mech.yamaguchi-u.ac.jp) ����λ�Ŧ�ˤ���ѹ���
 * ------------------
 * ���������ס��������󥹤�������Ƥ�������˵����դ�����Ǥ�����
 * console �ϴ��˥Хå����ڡ����ˤ��б�����Ƥ�����Ǥ��� (^^;)��frtm
 * �����Ϥˤ� '\b' ���Ȥ���н�ʬ�ʤΤǡ����������ץ������󥹤���ɬ
 * �פ�̵���ʤäƤ��ޤä��ΤǤ������޳ѤʤΤǺǸ�ޤǼ������ޤ�����
 *
 * �����������������ס��������󥹤λ��ͤǤ������ mail �Υߥ���������
 * �ޤ�����
 *
 * ESC[Pl;PcH	��������� (Pc,Pl) �ذ�ư���ǥե���ȤϤ��줾�� 1��
 * ESC[PnA		��������� Pn �Ծ�ذ�ư���ǥե���Ȥ� 1��
 * 		���̾�ü����ؤΰ�ư��̵�롥
 * ESC[PnB		��������� Pn �Ծ�ذ�ư���ǥե���Ȥ� 1��
 * 		���̲�ü��겼�ؤΰ�ư��̵�롥
 * ESC[PnC		��������� Pn �屦�ذ�ư���ǥե���Ȥ� 1��
 * 		���̱�ü��걦�ؤΰ�ư��̵�롥
 * ESC[PnD		��������� Pn �庸�ذ�ư���ǥե���Ȥ� 1��
 * 		���̱�ü��꺸�ؤΰ�ư��̵�롥
 * ESC[2J		���̤�õ����������� (1,1) �ء�
 * ESC[J		ɽ���ν���ޤǤ�õ
 * ESC[K		����������֤���Ԥν���ޤǤ�õ
 *
 * ��������κ�ɸ�Ϻ������ (1,1)�������� (MAX_COLUMN, MAX_LINE)��
 *
 * kernel/BTRON/device/console �β��� patch -p1 �Ǥ��Ƥ�褦�˥ѥå�
 * ����ޤ�����
 * ------------------
 *
 * Revision 1.6  1998/11/30 13:25:59  night
 * malloc �򤳤ΥǥХ����ɥ饤�Фǻ��Ѥ���Ȥ� (�ޥ��� USE_MALLOC �����
 * �����Ȥ�)��init_malloc () �� malloc �ǻ��Ѥ����ΰ����Ƭ���Ϥ���ꤹ��
 * �������ɲá�
 * kernlib ���������Ƥ��� malloc() �ϡ����Ѥ�����ä� init_malloc() ��
 * �¹Ԥ���malloc �ǻ��Ѥ����ΰ����Ƭ���Ϥ���ꤷ�ʤ���Фʤ�ʤ���
 *
 * Revision 1.5  1997/09/21 13:31:24  night
 * kernlib ����� malloc �饤�֥�����Ѥ��ʤ��褦�ˤ�����
 * (malloc �饤�֥���ȤäƤ��ޤ��ȡ�console �ɥ饤���Ѥ˳��ݤ��Ƥ����
 * ����ΰ�򥪡��С����Ƥ��ޤ�����)
 *
 * Revision 1.4  1997/08/31 14:00:18  night
 * VGA �����ط��ν������ɲá�
 *
 * Revision 1.3  1996/11/07  15:40:46  night
 * �����ͥ뤫��ν��Ϥ򥳥󥽡���ɥ饤�Фظ������������Ū��
 * ����᤿��
 * (�ϥ󥰥��åפ��Ƥ��ޤ�����)
 *
 * Revision 1.2  1996/11/06  13:54:17  night
 * �����ͥ뤫�鸽�ߤΥ���������֤��������������ɲá�
 * �����
 * �����ͥ�˥��󥽡���ɥ饤�Ф��׵�����դ��ݡ��Ȥ�
 * ��Ͽ����������ɲá�
 *
 * Revision 1.1  1996/07/24  16:03:59  night
 * �ǽ����Ͽ
 *
 * Revision 1.5  1996/01/02 16:20:30  night
 * Ω���夲���˲��̤򥯥ꥢ����褦���ѹ�������
 *
 * Revision 1.4  1995/09/21  15:50:55  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.3  1995/09/17  16:50:38  night
 * �������ѥ��åȤ������ src/kernel/kernlib/device.h �˹�碌����
 * open/close/read/write/control �ؿ������������
 *
 * Revision 1.2  1995/09/14  04:28:43  night
 * init_driver() �� init_console() ���ѹ���
 * ��å����������������ɲá�
 *
 * Revision 1.1  1995/09/12  18:18:11  night
 * �ǽ����Ͽ
 *
 *
 *
 */

/**************************************************************************
 Discription

**********************************************************************/

#include "console_internal.h"

/*********************************************************************
 *	 �ɽ��ѿ��������
 *
 */

void start(void);
static void	main_loop (void);
static void	doit (DDEV_REQ *packet);
static void	writes (B *s);


/*********************************************************************
 *	 ����ѿ��������
 *
 */
ID	recvport;
W	initialized = 0;
W	video_mode = TEXT_MODE;


/*
 * console �ǥХ����ɥ饤�Ф� main �ؿ�
 *
 * ���δؿ��ϡ��ǥХ����ɥ饤��Ω���夲���˰������¹Ԥ��롣
 *
 */
void start (void)
{
  char *p;
  /* 
   * �׵�����ѤΥݡ��Ȥκ���
   */
  init_console ();

  /*
   * Ω���夲��å�����
   */
  writes ("console driver start\n");
  p = rcsid;
  p = revision;
  main_loop ();
}

static void
main_loop (void)
{
  DDEV_REQ		req;	/* ���������׵�ѥ��å� */
  ER			err;
  extern ER		sys_errno;

  /*
   * �׵���� - �����Υ롼��
   */
  for (;;)
    {
      W	rsize;

      /* �׵�μ��� */
#ifdef DEBUG
      dbg_printf ("call get_req ()\n");
#endif
      rsize = sizeof (req);
      get_req (recvport, &req, &rsize);
      switch (sys_errno)
	{
	case E_OK:
	  /* ���辰���� */
#ifdef DEBUG
	  dbg_printf ("console: receive packet type = %d\n", req.header.msgtyp);
#endif
	  doit (&req);
	  break;

	default:
	  /* Unknown error */
#ifdef DEBUG
	  dbg_printf ("CONSOLE: get_req() Unknown error(error = %d)\n", err);
#endif
	  break;
	}
    }

  /* �����ιԤˤϡ���ʤ� */
}

/*
 * �����
 *
 * o �׵�����Ĥ��ѤΥ�å������Хåե� ID ��ݡ��ȥޥ͡��������Ͽ
 */
W
init_console (void)
{
  int		i;
  ER		error;
  W		x, y;

  /*
   * �׵�����Ĥ��ѤΥݡ��Ȥ��������롣
   */
  recvport = get_port (sizeof (DDEV_REQ), sizeof (DDEV_REQ));
  if (recvport <= 0)
    {
      dbg_printf ("CONSOLE: cannot make receive port.\n");
      slp_tsk ();
      /* ��å������Хåե������˼��� */
    }

  error = regist_port (CONSOLE_DRIVER, recvport);
  if (error != E_OK)
    {
      dbg_printf ("console: cannot regist port (error = %d)\n", error);
      return E_SYS;
    }
  initialized = 1;
  dbg_printf ("console: regist port %d\n", recvport);

  if (vget_csl (&x, &y) == E_OK)
    {
      set_curpos (x, y);
    }
  else
    {
      clear_console ();
    }

#ifdef USE_MALLOC
  init_malloc (0xC0000000);	/* Ŭ������ */
#endif

#ifdef notdef
  if (vset_cns (recvport) != E_OK)
    {
      /* �����ͥ�˥��󥽡����ѹ���¹Ԥ����뤳�Ȥ�
       * ���Ԥ�����
       */
      return E_SYS;
    }
#endif
  return E_OK;
}

/************************************************************************
 *
 *
 */
static void
doit (DDEV_REQ *packet)
{
  switch (packet->header.msgtyp)
    {
    case DEV_OPN:
      /* �ǥХ����Υ����ץ� */
      if (!initialized)
	{
	  init_console ();
	}
      open_console (packet->header.mbfid, &(packet->body.opn_req));
      break;

    case DEV_CLS:
      /* �ǥХ����Υ����� */
      close_console (packet->header.mbfid, &(packet->body.cls_req));
      break;

    case DEV_REA:
      read_console (packet->header.mbfid, &(packet->body.rea_req));
      break;

    case DEV_WRI:
      write_console (packet->header.mbfid, &(packet->body.wri_req));
      break;

    case DEV_CTL:
      control_console (packet->header.mbfid, &(packet->body.ctl_req));
      break;
    }
}

/************************************************************************
 * open_console --- console �Υ����ץ�
 *
 * ������	caller	��å������������
 *		packet	�����ץ�ѥ��å�
 *
 * ���͡�	��� E_OK ���֤���
 *
 * ������	E_OK ���å��������������֤���
 *
 */
W
open_console (ID caller, DDEV_OPN_REQ *packet)
{
  DDEV_RES	res;

  res.body.opn_res.dd = packet->dd;
  res.body.opn_res.errcd = E_OK;
  res.body.opn_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
  return (E_OK);
}

/************************************************************************
 * close_console --- �ɥ饤�ФΥ�����
 *
 * ������	dd	console �ɥ饤���ֹ�
 *		o_mode	�����ץ�⡼��
 *		error	���顼�ֹ�
 *
 * ���͡�	��� E_OK ���֤���
 *
 * ������	���󥽡���ϥ������ν����ǤϤʤˤ⤷�ʤ���
 *
 */
W
close_console (ID caller, DDEV_CLS_REQ *packet)
{
  DDEV_RES	res;

  res.body.cls_res.dd = packet->dd;
  res.body.cls_res.errcd = E_OK;
  res.body.cls_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
  return (E_OK);
}

/*************************************************************************
 * read_console --- 
 *
 * ������	caller
 *		packet
 *
 * ���͡�	E_NOSPT ���֤���
 *
 * ������	��å������������� E_NOSPT ���֤���
 *		���󥽡���ɥ饤�Ф�ɽ����������ʤΤ����Ϥϥ��ݡ��Ȥ���
 *		���ʤ���
 *
 */
W
read_console (ID caller, DDEV_REA_REQ *packet)
{
  DDEV_RES	res;

  res.body.rea_res.dd = packet->dd;
  res.body.rea_res.errcd = E_NOSPT;
  res.body.rea_res.errinfo = E_NOSPT;
  snd_mbf (caller, sizeof (res), &res);
  return (E_NOSPT);
}

/************************************************************************
 * write_console
 *
 * ������	caller
 *		packet
 *
 * ���͡�	
 *
 * ������	�����ȥݥ��󥿤ΰ��֤�ʸ�����ɽ�����롣
 *		���ΤȤ������������ץ������󥹤ˤ�������Ԥ���		
 *
 */
W
write_console(ID caller, DDEV_WRI_REQ *packet)
{
  DDEV_RES	res;
  int		i;
  ER		error;
  static int	esc_flag = 0, cnum = 0;
  W		cpos[2];
  UB		ch;

  error = E_OK;
  for (i = 0; i < (packet->size); i++)
    {
      ch = packet->dt[i];
      if (esc_flag == 1) {
	if (ch == '[') {
	  esc_flag = 2;
	}
	else {
	  error = write_char (ch);
	  esc_flag = 0;
	}
      }
      else if (esc_flag == 2) {
	if (ISDIGIT(ch)) {
	  cpos[cnum] *= 10;
	  cpos[cnum] += ch-'0';
	}
	else {
	  if (cpos[cnum] == 0) cpos[cnum] = 1;
	  switch(ch) {
	  case ';':
	    cnum++;
	    if (cnum == 2) esc_flag = 0;
	    break;
	  case 'H':
	    if (cpos[1] == 0) cpos[1] = 1;
	    set_curpos(cpos[1]-1, cpos[0]-1);
	    esc_flag = 0;
	    break;
	  case 'A':
	    move_curpos(0, -cpos[0]);
	    esc_flag = 0;
	    break;
	  case 'B':
	    move_curpos(0, cpos[0]);
	    esc_flag = 0;
	    break;
	  case 'C':
	    move_curpos(cpos[0], 0);
	    esc_flag = 0;
	    break;
	  case 'D':
	    move_curpos(-cpos[0], 0);
	    esc_flag = 0;
	    break;
	  case 'J':
	    if (cpos[0] == 2) {
	      set_curpos(0, 0);
	      clear_console();
	    }
	    else if (cpos[0] == 1) {
	      clear_rest_screen();
	    }
	    esc_flag = 0;
	    break;
	  case 'K':
	    clear_rest_line();
	    esc_flag = 0;
	    break;
	  default:
	    esc_flag = 0;
	    error = write_char(ch);
	  }
	}
      }
      else {
	if (ch == 0x1B) {
	  esc_flag = 1;
	  cpos[0] = 0; cpos[1] = 0;
	  cnum = 0;
	}
	else {
	  error = write_char (ch);
	}
      }
      if (error != E_OK)
	break;
    }  
  res.body.wri_res.dd = packet->dd;
  res.body.wri_res.errcd = error;
  res.body.wri_res.errinfo = error;
  res.body.wri_res.a_size = i;
  snd_mbf (caller, sizeof (res), &res);
  return (error);
}

/************************************************************************
 * control_console
 *
 * ������	caller
 *		packet
 *
 * ���͡�	E_NOSPT ���֤���
 *
 * ������
 *
 */
W
control_console (ID caller, DDEV_CTL_REQ *packet)
{
  DDEV_RES	res;
  ER		errno;

  if ((packet->cmd & GR_NULL) != 0)
    {
      if (video_mode != GRAPHIC_MODE)
	{
	  res.body.ctl_res.dd = packet->dd;
	  res.body.ctl_res.errcd = E_PAR;
	  res.body.ctl_res.errinfo = E_PAR;
	  snd_mbf (caller, sizeof (res), &res);
	  return (E_PAR);
	}

#ifdef VGA
      errno = graphic_command (caller, packet);
      res.body.ctl_res.dd = packet->dd;
      res.body.ctl_res.errcd = errno;
      res.body.ctl_res.errinfo = errno;
#else
      res.body.ctl_res.dd = packet->dd;
      res.body.ctl_res.errcd = E_NOSPT;
      res.body.ctl_res.errinfo = E_NOSPT;
#endif
      snd_mbf (caller, sizeof (res), &res);
      return (errno);
    }
  else
    {
      switch (packet->cmd)
	{
	case CONSOLE_CLEAR:
	  clear_console ();
	  res.body.ctl_res.dd = packet->dd;
	  res.body.ctl_res.errcd = E_OK;
	  res.body.ctl_res.errinfo = E_OK;
	  snd_mbf (caller, sizeof (res), &res);
	  return (E_OK);

	case CONSOLE_MOVE:
	  if (packet->len != 2)
	    {
	      res.body.ctl_res.dd = packet->dd;
	      res.body.ctl_res.errcd = E_PAR;
	      res.body.ctl_res.errinfo = E_PAR;
	      snd_mbf (caller, sizeof (res), &res);
	      return (E_PAR);
	    }
	  set_curpos (packet->param[0], packet->param[1]);
	  res.body.ctl_res.dd = packet->dd;
	  res.body.ctl_res.errcd = E_OK;
	  res.body.ctl_res.errinfo = E_OK;
	  snd_mbf (caller, sizeof (res), &res);
	  return (E_OK);

	case CONSOLE_GRAPHIC:
	  if (video_mode == TEXT_MODE)
	    {
#ifdef VGA
	      vga_graph ();
	      res.body.ctl_res.dd = packet->dd;
	      res.body.ctl_res.errcd = E_OK;
	      res.body.ctl_res.errinfo = E_OK;
	      video_mode = GRAPHIC_MODE;
#else
	      res.body.ctl_res.dd = packet->dd;
	      res.body.ctl_res.errcd = E_NOSPT;
	      res.body.ctl_res.errinfo = E_NOSPT;
#endif
	      snd_mbf (caller, sizeof (res), &res);
	      return (E_OK);
	    }
	  else
	    {
	      res.body.ctl_res.dd = packet->dd;
	      res.body.ctl_res.errcd = E_PAR;
	      res.body.ctl_res.errinfo = E_PAR;
	      snd_mbf (caller, sizeof (res), &res);
	      return (E_PAR);
	    }
	  
	case CONSOLE_TEXT:
	  if (video_mode == GRAPHIC_MODE)
	    {
#ifdef VGA
	      vga_text ();
	      res.body.ctl_res.dd = packet->dd;
	      res.body.ctl_res.errcd = E_OK;
	      res.body.ctl_res.errinfo = E_OK;
	      video_mode = TEXT_MODE;
#else
	      res.body.ctl_res.dd = packet->dd;
	      res.body.ctl_res.errcd = E_NOSPT;
	      res.body.ctl_res.errinfo = E_NOSPT;
#endif
	      snd_mbf (caller, sizeof (res), &res);
	      return (E_OK);
	    }
	  else
	    {
	      res.body.ctl_res.dd = packet->dd;
	      res.body.ctl_res.errcd = E_PAR;
	      res.body.ctl_res.errinfo = E_PAR;
	      snd_mbf (caller, sizeof (res), &res);
	      return (E_PAR);
	    }
	  
	default:
	  res.body.ctl_res.dd = packet->dd;
	  res.body.ctl_res.errcd = E_NOSPT;
	  res.body.ctl_res.errinfo = E_NOSPT;
	  snd_mbf (caller, sizeof (res), &res);
	  return (E_NOSPT);
	}
    }
}


static void
writes (B *s)
{
  ER error;

  while (*s != '\0')
    {
      error = write_char (*s);
      if (error != E_OK)
	break;
      s++;
    }  
}
