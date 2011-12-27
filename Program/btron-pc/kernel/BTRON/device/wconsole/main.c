/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/wconsole/main.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/wconsole/main.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: main.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.6  2000-04-03 14:30:42  naniwa
 * to call timer handler in task
 *
 * Revision 1.5  2000/02/27 15:29:24  naniwa
 * to work as multi task OS
 *
 * Revision 1.4  2000/02/20 09:34:15  naniwa
 * to correspond to writing from device
 *
 * Revision 1.3  2000/02/19 03:00:11  naniwa
 * to support multi window environment
 *
 * Revision 1.2  2000/01/29 16:16:14  naniwa
 * to work with POSIX fcntl
 *
 * Revision 1.1  1999/05/15 09:56:04  naniwa
 * first version
 *
 */

/**************************************************************************
 Discription

**********************************************************************/

#include "wconsole_internal.h"
#include "../servers/port-manager.h"

extern void draw_window();

/*********************************************************************
 *	 �ɽ��ѿ��������
 *
 */
static void	main_loop ();
static void	doit (DDEV_REQ *packet);
static void	writes (B *s, W winid);

static REQ_LIST req_list[MAX_REQ_LIST], *free_req_list = NULL, *cmsg = NULL;

/*********************************************************************
 *	 ����ѿ��������
 *
 */
ID	recvport;
W	initialized = 0;
W	video_mode = TEXT_MODE;
ID	recv, console, keyboard;
#ifdef notdef
ID	recv_k;
#endif

extern int focus, max_win;
extern struct window w[];


/*
 * wconsole �ǥХ����ɥ饤�Ф� main �ؿ�
 *
 * ���δؿ��ϡ��ǥХ����ɥ饤��Ω���夲���˰������¹Ԥ��롣
 *
 */
start ()
{
  extern char	version[];

  /* 
   * �׵�����ѤΥݡ��Ȥκ���
   */
  init_wconsole ();

  /*
   * Ω���夲��å�����
   */
  writes ("wconsole driver start\n", 0);

  main_loop ();
}

static void
main_loop ()
{
  DDEV_REQ	req;	/* ���������׵�ѥ��å� */
  DDEV_RES	res;
  ER		err;
  ER		errno;
  INT		rsize;

  /*
   * �׵���� - �����Υ롼��
   */
  for (;;) {
    rsize = sizeof (req);
#ifdef notdef
    dbg_printf ("WCONSOLE: call rcv_mbf on recvport\n");
#endif
    errno = rcv_mbf (&req, &rsize, recvport);
    switch (errno) {
    case E_OK:
      /* ���辰���� */
#ifdef notdef
      dbg_printf ("WCONSOLE: receive packet type = %d\n", req.header.msgtyp);
#endif
      doit (&req);
      break;
    case E_TMOUT:
      break;
    default:
      /* Unknown error */
#ifdef DEBUG
      dbg_printf ("WCONSOLE: get_req() Unknown error(error = %d)\n", err);
#endif
      break;
    }

#if 0      
    /* keyboard ������ֻ��ν��� */
    if (cmsg != NULL) {
      rsize = sizeof (res);
      errno = prcv_mbf (&res, &rsize, recv_k);
      if (errno == E_OK) {
	struct posix_response pres;

#ifdef notdef
	dbg_printf ("\nWCONSOLE: recv from keyboard\n");
#endif
	/* POSIX ���ץꥱ������� ���ֻ���ȯ�� */
	vput_reg(cmsg->msg.header.tskid,
		 cmsg->msg.body.prd_req.buf,
		 cmsg->msg.body.prd_req.length,
		 res.body.rea_res.dt);

	pres.receive_port = 0;
	pres.msg_length = sizeof (pres);
	pres.operation = PSC_READ;
	pres.errno = EP_OK;
	pres.status = cmsg->msg.body.prd_req.length;
	pres.ret1 = 0;
	pres.ret2 = 0;
#ifdef notdef
	dbg_printf ("WCONSOLE: send to appli %d\n", cmsg->msg.header.mbfid);
#endif
	snd_mbf (cmsg->msg.header.mbfid, sizeof (pres), &pres);

	/* cmsg �ξõ� */
	cmsg->next = free_req_list;
	free_req_list = cmsg;
	cmsg = NULL;
      }
    }
#endif

    if ((cmsg == NULL) && (w[focus].list != NULL)) {
#ifdef notdef
      dbg_printf ("\nWCONSOLE: pick up new msg\n");
#endif
      /* ���Υ�å������γ��� */
      cmsg = w[focus].list;
      w[focus].list = cmsg->next;
      if (w[focus].list == cmsg) w[focus].list = NULL;
      cmsg->prev->next = cmsg->next;
      cmsg->next->prev = cmsg->prev;
	
      /* keyboard �ؤ� read ��ȯ�� */
      req.header.mbfid = recvport;
      req.header.msgtyp = DEV_RLY;
      req.body.rea_req.dd = focus;
      req.body.rea_req.start = cmsg->msg.body.prd_req.start;
      req.body.rea_req.size = cmsg->msg.body.prd_req.length;
#ifdef notdef
      dbg_printf ("WCONSOLE: snd_mbf to key again\n");
#endif
      errno = snd_mbf (keyboard, sizeof (req), &req);
      
      /* �����˼��Ԥ������ */
      if (errno != E_OK) {
	dbg_printf("WCONSOLE: cant send message to keyboard\n");
      }
    }
  }

  /* �����ιԤˤϡ���ʤ� */
}


/*
 * �����
 *
 * o �׵�����Ĥ��ѤΥ�å������Хåե� ID ��ݡ��ȥޥ͡��������Ͽ
 */

static void init_req_list()
{
  int i;

  for(i = 0; i < MAX_REQ_LIST - 1; ++i) {
    req_list[i].next = &req_list[i+1];
  }
  req_list[MAX_REQ_LIST-1].next = NULL;
  free_req_list = &req_list[0];
}

W
init_wconsole (void)
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
      dbg_printf ("WCONSOLE: cannot make receive port.\n");
      slp_tsk ();
      /* ��å������Хåե������˼��� */
    }

  error = regist_port (WCONSOLE_DRIVER, recvport);
  if (error != E_OK)
    {
      dbg_printf ("WCONSOLE: cannot regist port (error = %d)\n", error);
    }
  initialized = 1;
  dbg_printf ("WCONSOLE: regist port %d\n", recvport);

  recv = get_port (sizeof (DDEV_RES), sizeof (DDEV_RES));
  if (recv <= 0) {
    dbg_printf ("WCONSOLE: cannot make receive port for console.\n");
    slp_tsk ();
  }
#ifdef notdef
  recv_k = get_port (sizeof (DDEV_RES), sizeof (DDEV_RES));
  if (recv_k <= 0) {
    dbg_printf ("WCONSOLE: cannot make receive port for keyboard.\n");
    slp_tsk ();
  }
#endif
  if (find_port (CONSOLE_DRIVER, &console) != E_PORT_OK)    {
    dbg_printf ("WCONSOLE: cannot find port for %s.\n", CONSOLE_DRIVER);
    slp_tsk ();
  }
  if (find_port (KEYBOARD_DRIVER, &keyboard) != E_PORT_OK)    {
    dbg_printf ("WCONSOLE: cannot find port for %s.\n", KEYBOARD_DRIVER);
    slp_tsk ();
  }

  if (vget_csl (&x, &y) == E_OK) {
    set_curpos (x, y, 0);
  }
  else {
    clear_wconsole (0);
  }

  /* request list �ν���� */
  init_req_list();
#ifdef USE_MALLOC
  init_malloc (0xC0000000);	/* Ŭ������ */
#endif

#ifdef notdef
  if (vset_cns (recvport) != E_OK)
    {
      /* �����ͥ�˥��󥽡����ѹ���¹Ԥ����뤳�Ȥ�
       * ���Ԥ�����
       */
    }
#endif
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
	  init_wconsole ();
	}
      open_wconsole (packet->header.mbfid, &(packet->body.opn_req));
      break;

    case DEV_CLS:
      /* �ǥХ����Υ����� */
      close_wconsole (packet->header.mbfid, &(packet->body.cls_req));
      break;

    case DEV_REA:
      read_wconsole (packet->header.mbfid, &(packet->body.rea_req));
      break;

    case DEV_PRD:
      posix_read_wconsole (packet->header.mbfid, packet->header.tskid,
			   packet);
      break;

    case DEV_WRI:
      write_wconsole (packet->header.mbfid, &(packet->body.wri_req));
      break;

    case DEV_CTL:
      control_wconsole (packet->header.mbfid, &(packet->body.ctl_req));
      break;

    case DEV_RLY:
      relay_wconsole (packet->header.mbfid, &(packet->body.rly_req));
      break;
    }
}

/************************************************************************
 * open_wconsole --- wconsole �Υ����ץ�
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
open_wconsole (ID caller, DDEV_OPN_REQ *packet)
{
  DDEV_RES	res;

  res.body.opn_res.dd = packet->dd;
  res.body.opn_res.errcd = E_OK;
  res.body.opn_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
  return (E_OK);
}

/************************************************************************
 * close_wconsole --- �ɥ饤�ФΥ�����
 *
 * ������	dd	wconsole �ɥ饤���ֹ�
 *		o_mode	�����ץ�⡼��
 *		error	���顼�ֹ�
 *
 * ���͡�	��� E_OK ���֤���
 *
 * ������	���󥽡���ϥ������ν����ǤϤʤˤ⤷�ʤ���
 *
 */
W
close_wconsole (ID caller, DDEV_CLS_REQ *packet)
{
  DDEV_RES	res;

  res.body.cls_res.dd = packet->dd;
  res.body.cls_res.errcd = E_OK;
  res.body.cls_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
  return (E_OK);
}

/*************************************************************************
 * read_wconsole --- 
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
read_wconsole (ID caller, DDEV_REA_REQ *packet)
{
  DDEV_RES	res;

  res.body.rea_res.dd = packet->dd;
  res.body.rea_res.errcd = E_NOSPT;
  res.body.rea_res.errinfo = E_NOSPT;
  snd_mbf (caller, sizeof (res), &res);
  return (E_NOSPT);
}

/*************************************************************************
 * posix_read_wconsole --- 
 *
 * ������	caller
 *	tskid,	packet
 *
 * ���͡�
 *
 * ������ keyboard �ؤ����Ϥ��꼡����
 *	
 *	
 *
 */
W posix_read_wconsole (ID caller, ID tskid, DDEV_REQ *packet)
{
  DDEV_RES	res;
  DDEV_REQ	req;
  W rsize, errno, win;
  REQ_LIST *list, *new;


  win = packet->body.prd_req.dd;
  if (cmsg != NULL || (win != focus)) {
#ifdef notdef
    dbg_printf ("\nWCONSOLE: regist request\n");
#endif
    /* packet �� win �� queue ����Ͽ */
    list = w[win].list;
    new = free_req_list;
    if (new == NULL) {
      dbg_printf("WCONSOLE: emtpy request list\n");
      return(E_NOMEM);
    }
    free_req_list = new->next;
    bcopy(packet, &(new->msg), sizeof(DDEV_REQ));

    if (list == NULL) {
      w[win].list = new;
      new->next = new;
      new->prev = new;
    }
    else {
      new->next = list;
      new->prev = list->prev;
      list->prev->next = new;
      list->prev = new;
    }
  }
  else {
#ifdef notdef
    dbg_printf ("\nWCONSOLE: call keyboard\n");
#endif
    /* cmsg �� packet ����Ͽ */
    cmsg = free_req_list;
    if (cmsg == NULL) {
      dbg_printf("WCONSOLE: emtpy request list\n");
      return(E_NOMEM);
    }
    free_req_list = cmsg->next;
    cmsg->next = NULL;
    bcopy(packet, &(cmsg->msg), sizeof(DDEV_REQ));

    /* keyboard �� read ��ȯ�� */
    req.header.mbfid = recvport;
    req.header.msgtyp = DEV_RLY;
    req.body.rea_req.dd = focus;
    req.body.rea_req.start = packet->body.prd_req.start;
    req.body.rea_req.size = packet->body.prd_req.length;
    errno = snd_mbf (keyboard, sizeof (req), &req);
    
    /* �����˼��Ԥ������ */
    if (errno != E_OK) {
      dbg_printf("WCONSOLE: cant send message to keyboard\n");
    }
    
    return (errno);
  }
  return(E_OK);
}

/************************************************************************
 * write_wconsole
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
write_wconsole(ID caller, DDEV_WRI_REQ *packet)
{
  DDEV_RES	res;
  int		i;
  ER		error;
  static int	esc_flag = 0, cnum = 0;
  W		cpos[2];
  UB		ch;
  DDEV_REQ		req;		/* �׵�ѥ��å� */
  W  rsize, winid;

  if (video_mode == TEXT_MODE) {
    req.header.mbfid = recv;
    req.header.msgtyp = DEV_WRI;
    bcopy(packet, &(req.body.ctl_req), sizeof (req.body));
    error = snd_mbf (console, sizeof (req), &req);
    if (error != E_OK) {
      res.body.ctl_res.dd = packet->dd;
      res.body.ctl_res.errcd = error;
      res.body.ctl_res.errinfo = error;
      snd_mbf (caller, sizeof (res), &res);
      return (error);
    }
    rsize = sizeof (res);
#ifdef notdef
    dbg_printf ("WCONSOLE: call rcv_mbf on recv 1\n");
#endif
    error = rcv_mbf (&res, (INT *)&rsize, recv);
    
    res.body.ctl_res.dd = packet->dd;
    res.body.ctl_res.errcd = E_OK;
    res.body.ctl_res.errinfo = E_OK;
    snd_mbf (caller, sizeof (res), &res);
    return (E_OK);
  }
  winid = packet->dd; /* minor device �ֹ�� window ����� */
  if (winid >= max_win || winid < 0) winid = 0;
  if (winid == 0) winid = focus; /* default console �ΰ����Ϥ���ä��ü� */
  error = E_OK;
  if (w[winid].map) {
    for (i = 0; i < (packet->size); i++) {
      erase_cursor(winid);
      ch = packet->dt[i];
      if (esc_flag == 1) {
	if (ch == '[') {
	  esc_flag = 2;
	}
	else {
	  error = write_char (ch, winid);
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
	    set_curpos(cpos[1]-1, cpos[0]-1, winid);
	    esc_flag = 0;
	    break;
	  case 'A':
	    move_curpos(0, -cpos[0], winid);
	    esc_flag = 0;
	    break;
	  case 'B':
	    move_curpos(0, cpos[0], winid);
	    esc_flag = 0;
	    break;
	  case 'C':
	    move_curpos(cpos[0], 0, winid);
	    esc_flag = 0;
	    break;
	  case 'D':
	    move_curpos(-cpos[0], 0, winid);
	    esc_flag = 0;
	    break;
	  case 'J':
	    if (cpos[0] == 2) {
	      set_curpos(0, 0, winid);
	      clear_wconsole(winid);
	    }
	    else if (cpos[0] == 1) {
	      clear_rest_screen(winid);
	    }
	    esc_flag = 0;
	    break;
	  case 'K':
	    clear_rest_line(winid);
	    esc_flag = 0;
	    break;
	  default:
	    esc_flag = 0;
	    error = write_char(ch, winid);
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
	  error = write_char (ch, winid);
	}
      }
      if (error != E_OK)
	break;
    }
  }
  res.body.wri_res.dd = packet->dd;
  res.body.wri_res.errcd = error;
  res.body.wri_res.errinfo = error;
  res.body.wri_res.a_size = i;
  snd_mbf (caller, sizeof (res), &res);
  return (error);
}

/************************************************************************
 * control_wconsole
 *
 * ������	caller
 *		packet
 *
 * ���͡�	
 *
 * ���������̤Υ⡼�ɤ��ѹ�������ե��å��������衢������ɥ������ʤ�
 *
 */
W
control_wconsole (ID caller, DDEV_CTL_REQ *packet)
{
  DDEV_REQ		req;		/* �׵�ѥ��å� */
  DDEV_RES	res;
  ER		errno;
  W			rsize;
  W x, y;
  union wc_cmd_t *p;
  int redraw;

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
      req.header.mbfid = recv;
      req.header.msgtyp = DEV_CTL;
      bcopy(packet, &(req.body.ctl_req), sizeof (req.body));
      errno = snd_mbf (console, sizeof (req), &req);
      /*      errno = graphic_command (caller, packet);*/
      if (errno != E_OK) {
	  res.body.ctl_res.dd = packet->dd;
	  res.body.ctl_res.errcd = errno;
	  res.body.ctl_res.errinfo = errno;
	  snd_mbf (caller, sizeof (res), &res);
	  return (errno);
      }
      rsize = sizeof (res);
#ifdef notdef
    dbg_printf ("WCONSOLE: call rcv_mbf on recv 2\n");
#endif
      errno = rcv_mbf (&res, (INT *)&rsize, recv);
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
#if 0
  else if ((packet->cmd & WC_NULL) != 0) {
#else
  else if (((packet->cmd & WC_NULL) != 0) && (video_mode == GRAPHIC_MODE)) {
#endif
    switch (packet->cmd) {
    case WC_WINMOD:
      redraw = 0;
      p = (union wc_cmd_t *)packet->param;
      if (p->winmod.mode == 0) {
	if (focus != 0) redraw = 1;
	focus = 0;
	w[0].map = 1;
	w[1].map = 0;
	w[2].map = 0;
      }
      else if (p->winmod.mode == 1) {
	if (focus == 0)  redraw = 1;
	else erase_cursor(focus);
	focus = 1;
	w[0].map = 0;
	w[1].map = 1;
	w[2].map = 1;
      }
      else if (p->winmod.mode == 2) {
	if (focus == 0)  redraw = 1;
	else erase_cursor(focus);
	focus = 2;
	w[0].map = 0;
	w[1].map = 1;
	w[2].map = 1;
      }
      if (redraw) {
	fillbox_vga(0, 0, 640, 480, 0);
	draw_window();
      }
      res.body.ctl_res.dd = packet->dd;
      res.body.ctl_res.errcd = E_OK;
      res.body.ctl_res.errinfo = E_OK;
      snd_mbf (caller, sizeof (res), &res);
      return (E_OK);
    case WC_CHGFCS:
      {
	int i;
	i = (focus+1) % max_win;
	while(focus != i) {
	  if (w[i].map) break;
	  i = (i+1) % max_win;
	}
	if (i != focus) {
	  erase_cursor(focus);
	  focus = i;
	  move_curpos (0, 0, focus);
	}
      }
#if 0
      dbg_printf("WC_CHGFCS to %d\n", focus);
#endif
      res.body.ctl_res.dd = packet->dd;
      res.body.ctl_res.errcd = E_OK;
      res.body.ctl_res.errinfo = E_OK;
#ifdef notdef
      dbg_printf("send reply\n");
#endif
      snd_mbf (caller, sizeof (res), &res);
      return (E_OK);
    }
    res.body.ctl_res.dd = packet->dd;
    res.body.ctl_res.errcd = E_NOSPT;
    res.body.ctl_res.errinfo = E_NOSPT;
    snd_mbf (caller, sizeof (res), &res);
    return (E_NOSPT);
  }
  else
    {
      switch (packet->cmd)
	{
	case CONSOLE_CLEAR:
	  clear_wconsole (0);
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
	  set_curpos (packet->param[0], packet->param[1], 0);
	  res.body.ctl_res.dd = packet->dd;
	  res.body.ctl_res.errcd = E_OK;
	  res.body.ctl_res.errinfo = E_OK;
	  snd_mbf (caller, sizeof (res), &res);
	  return (E_OK);

	case CONSOLE_GRAPHIC:
	  if (video_mode == TEXT_MODE)
	    {
#ifdef VGA
	      req.header.mbfid = recv;
	      req.header.msgtyp = DEV_CTL;
	      req.body.ctl_req.cmd = CONSOLE_GRAPHIC;
	      req.body.ctl_req.len = 0;
	      errno = snd_mbf (console, sizeof (req), &req);
	      /*      errno = graphic_command (caller, packet);*/
	      if (errno != E_OK) {
		res.body.ctl_res.dd = packet->dd;
		res.body.ctl_res.errcd = errno;
		res.body.ctl_res.errinfo = errno;
		snd_mbf (caller, sizeof (res), &res);
		return (errno);
	      }
	      rsize = sizeof (res);
#ifdef notdef
    dbg_printf ("WCONSOLE: call rcv_mbf on recv 3\n");
#endif
	      errno = rcv_mbf (&res, (INT *)&rsize, recv);

	      res.body.ctl_res.dd = packet->dd;
	      res.body.ctl_res.errcd = E_OK;
	      res.body.ctl_res.errinfo = E_OK;
	      video_mode = GRAPHIC_MODE;
	      draw_window();
	      set_curpos (0, 0, focus);
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
	      req.header.mbfid = recv;
	      req.header.msgtyp = DEV_CTL;
	      req.body.ctl_req.cmd = CONSOLE_TEXT;
	      req.body.ctl_req.len = 0;
	      errno = snd_mbf (console, sizeof (req), &req);
	      if (errno != E_OK) {
		res.body.ctl_res.dd = packet->dd;
		res.body.ctl_res.errcd = errno;
		res.body.ctl_res.errinfo = errno;
		snd_mbf (caller, sizeof (res), &res);
		return (errno);
	      }
	      rsize = sizeof (res);
#ifdef notdef
    dbg_printf ("WCONSOLE: call rcv_mbf on recv 4\n");
#endif
	      errno = rcv_mbf (&res, (INT *)&rsize, recv);

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
	      if (vget_csl (&x, &y) == E_OK) {
		set_curpos (x, y, 0);
	      }
	      else {
		clear_wconsole (0);
	      }
	      /* window ���֤ν���� */
	      focus = 0;
	      w[0].map = 1;
	      w[1].map = 0;
	      w[2].map = 0;
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
writes (B *s, W winid)
{
  ER error;

  while (*s != '\0')
    {
      error = write_char (*s, winid);
      if (error != E_OK)
	break;
      s++;
    }
}

/* relay_wconsoe()
 */
W relay_wconsole(ID caller, DDEV_RLY_REQ *packet)
{
  struct posix_response pres;
  DDEV_REQ req;
  ER errno;

  /* keyboard ������ֻ��ν��� */
  if (cmsg != NULL) {
    /* POSIX ���ץꥱ������� ���ֻ���ȯ�� */
    vput_reg(cmsg->msg.header.tskid,
	     cmsg->msg.body.prd_req.buf,
	     cmsg->msg.body.prd_req.length,
	     packet->dt);

    pres.receive_port = 0;
    pres.msg_length = sizeof (pres);
    pres.operation = PSC_READ;
    pres.errno = EP_OK;
    pres.status = cmsg->msg.body.prd_req.length;
    pres.ret1 = 0;
    pres.ret2 = 0;
#ifdef notdef
    dbg_printf ("WCONSOLE: send to appli %d\n", cmsg->msg.header.mbfid);
#endif
    snd_mbf (cmsg->msg.header.mbfid, sizeof (pres), &pres);

    /* cmsg �ξõ� */
    cmsg->next = free_req_list;
    free_req_list = cmsg;
    cmsg = NULL;
  }
}
