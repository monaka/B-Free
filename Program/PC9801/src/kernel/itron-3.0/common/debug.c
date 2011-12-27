/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* debug.c --- デバッグ用コマンドラインインタプリンタ
 *
 * $Revision: 1.1 $
 *
 *	このファイルには、デバッグ用のコマンドラインインタプリタモジュールが入る。
 *
 * 	コマンドラインインタプリンタは、いわゆる shell と呼ばれる UNIX 上のユーテ
 *	ィリティと同じような動作をする。
 *	ただし、現時点では、外部コマンドは使用できない(使用できるコマンドは、すべ
 *	てこのモジュールの中で定義する)。
 *
 */

static char rcsid[] = "$Id: debug.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

/*
 * $Log: debug.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.20  1995-10-10 16:23:22  night
 * SCSI デバイステスト用のコマンド "scsi" と、init プログラム実行用のコマ
 * ンド "init" を追加。
 *
 * Revision 1.19  1995/10/03  14:09:54  night
 * FD ドライバのテスト用コマンドを追加。
 * 1) fdwrite： 任意のオフセットから 512 バイト分に 0xbb を 後半 512 バイト
 *              に 0xcc を書き込む。
 * 2) fdmod：   フロッピィディスクのモードを変更する。
 *              o 2HD ...... 1.2MB (1024byte/sector)
 *              o 2HC ...... 1.2MB (512byte/sector)
 *
 * Revision 1.18  1995/10/01  12:56:15  night
 * フロッピィデバイスドライバのテスト関数を追加。
 *
 * Revision 1.17  1995/09/21  15:51:07  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.16  1995/09/20  15:33:36  night
 * f_console_test () の中を修正。
 * 出力する文字列の長さを、正しく直した。
 *
 *
 */

#include "itron.h"
#include "version.h"
#include "errno.h"
#include "task.h"
#include "func.h"
#include "misc.h"
#include "ctype.h"
#include "graphics.h"
#include "../io/io.h"
#include "../../../boot/2nd/boot.h"
#include "../../../boot/2nd/config.h"
#include "../../servers/port-manager.h"
#include "../../kernlib/device.h"


#define MAXLINE		100
#define MAXWORDS	10

#define PROMPT	"debug> "

static void	write_line (B *line, W size);
static W	read_line (B *line, W size);
static W	parse_line (B *line, B **av, W size);

extern void	f_version ();
extern void	f_help ();
extern void	f_dump ();
extern void	f_ps ();
extern void	f_status ();
extern void	f_free ();
extern void	f_suspend ();
extern void	f_wakeup ();
extern void	f_cpureset ();
extern void	f_ioecho ();
extern void	f_gdc_test ();
extern void	f_demo ();
extern void	f_syscall ();
extern void	f_bootinfo ();
extern void	f_run_module ();
extern void	f_console_test ();
extern void	f_keyboard_test ();
extern void	f_fd_test ();
extern void	f_fd_chmod ();
extern void	f_fd_write ();
extern void	f_scsi ();
extern void	f_init ();

typedef struct buildin_command_t
{
  B	*name;
  FP	func;
  INT	minarg;
  INT	maxarg;
} T_COMMAND;

T_COMMAND	command_list[] =
{
  "version",	f_version,	1,	1,	/* バージョン番号の表示		*/
  "help",	f_help,		1,	1,	/* ヘルプメッセージの表示	*/
  "dump",	f_dump,		2,	3,	/* メモリダンプ			*/
  "ps",		f_ps,		1,	1,	/* プロセスリスト		*/
  "status",	f_status,	-1,	-1,	/* ステータス                   */
  "free",	f_free,		1,      2,      /* 空き資源の状態の表示         */
  "suspend",	f_suspend,	2,	2,	/* 指定したタスクのサスペンド   */
  "wakeup",	f_wakeup,	2,	2,	/* サスペンドしたタスクの wakeup*/
  "reset",	f_cpureset,	1,	1,	/* ＣＰＵのリセット		*/
  "ioecho",	f_ioecho,	2, 	2,	/* echo デバイスドライバにwriteメッセージを送る */
  "gdc",	f_gdc_test,	1,	1,	/* GDCのテスト用		*/
  "demo",	f_demo,		1,	1,	/* デモ用関数			*/
  "syscall",	f_syscall,	1,	1,	/* システムコールのテスト用 	*/
  "bootinfo",	f_bootinfo,	1,	1,	/* ブート情報の表示             */
  "run-module",	f_run_module,	2,	2,	/* ブート時にロードし */
						/* たモジュールの実行 */
  "console", 	f_console_test,	1,	1,	/* */
  "keyboard", 	f_keyboard_test,	1,	1,	/* */
  "fd",		f_fd_test,	1,	2,	/* FD のテスト */
  "fdwrite",	f_fd_write,	1,	2,	/* FD の write */
  "fdmod",	f_fd_chmod,	1,	2,	/* FD のモード切換え */
  "scsi",	f_scsi,		1,	2,	/* SCSI のテスト */
  "init",	f_init,		1,	1,	/* init */
  NULL,		NULL,		0,	0,
};
  

void
debugger (void)
{
  B	linebuf[MAXLINE];

  printf ("debugger: ver 1.0\n");
/*  init_debugger (); */
  for (;;)
    {
      printf ("%s", PROMPT);
      read_line (linebuf, sizeof (linebuf));
      if (strlen (linebuf) <= 0)
	continue;
/*      printf (">> %s\n", linebuf); */
      eval (linebuf);
    }
/* NOT REACHED */
}

/**************************************************************************************
 * eval
 *
 */
ER
eval (B *linebuf)
{
  W		i;
  T_COMMAND	*cmdp;
  B		*argv[MAXWORDS];
  W		argc;

  argc = parse_line (linebuf, argv, MAXWORDS);
  for (i = 0; command_list[i].name != NULL; i++)
    {
      if (strncmp (argv[0], command_list[i].name, strlen (command_list[i].name)) == 0)
	{
	  /* コマンドが見つかった */
	  if (((command_list[i].minarg == -1) || (argc >= command_list[i].minarg)) &&
	      ((command_list[i].maxarg == -1) || (argc <= command_list[i].maxarg)))
	    (command_list[i].func)(argc, argv);
	  else
	    printf ("argument mismatch.\n");
	  return (E_OK);
	}
    }	
  printf ("command not found\n");
  return (E_PAR);
}

static W
parse_line (B *line, B **av, W size)
{
  W	i;

  for (i = 0; i < size; i++, line++)
    {
      while (isspace(*line) && (*line != '\0'))
	line++;
      if (*line == '\0')
	break;
      av[i] = line;
      while ((!isspace(*line)) && (*line != '\0'))
	line++;
      *line = '\0';
    }
  return (i);
}



/***************************************************************************************
 *
 *  細々とした関数群
 *
 ***************************************************************************************/



static void
write_line (B *line, W size)
{
  while (size-- > 0)
    {
      putchar (*line);
    }
}

/*********************************************************************************
 * read_line --- 編集機能つきの1行入力関数
 *
 */
static W
read_line (B *line, W size)
{
  W	i;
  W	ch;

  bzero (line, size);
  for (i = 0; i < size - 1; i++)
    {
      ch = getchar ();
      if (ISCONTROL(ch))
	{
	  /* コントロールキーが入力されたとき */
	  if (ch == '\n')	
	    {
	      putchar (ch);
	      break;
	    }
	  else if (ch == 0x08)	/* CONTROL('H')) */
	    {
	      if (i > 0)
		{
		  line[i - 1] = '\0';
		  delete_key ();
		  i--;
		}
	    }
	  i--;
	}
      else
	{
	  /* 通常文字の場合 */
	  putchar (ch);
	  line[i] = ch;
	}
    }
  line[i] = '\0';
  return (i);
}



/***************************************************************************************
 *
 *	ビルトインコマンド
 *
 ***************************************************************************************/


void
f_version ()
{
  T_VER	ver;

  get_ver (&ver);
  printf ("** ITRON for BTRON/386 Version informatoin. **\n");
  printf ("Maker:    %d\n", ver.maker);
  printf ("ID:       %d\n", ver.id);
  printf ("Spec ver: %d\n", ver.spver);
  printf ("CPU:      %d\n", ver.cpu);
}


void
f_help ()
{
  printf ("*** debugger help ***\n");
  printf ("help    --- This message.\n");
  printf ("dump    --- Dump memory.\n");
  printf ("ps      --- Print task status.\n");
  printf ("status  --- Print status.\n");
  printf ("free    --- Print status of resource.\n");
  printf ("suspend --- Suspend task.\n");
  printf ("wakeup  --- Wakeup task.\n");
  printf ("reset   --- System reset.\n");
  printf ("demo    --- ITRON demo.\n");
  printf ("syscall --- systemcall test.\n");
}

/***************************************************************************
 * f_dump --- メモリのダンプ
 */
void
f_dump (W ac, B **av)
{
  UW	addr;

  
}

void
f_ps ()
{
  print_task_list ();
}

void
f_status(W ac, B **av)
{
  ID		taskid;
  ER		err;
  T_RTSK	stat;

  taskid = (ID)atoi (av[1]);
  if (taskid <= 0)
    {
      return ;
    }
  err = ref_tsk (&stat, taskid);
  if (err != E_OK)
    {
      printf ("status: argument error (error number = %d)\n", err);
      return;
    }
  printf ("Task %d status.\n", taskid);
  printf ("task priority: %d\n", stat.tskpri);
  printf ("task status:   %d\n", stat.tskstat);
  printf ("total time:    %d\n", stat.total_time);
}


void
f_free(W ac, B **av)
{
  if (strcmp (av[0], "memory") || (ac <= 0))
    {
      printf ("** free memory. **\n");
    }
  else if (strcmp (av[0], "task") || (ac <= 0))
    {
      printf ("** free task. **\n");
    }
}

/*******************************************************************
 * f_suspend --- 指定したタスクのサスペンド
 *
 */ 
void
f_suspend (W ac, B **av)
{
  ID	taskid;
  ER	err;

  if (ac >= 2)
    {
      taskid = atoi (av[1]);
      if (taskid == 0)
	{
	  printf ("suspend: argument error.\n");
	  return;
	}
      printf ("suspend task %d\n", taskid);
      err = sus_tsk (taskid);
      if (err != E_OK)
	{
	  printf ("suspend: error %d\n");
	}
      else
	{
	  printf ("suspend task (%d)\n");
	}
    }
}


/*******************************************************************
 * f_wakeup --- 指定したタスクの wakeup
 *
 */ 
void
f_wakeup (W ac, B **av)
{
  ID	id;
  ER	err;
  W	i;

  if (ac == 2)
    {
      id = atoi (av[1]);
      if (id == 0)
	{
	  printf ("suspend: argument error.\n");
	  return;
	}
      err = rsm_tsk (id);
      if (err != E_OK)
	{
	  printf ("resume: error %d\n", id);
	}
      else
	{
	  printf ("resume task %d\n", id);
	}
    }
}


/***************************************************************************
 */
void
f_cpureset ()
{
  outb (0x00f0, 0);
}

/*
 * get_resport --- 返答受信用のポートを作成する
 */
static ID
get_resport (T_CMBF *pk_cmbf)
{
  ID	i;
  ER	err;

  for (i = MIN_MSGBUF; i < MAX_MSGBUF; i++)
    {
      err = cre_mbf (i, pk_cmbf);
      if (err == E_OK)
	{
	  return (i);
	}
    }
  return (0);
}

void
f_ioecho (int ac, char **av)
{
  ER	err;
  ID	id;
  T_IO_REQUEST	req;
  T_IO_RESPONSE	res;
  T_CMBF	res_port_arg;
  ID	res_port;

  err = ref_dev (L"ＥＣＨＯ", CHAR, &id);
  if (err != E_OK)
    {
      printf ("debug: cannot get io device (ECHO)\n");
      return;
    }
  res_port_arg.mbfatr = TA_TFIFO;
  res_port_arg.bufsz = sizeof (T_IO_RESPONSE);
  res_port_arg.maxmsz = sizeof (T_IO_RESPONSE);
  res_port = get_resport (&res_port_arg);
  if (res_port == 0)
    {
      printf ("cannot create message buffer.\n");
      return;
    }

  req.command = IO_WRITE;
  req.resport = res_port;
  req.s.write_pack.size = strlen (av[1]);
  req.s.write_pack.bufp = av[1];
  err = io_request (id, &req, &res, IO_SYNC);
  if (err != E_OK)
    {
      printf ("debug: cannot io_request error = %d\n", err);
      return;
    }
  printf ("\ndebug; io request success.\n");
}


void
f_gdc_test (int ac, char **av)
{
  ER	err;
  ID	id;
  T_IO_REQUEST	req;
  T_IO_RESPONSE	res;
  T_CMBF	res_port_arg;
  ID	res_port;
  struct graphic_packet	graph;

  err = ref_dev (L"ＤＳＰ", CHAR, &id);
  if (err != E_OK)
    {
      printf ("debug: cannot get io device (ECHO)\n");
      return;
    }
  res_port_arg.mbfatr = TA_TFIFO;
  res_port_arg.bufsz = sizeof (T_IO_RESPONSE);
  res_port_arg.maxmsz = sizeof (T_IO_RESPONSE);
  res_port = get_resport (&res_port_arg);
  if (res_port == 0)
    {
      printf ("cannot create message buffer.\n");
      return;
    }

  graph.command = Draw_Line;
  graph.b.line.x1 = 10;
  graph.b.line.y1 = 10;
  graph.b.line.x2 = 10;
  graph.b.line.y2 = 10;
  req.command = IO_CONTROL;
  req.resport = res_port;
  req.s.control_pack.argp = &graph;
  err = io_request (id, &req, &res, IO_SYNC);
  if (err != E_OK)
    {
      printf ("debug: cannot io_request error = %d\n", err);
      return;
    }
  printf ("\ndebug; io request success.\n");
}


/***************************************************************************
 *
 *	ITRON によるデモプログラム --- 1 ---
 *
 */

static int	demo_c = 'a';
static int	sleep_flag;

static void
wakeup_func ()
{
  set_flg (sleep_flag, 0xffff);
  clr_flg (sleep_flag, 0);
  set_timer (50, wakeup_func, NULL);
}

static void
demo_program (void)
{
  int	x, y;
  int	delta_x;
  int	delta_y;
  int	ch;
  int	task_id;
  int	flag_pattern;
  int	err;

  get_tid (&task_id);
  x = task_id;
  y = 10;
  delta_x = +1;
  delta_y = +1;
  ch = demo_c;
  demo_c++;
  for (;;)
    {
      move_cursol (x, y);
      putchar (' ');
      x += delta_x;
      y += delta_y;
      move_cursol (x, y);
      putchar (ch);
      if ((x >= 78) || (x <= 1))
	{
	  delta_x = -delta_x;
	}
      if ((y >= 23) || (y <= 1))
	{
	  delta_y = -delta_y;
	}
      err = wai_flg (&flag_pattern, sleep_flag, 1, TWF_ORW);
      if (!(flag_pattern & 1))
	{
	  falldown ("demo_program: wai_flag invalid %d\n", err);
	}
    }
}


static void
fork_demo (void)
{
  T_CTSK	pktsk;
  ER		err;
  int		taskid;

  pktsk.tskatr = TA_HLNG;
  pktsk.startaddr = demo_program;
  pktsk.itskpri = 1;
  pktsk.stksz = PAGE_SIZE * 2;
  pktsk.addrmap = NULL;
  err = new_task (&pktsk, &taskid, TRUE);
}

void
f_demo (int ac, char **av)
{
  W	ch;
  T_CFLG	event_flag;

  event_flag.iflgptn = 1;
  event_flag.flgatr = TA_WMUL;
  sleep_flag = 10;
  cre_flg (sleep_flag, &event_flag);
  set_timer (50, wakeup_func, NULL);
  
  console_clear ();
  move_cursol (20, 5);
  printf ("---  DEMO PROGRAM START. ---\n");
  
  fork_demo ();
  for (;;)
    {
      ch = getchar ();
      switch (ch)
	{
	case '+':
	  fork_demo ();
	  break;

	case 'p':
	  console_clear();
	  print_task_list ();
	  getchar ();
	  console_clear();
	  break;
	}
    }
}

/* システムコールのテスト関数。
 *
 */
void
f_syscall ()
{
  W	rval;

  printf ("system call test.\n");
  rval = call_syscall (100, "foo\n");
  printf ("system call return value. %d\n", rval);
}



/*
 *
 */
void
f_bootinfo ()
{
  W			i;
  struct boot_header	*info;
  struct module_info	*modulep;

  info = (struct boot_header *)((unsigned int)MODULE_TABLE);
  modulep = (struct module_info *)info->modules;
  printf ("info addr = 0x%x\n", info);
  printf ("module count = %d\n", info->count);
  for (i = 0; i < info->count; i++)
    {
      printf ("[%d]: vaddr = 0x%x, paddr = 0x%x, entry = 0x%x, %s\n",
	      i, modulep[i].vaddr, modulep[i].paddr, modulep[i].entry, modulep[i].name);
    }
}

/* run_module
 *
 */
void
f_run_module (W ac, B **av)
{
  W			i, n;
  struct boot_header	*info;
  struct module_info	*modulep;
  ID			rid;
  T_CTSK		pktsk;
  T_TCB			*new_taskp;

  n = atoi (av[1]);
  info = (struct boot_header *)MODULE_TABLE;
  if ((n < 1) || (n >= info->count))
    {
      printf ("module is overflow.\n");
      return;
    }
  modulep = info->modules;
  printf ("run module: %d (%s) ", n, modulep[n].name);
#ifdef notdef
  printf ("module information: vaddr = 0x%x, paddr = 0x%x, entry = 0x%x\n\n",
	  modulep[n].vaddr, modulep[n].paddr, modulep[n].entry);
#endif /* notdef */
  pktsk.tskatr = TA_HLNG;
  pktsk.itskpri = 2;
  pktsk.stksz = PAGE_SIZE * 2;
  pktsk.addrmap = NULL;
/*  pktsk.addrmap = 0x3000; */
  pktsk.startaddr = (FP)modulep[n].entry;
  if (new_task (&pktsk, &rid, FALSE) != E_OK)
    {
      printf ("Can not make new task.\n");
      return;
    }
  printf ("Task id = %d\n", rid);
  new_taskp = get_tskp (rid);
  if (new_taskp == NULL)
    {
      printf ("new task is NULL.\n");
      return;
    }

  /* 生成したタスクの仮想メモリにモジュールをマッピング */
  /* ただしドライバの場合には、マッピングしない */
  if (modulep[n].type == driver)
    {
      printf ("This module is driver. not mapped\n");
    }
  else
    {
      for (i = 0; i < ROUNDUP (modulep[n].length, PAGE_SIZE) / PAGE_SIZE; i++)
	{
#ifdef notdef
	  printf ("memory map: virtual addr: 0x%x, phisical addr = 0x%x\n",
		  modulep[n].vaddr + i * PAGE_SIZE, 
		  modulep[n].paddr + i * PAGE_SIZE);
#endif /* notdef */
      
	  if (vmap (new_taskp,
		    modulep[n].vaddr + i * PAGE_SIZE, 
		    modulep[n].paddr + i * PAGE_SIZE) == FALSE)
	    {
	      printf ("Cannot memory map: virtual addr: 0x%x, phisical addr = 0x%x\n",
		      modulep[n].vaddr + i * PAGE_SIZE, 
		      modulep[n].paddr + i * PAGE_SIZE);
	    }
	}
    }
  sta_tsk (rid, 0);
/*  printf ("run-module: done.\n"); */
}

/* コンソールのテストプログラム
 */
void
f_console_test (W ac, B **av)
{
  ID			port;
  ID			resport;
  DDEV_REQ		req;		/* 要求パケット */
  DDEV_RES		res;		/* 返答パケット */
  W			rsize;
  struct find_port_t	finddata;
  ER			error;
  T_CMBF		cmbf;

  strcpy (finddata.name, "driver.console");	/* */
  find_port (&finddata, &port);
  printf ("name = driver.console, port = %d\n", port);

  cmbf.bufsz  = sizeof (res);
  cmbf.maxmsz = sizeof (res);
  cmbf.mbfatr = TA_TFIFO;
  resport = get_resport (&cmbf);

  printf ("*** OPEN ***\n");
  req.header.mbfid = resport;
  req.header.msgtyp = DEV_OPN;
  req.body.opn_req.dd = 0xAA;
  error = snd_mbf (port, sizeof (req), &req);
  if (error != E_OK)
    {
      printf ("cannot send packet. %d\n", error);
      return;
    }

  rsize = sizeof (res);
  rcv_mbf (&res, (INT *)&rsize, resport);

  printf ("res.dd      = 0x%x\n", res.body.opn_res.dd);
  printf ("res.error   = 0x%x\n", res.body.opn_res.errcd);
  printf ("res.errinfo = 0x%x\n", res.body.opn_res.errinfo);

  printf ("*** READ ***\n");
  req.header.mbfid = resport;
  req.header.msgtyp = DEV_REA;
  req.body.rea_req.dd = 0xAA;
  error = snd_mbf (port, sizeof (req), &req);
  if (error != E_OK)
    {
      printf ("cannot send packet. %d\n", error);
      return;
    }
  rsize = sizeof (res);
  rcv_mbf (&res, (INT *)&rsize, resport);
  printf ("res.dd      = 0x%x\n", res.body.rea_res.dd);
  printf ("res.error   = 0x%x\n", res.body.rea_res.errcd);
  printf ("res.errinfo = 0x%x\n", res.body.rea_res.errinfo);

  printf ("*** CLEAR (CTL) & WRITE *** \n");
  printf (">> "); getchar (); printf ("\n");
  req.header.mbfid = resport;
  req.header.msgtyp = DEV_CTL;
  req.body.ctl_req.dd = 0xAA;
  req.body.ctl_req.cmd = 1;	/* clear console */
  error = snd_mbf (port, sizeof (req), &req);
  if (error != E_OK)
    {
      printf ("cannot send packet. %d\n", error);
      return;
    }
  rsize = sizeof (res);
  rcv_mbf (&res, (INT *)&rsize, resport);

  req.header.mbfid = resport;
  req.header.msgtyp = DEV_CTL;
  req.body.ctl_req.dd = 0xAA;
  req.body.ctl_req.cmd = 2;	/* move corsol */
  req.body.ctl_req.len = 2;
  req.body.ctl_req.param[0] = 0;	/* X */
  req.body.ctl_req.param[1] = 10;	/* Y */
  error = snd_mbf (port, sizeof (req), &req);
  if (error != E_OK)
    {
      printf ("cannot send packet. %d\n", error);
      return;
    }
  rsize = sizeof (res);
  rcv_mbf (&res, (INT *)&rsize, resport);
  if (res.body.ctl_res.errcd != E_OK)
    {
      printf ("ctl_res.errcd = %d\n", res.body.ctl_res.errcd);
      return;
    }

  req.header.mbfid = resport;
  req.header.msgtyp = DEV_WRI;
  req.body.wri_req.dd = 0xAA;
  req.body.wri_req.size = 40;
  bcopy ("01234\t890123456789\n0123\b\b234567890123456789", req.body.wri_req.dt, 44);
  error = snd_mbf (port, sizeof (req), &req);
  if (error != E_OK)
    {
      printf ("cannot send packet. %d\n", error);
      return;
    }
  rsize = sizeof (res);
  rcv_mbf (&res, (INT *)&rsize, resport);
  getchar ();
  printf ("res.dd      = 0x%x\n", res.body.wri_res.dd);
  printf ("res.error   = 0x%x\n", res.body.wri_res.errcd);
  printf ("res.errinfo = 0x%x\n", res.body.wri_res.errinfo);
  return;
}

/* キーボードのテストプログラム
 */
void
f_keyboard_test (W ac, B **av)
{
  ID			port;
  ID			resport;
  DDEV_REQ		req;		/* 要求パケット */
  DDEV_RES		res;		/* 返答パケット */
  W			rsize;
  struct find_port_t	finddata;
  ER			error;
  T_CMBF		cmbf;
  extern W		readc (ID port, ID resport);


  av[0] = "run-module";
  av[1] = "3";
  f_run_module (2, av);

  strcpy (finddata.name, "driver.keyboard");	/* */
  do
    {
      find_port (&finddata, &port);
      printf ("name = driver.keyboard, port = %d\n", port);
    }
  while (port == 0);

  cmbf.bufsz  = sizeof (res);
  cmbf.maxmsz = sizeof (res);
  cmbf.mbfatr = TA_TFIFO;
  resport = get_resport (&cmbf);

  printf ("*** OPEN ***\n");
  req.header.mbfid = resport;
  req.header.msgtyp = DEV_OPN;
  req.body.opn_req.dd = 0xAA;
  error = snd_mbf (port, sizeof (req), &req);
  if (error != E_OK)
    {
      printf ("cannot send packet. %d\n", error);
      return;
    }

  rsize = sizeof (res);
  rcv_mbf (&res, (INT *)&rsize, resport);

  printf ("res.dd      = 0x%x\n", res.body.opn_res.dd);
  printf ("res.error   = 0x%x\n", res.body.opn_res.errcd);
  printf ("res.errinfo = 0x%x\n", res.body.opn_res.errinfo);

  printf ("*** READ ***\n");
  readc (port, resport);
  falldown ("...");
  return;
}


W
readc (ID port, ID resport)
{
  DDEV_REQ		req;		/* 要求パケット */
  DDEV_RES		res;		/* 返答パケット */
  W			rsize;
  ER			error;
  W			i;
  
  for (;;)
    {
      req.header.mbfid = resport;
      req.header.msgtyp = DEV_REA;
      req.body.rea_req.dd = 0xAA;
      req.body.rea_req.size = 10;
      error = snd_mbf (port, sizeof (req), &req);
      if (error != E_OK)
	{
	  printf ("cannot send packet. %d\n", error);
	  return;
	}
      rsize = sizeof (res);
      rcv_mbf (&res, (INT *)&rsize, resport);
      if (res.body.rea_res.dt[0] != 0)
	{
#ifdef notdef
	  printf ("res.dd      = 0x%x\n", res.body.rea_res.dd);
	  printf ("res.error   = 0x%x\n", res.body.rea_res.errcd);
	  printf ("res.errinfo = 0x%x\n", res.body.rea_res.errinfo);
	  printf ("-(%c)%d-", res.body.rea_res.dt[0], res.body.rea_res.dt[0]);
#endif /* notdef */
	  printf ("res.a_size  = 0x%x\n",  res.body.rea_res.a_size);
	  for (i = 0; i < res.body.rea_res.a_size; i++)
	    putchar (res.body.rea_res.dt[i]);
	}
    }
}

void
f_fd_test (W ac, B **av)
{
  ID			port;
  ID			resport;
  DDEV_REQ		req;		/* 要求パケット */
  DDEV_RES		res;		/* 返答パケット */
  W			rsize;
  struct find_port_t	finddata;
  ER			error;
  T_CMBF		cmbf;
  W			i;

#ifdef notdef
  av[0] = "run-module";
  av[1] = "3";
  f_run_module (2, av);
#endif /* notdef */

  strcpy (finddata.name, "driver.fd");	/* */
  do
    {
      find_port (&finddata, &port);
      printf ("name = driver.keyboard, port = %d\n", port);
    }
  while (port == 0);

  cmbf.bufsz  = sizeof (res);
  cmbf.maxmsz = sizeof (res);
  cmbf.mbfatr = TA_TFIFO;
  resport = get_resport (&cmbf);

  printf ("*** OPEN ***\n");
  req.header.mbfid = resport;
  req.header.msgtyp = DEV_OPN;
  req.body.opn_req.dd = 0x00;
  error = snd_mbf (port, sizeof (req), &req);
  if (error != E_OK)
    {
      printf ("cannot send packet. %d\n", error);
      return;
    }

  rsize = sizeof (res);
  rcv_mbf (&res, (INT *)&rsize, resport);

  printf ("res.dd      = 0x%x\n", res.body.opn_res.dd);
  printf ("res.error   = 0x%x\n", res.body.opn_res.errcd);
  printf ("res.errinfo = 0x%x\n", res.body.opn_res.errinfo);

  printf ("*** READ ***\n");
  req.header.mbfid = resport;
  req.header.msgtyp = DEV_REA;
  req.body.rea_req.dd = 0x00;
  req.body.rea_req.start = atoi (av[1]);
  req.body.rea_req.size = 1024;
  error = snd_mbf (port, sizeof (req), &req);
  if (error != E_OK)
    {
      printf ("cannot send packet. %d\n", error);
      return;
    }
  printf ("kernel: snd_mbf\n");	/* */
  rsize = sizeof (res);
  rcv_mbf (&res, (INT *)&rsize, resport);
  printf ("kernel: rcv_mbf\n");	/* */
  printf ("res.dd      = 0x%x\n", res.body.rea_res.dd);
  printf ("res.error   = 0x%x\n", res.body.rea_res.errcd);
  printf ("res.errinfo = 0x%x\n", res.body.rea_res.errinfo);
  if (res.body.rea_res.a_size > 0)
    {
      printf ("res.a_size  = 0x%x\n",  res.body.rea_res.a_size);
      for (i = 0; i < 5; i++)
	{
	  printf ("0x%x (%d)\n", res.body.rea_res.dt[i], res.body.rea_res.dt[i]);
	}
    }
#ifdef notdef
  printf ("*** WRITE ***\n");
  req.header.mbfid = resport;
  req.header.msgtyp = DEV_WRI;
  req.body.wri_req.dd = 0x00;
  req.body.wri_req.start = atoi (av[1]);
  req.body.wri_req.size = 1024;
  for (i = 0; i < 512; i++)
    {
      req.body.wri_req.dt[i] = 0xbb;
    }
  for (i = 512; i < 1024; i++)
    {
      req.body.wri_req.dt[i] = 0xcc;
    }
  
  error = snd_mbf (port, sizeof (req), &req);
  if (error != E_OK)
    {
      printf ("cannot send packet. %d\n", error);
      return;
    }
  printf ("kernel: snd_mbf\n");	/* */
  rsize = sizeof (res);
  rcv_mbf (&res, (INT *)&rsize, resport);
  printf ("kernel: rcv_mbf\n");	/* */
#endif
}

void
f_fd_write (W ac, B **av)
{
  ID			port;
  ID			resport;
  DDEV_REQ		req;		/* 要求パケット */
  DDEV_RES		res;		/* 返答パケット */
  W			rsize;
  struct find_port_t	finddata;
  ER			error;
  T_CMBF		cmbf;
  W			i;

  strcpy (finddata.name, "driver.fd");	/* */
  do
    {
      find_port (&finddata, &port);
      printf ("name = driver.fd, port = %d\n", port);
    }
  while (port == 0);

  cmbf.bufsz  = sizeof (res);
  cmbf.maxmsz = sizeof (res);
  cmbf.mbfatr = TA_TFIFO;
  resport = get_resport (&cmbf);

  printf ("*** OPEN ***\n");
  req.header.mbfid = resport;
  req.header.msgtyp = DEV_OPN;
  req.body.opn_req.dd = 0x00;
  error = snd_mbf (port, sizeof (req), &req);
  if (error != E_OK)
    {
      printf ("cannot send packet. %d\n", error);
      return;
    }

  rsize = sizeof (res);
  rcv_mbf (&res, (INT *)&rsize, resport);

  printf ("res.dd      = 0x%x\n", res.body.opn_res.dd);
  printf ("res.error   = 0x%x\n", res.body.opn_res.errcd);
  printf ("res.errinfo = 0x%x\n", res.body.opn_res.errinfo);

  printf ("*** WRITE ***\n");
  req.header.mbfid = resport;
  req.header.msgtyp = DEV_WRI;
  req.body.wri_req.dd = 0x00;
  req.body.wri_req.start = atoi (av[1]);
  req.body.wri_req.size = 1024;
  for (i = 0; i < 512; i++)
    {
      req.body.wri_req.dt[i] = 0xbb;
    }
  for (i = 512; i < 1024; i++)
    {
      req.body.wri_req.dt[i] = 0xcc;
    }
  
  error = snd_mbf (port, sizeof (req), &req);
  if (error != E_OK)
    {
      printf ("cannot send packet. %d\n", error);
      return;
    }
  printf ("kernel: snd_mbf\n");	/* */
  rsize = sizeof (res);
  rcv_mbf (&res, (INT *)&rsize, resport);
  printf ("kernel: rcv_mbf\n");	/* */
}

void
f_fd_chmod (W ac, B **av)
{
  ID			port;
  ID			resport;
  DDEV_REQ		req;		/* 要求パケット */
  DDEV_RES		res;		/* 返答パケット */
  W			rsize;
  struct find_port_t	finddata;
  ER			error;
  T_CMBF		cmbf;
  W			i;

  strcpy (finddata.name, "driver.fd");	/* */
  do
    {
      find_port (&finddata, &port);
      printf ("name = driver.keyboard, port = %d\n", port);
    }
  while (port == 0);

  cmbf.bufsz  = sizeof (res);
  cmbf.maxmsz = sizeof (res);
  cmbf.mbfatr = TA_TFIFO;
  resport = get_resport (&cmbf);

  if (strcmp (av[1], "2HD") == 0)
    {
      req.body.ctl_req.param[0] = 0;	/* M2HD */
    }
  else if  (strcmp (av[1], "2HC") == 0)
    {
      req.body.ctl_req.param[0] = 1;	/* M2HC */
    }
  else
    {
      printf ("unknown disk mode: %s\n", av[1]);
      return;
    }
      
  printf ("change mode\n");
  req.header.mbfid = resport;
  req.header.msgtyp = DEV_CTL;
  req.body.ctl_req.dd = 0x00;
  req.body.ctl_req.cmd = 1;	/* CHANGE_MODE */
  req.body.ctl_req.len = 1;
  req.body.ctl_req.param[1] = 0;	/* drive 0 */
  error = snd_mbf (port, sizeof (req), &req);
  if (error != E_OK)
    {
      printf ("cannot send packet. %d\n", error);
      return;
    }
  printf ("kernel: snd_mbf\n");	/* */
  rsize = sizeof (res);
  rcv_mbf (&res, (INT *)&rsize, resport);
  printf ("kernel: rcv_mbf\n");	/* */
  printf ("dev_ctl: error = %d\n", res.body.ctl_res.errcd);
}

void
f_scsi (W ac, B **av)
{
  W	i;
  UB	*flag;
  struct scsiinfo
    {
      UB	sector;
      UB	head:4;
      UB	cylinder_msb:4;
      UB	cylinder_lsb;
      UB	cylinder:4;
      UB	blocksize:2;
      UB	cylinder_length:1;
      UB	hard_sec:1;
    } *info;
  struct sinfo
    {
      UB	table[4];
    } *sinfo;
  W auxreg;


  flag = (UB*)0x80000482;
  printf ("SCSI ID BITS: 0x%x\n", *flag);

  info = (struct scsiinfo *)0x80000460;
  sinfo = info;
  for (i = 0; i < 6; i++)
    {
      if (*flag & (1 << i))
	{
	  printf ("ID[%d]: Exist:\n", i);
	  printf ("  sector / track : %d\n", info[i].sector);
	  printf ("  head           : %d\n", info[i].head);
	  printf ("  cylinder(MSB)  : %d\n", info[i].cylinder_msb);
	  printf ("  cylinder(LSB)  : %d\n", info[i].cylinder_lsb);
	  printf ("  sector         : %s\n", info[i].hard_sec ? "hard sector" : "soft sector");
	  printf ("  cylinder length: %d\n", info[i].cylinder_length);
	  printf ("  blocksize      : %d\n", info[i].blocksize);
	  printf ("  cylinder       : %d\n", info[i].cylinder);
	  printf ("  info[0] = 0x%x, 0x%x, 0x%x, 0x%x\n", 
		  sinfo[i].table[0],
		  sinfo[i].table[1],
		  sinfo[i].table[2],
		  sinfo[i].table[3]);
	}
      else
	{
	  printf ("ID[%d]: Not Exist:\n", i);
	}
    }
  if (*flag != 0)
    {
      auxreg = inb (0x0cc0);
      printf ("Aux reg:0x%x\n", auxreg);
    }
}

void
f_init (W ac, B **av)
{
  struct boot_header	*info;
  struct module_info	*modulep;
  ID			rid;
  T_CTSK		pktsk;
  T_TCB			*new_taskp;
  W			i, n;

  info = (struct boot_header *)MODULE_TABLE;
  for (i = 0; i < info->count; i++)
    {
      run (i);
    }

  for (;;)
    slp_tsk ();
}
