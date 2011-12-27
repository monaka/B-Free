/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* console.c --- コンソールへの入出力モジュール
 *
 *
 * コンソールへの入力および出力を行う。
 *
 * なお、ここで定義しているコンソール入出力関数群は、ITRON レベルのため、
 * 上位層では、別のデバイスドライバが入出力を行う。また、ITRON からの出力も、
 * 上位層で定義するデバイスドライバを介して行う。ここの関数群を使用するのは、
 * 立ち上げ直後だけである。
 *
 *
 */

#include "itron.h"
#include "itron_module.h"
#include "errno.h"
#include "misc.h"
#ifdef PORTMANAGER
#include "../servers/port-manager.h"
#endif
#include "func.h"
#ifdef KERNLIB
#include "../kernlib/device.h"
#endif

/*	アトリビュートの設定
 */
#define NORM_ATTR	0x1e		/* 通常文字 			*/
#define MAX_WIDTH	79
#define MAX_HEIGHT	24

#ifdef IBMPC
#define TEXT_VRAM_ADDR	0x800B8000
#define TEXT_VRAM_SIZE	2000

#define GDC_ADDR	0x03d4
#define GDC_DATA	0x03d5
#else
#define TEXT_VRAM_ADDR	0x800A0000	/* TEXT VRAM のアドレス		*/
#define TEXT_VRAM_SIZE	2000

#define GDC_STAT	0x0060
#define GDC_COMMAND	0x0062
#endif


#define CURSOR_POS(x,y)		(x + y * 80)

#if 0
static W	mydevid;
#endif
static ER	call_console (ID driver, TC ch);

ID		console_driver = 0;
ID		console_recv;



/***********************************************************************
 * position --- カーソル位置情報
 *
 */
struct position
{
  W	x;
  W	y;
};

static	struct position	cursor;


static void	set_cursor_pos (W x, W y);
static void	inc_cursor (W count);
static void	write_vram (W x, W y, W ch, W attr);
static void	write_kanji_vram (W x, W y, UW kanji, W attr);
static void	scroll_up (void);

/*************************************************************************
 * simple_init_console 
 *
 * 引数：
 *
 * 返値：
 *
 * 処理：
 *
 */
void
simple_init_console (void)
{
  console_clear ();
}

/*************************************************************************
 * init_crt --- CRT の初期化
 *
 * 引数：	なし
 *
 * 返値：	コンソールデバイスのデバイス情報
 *
 * 処理：	コンソールデバイスを初期化する(画面をクリアする)。
 *		カーソルポインタ (cursor) を初期化する。
 *		ドライバ情報を初期化し、呼び出し関数に返す。
 *
 * 注意：	ドライバ情報は、あらかじめ GLOBAL として宣言してあるが、
 *		ドライバ名だけは、開発環境の漢字コードが TRON コードで
 *		ないため、この関数で TRON コードに変換する。
 *
 */
BOOL
init_crt (W id)
{
  return (TRUE);
}

/************************************************************************
 * intr_crt
 */
W
intr_crt ()
{
  return 0;  /* dummy value */
}

/*************************************************************************
 * putchar --- １文字出力; カーネル内のサービスルーチン
 *
 *
 * 引数：	ch	出力する文字コード (TRON コード)
 *
 * 返値：	なし
 *
 * 処理：	write_vram を呼び出し、VRAM に１文字出力する。
 *		この関数は、カーネルからメッセージを出力する時にも使用する。
 *		そのため、この関数は GLOBAL として定義している。
 *
 *
 * 注意：	この関数は、出力できる文字かどうかをチェックしていない。
 *		出力できない文字が指定された場合でも、VRAM に書き込もうとする。
 *
 */
void
putchar (TC ch)
{
#if 0
  TC	c;
  static B before_ch;
#endif

  if (console_driver != NULL)
    {
      call_console (console_driver, ch);
#ifdef notdef
      return;
#endif
    }
#ifdef ntodef
  dis_int ();
#endif

  switch (ch)
    {
    default:
      write_vram (cursor.x, cursor.y, ch, NORM_ATTR);
      inc_cursor (1);
      break;

    case '\n':
      write_cr ();
      break;

    case '\t':
      write_tab ();
      break;
    }
#ifdef ntodef
  ena_int ();
#endif
}


/* call_console
 *
 */
static ER
call_console (ID driver, TC ch)
{
  DDEV_REQ		req;		/* 要求パケット */
  DDEV_RES		res;		/* 返答パケット */
  W			rsize;
  ER			error;
#if 0
  W			i;
#endif
  
  ena_int ();
  req.header.mbfid = console_recv;
  req.header.msgtyp = DEV_WRI;
  req.body.wri_req.dd = 0xAA;
  req.body.wri_req.size = 1;
  req.body.wri_req.dt[0] = (B) (ch & 0xff);
  error = snd_mbf (console_driver, sizeof (req), &req);
  if (error != E_OK)
    {
      return (E_SYS);
    }
  rsize = sizeof (res);
  error = rcv_mbf (&res, (INT *)&rsize, console_recv);
  if (res.body.wri_res.errcd != E_OK)
    {
      return (E_SYS);
    }
  return (E_OK);
}


/*************************************************************************
 * putchar_tron --- １文字出力; カーネル内のサービスルーチン
 *
 *
 * 引数：	ch	出力する文字コード (TRON コード)
 *
 * 返値：	なし
 *
 * 処理：	write_vram を呼び出し、VRAM に１文字出力する。
 *		この関数は、カーネルからメッセージを出力する時にも使用する。
 *		そのため、この関数は GLOBAL として定義している。
 *
 *
 * 注意：	この関数は、出力できる文字かどうかをチェックしていない。
 *		出力できない文字が指定された場合でも、VRAM に書き込もうとする。
 *
 */
void
putchar_tron (TC ch)
{
  TC c;

  switch (ch)
    {
    default:
      if (ch & 0xFF00)
	{
	  c = halfword_swap (ch);
	  write_kanji_vram (cursor.x, cursor.y, c, NORM_ATTR);
	  inc_cursor (2);
	}
      else
	{
	  write_vram (cursor.x, cursor.y, ch, NORM_ATTR);
	  inc_cursor (1);
	}
      break;

    case '\n':
      write_cr ();
      break;

    case '\t':
      write_tab ();
      break;
    }
}

/*************************************************************************
 * inc_cursor --- カーソルを進める。
 *
 * 引数：	count	カーソルの移動量
 *
 * 返値：	なし
 *
 * 処理：	指定した量だけ、カーソルを進める(count は、せいぜい2である)。
 *		もし、カーソルが画面をはみだした場合は、カーソルを次の行に
 *		進める。
 *		さらに一番下の行にいる場合には、これ以上下の行にいけないので、
 *		スクロールアップする。
 *
 */
static void
inc_cursor (W count)
{
  cursor.x += count;
  if (cursor.x >= MAX_WIDTH)
    {
      cursor.y++;
      if (cursor.y >= MAX_HEIGHT)
	{
	  scroll_up ();
	  cursor.y = MAX_HEIGHT;
	}
      cursor.x = 0;
    }
  set_cursor_pos (cursor.x, cursor.y);
}

/*************************************************************************
 * write_cr --- 改行する
 *
 * 引数：	なし
 *
 * 返値：	なし
 *
 * 処理：	カーソルを次の行の先頭に移動させる。
 *		もし、すでに行が一番最後ならば、画面全体をスクロールする。
 *
 */
void
write_cr (void)
{
  cursor.x = 0;
  if (cursor.y >= MAX_HEIGHT)
    {
      scroll_up ();
    }
  else
    {
      cursor.y++;
    }
  set_cursor_pos (cursor.x, cursor.y);
}

/*************************************************************************
 * write_tab --- タブを出力する。
 *
 * 引数：	なし
 *
 * 返値：	なし
 *
 * 処理：	タブを出力する。
 *
 */
void
write_tab ()
{
  W	tmp;

  if (cursor.x < MAX_WIDTH)
    {
      tmp = ROUNDUP (cursor.x + 1, 8) - 1;
      while (cursor.x < tmp)
	{
	  write_vram (cursor.x, cursor.y, ' ', 0xe1);
	  cursor.x++;
	}
    }
}

/*************************************************************************
 * scroll_up --- 画面全体をスクロールアップする
 *
 * 引数：	なし
 *
 * 返値：	なし
 *
 * 処理：	画面全体を一行スクロールする。
 *
 */
static void
scroll_up (void)  
{
  UH	*addr;
  W		i;
  
  addr = (unsigned short *)TEXT_VRAM_ADDR;
  for (i = 0; i < TEXT_VRAM_SIZE - 80; i++)
    {
      addr[i] = addr[i + 80];
    }
  for (i = 0; i <= MAX_WIDTH; i++)
    {
      write_vram (i, MAX_HEIGHT, ' ', 0xe1);
    }
}

/*************************************************************************
 * console_clear --- コンソールをクリアする。
 *
 * 引数：	なし
 *
 * 返値：	なし
 *
 * 処理：	コンソールのはじからはじまでを空白文字で埋める。
 *
 */
void
console_clear (void)
{
  W	x, y;
  
  for (y = 0; y <= MAX_HEIGHT; y++)
    for (x = 0; x <= MAX_WIDTH; x++)
      {
	write_vram (x, y, ' ', 0xE1);
      }
  cursor.x = cursor.y = 0;
#ifdef PC9801
  set_cursor_pos (cursor.x, cursor.y);
#endif
}

/*************************************************************************
 * write_vram --- VRAM に文字を書き込む
 *
 * 引数：	x, y	座標
 *		ch	出力する文字
 *		attr	文字属性
 *
 * 返値：	なし
 *
 * 処理：	引数 x, y で指定した座標に文字を書き込む.
 *		この関数は、ASCII 文字用になっている。
 *
 *
 * 注意：	現在、attr は無視している。
 *
 */
static void
write_vram (W x, W y, W ch, W attr)
{
  short	*addr;
#if 0
  short *attr_addr;
#endif
  
  addr = (short *)TEXT_VRAM_ADDR;
  ch = ch | (addr [CURSOR_POS (x, y)] & 0xff00);
  addr [CURSOR_POS (x, y)] = ch;
}

/*************************************************************************
 * write_kanji_vram --- ＶＲＡＭに漢字を１文字書き込む.
 *
 * 引数：	x, y	座標
 *		kanji	出力する文字
 *		attr	文字属性
 *
 * 返値：	なし
 *
 * 処理：	引数 x, y で指定した座標に文字を書き込む.
 *		この関数は、TRON 文字用になっている。
 *
 *
 * 注意：	現在、attr は無視している。
 *
 */
static void
write_kanji_vram (W x, W y, UW kanji, W attr)
{
  UH	*addr;
#if 0
  H	*attr_addr;
#endif
  UH	 first, second;

  first = ((kanji >> 8) & 0xff) - 0x20;
  second = (kanji << 8) & 0xff00;

  addr = (unsigned short *)TEXT_VRAM_ADDR;
  addr [CURSOR_POS (x, y)] = second | first;
  addr [CURSOR_POS (x + 1, y)] = second | (first | 0x80);
}

/*************************************************************************
 * set_cursor_pos --- カーソル位置の設定.
 *
 * 引数：	x, y	座標
 *
 * 返値：	なし
 *
 * 処理：	指定した座標にカーソルを移動させる。
 *
 */
static void
set_cursor_pos (W x, W y)
{
  W	addr;
#ifdef notdef  
  dis_int ();
#endif
  addr = CURSOR_POS (x, y);
  outb (GDC_ADDR, 0x0e);
  outb (GDC_DATA, (addr >> 8) & 0xff);
  outb (GDC_ADDR, 0x0f);
  outb (GDC_DATA, addr & 0xff);
#ifdef notdef
  ena_int ();
#endif
}

/*************************************************************************
 * delete_key --- 文字を一文字消去する。
 *
 * 引数：	なし
 *
 * 返値：	なし
 *
 * 処理：	現在カーソルがある行の最後の文字を消去する。
 *
 */
void
delete_key (void)
{
  if (cursor.x > 0)
    {
      cursor.x--;
      write_vram (cursor.x, cursor.y, ' ', 0x1f);
      set_cursor_pos (cursor.x, cursor.y);
    }
}


/*************************************************************************
 * move_cursol --- カーソルを指定した位置に移動
 *
 * 引数：	x
 *		y
 *
 * 返値：	TRUE
 *		FALSE
 *
 * 処理：	大域変数 cursor には、カーソル位置が入っている。
 *		move_cursol() は、cursor の値を変更する。
 *		さらに、カーソルの表示位置を新しい cursor の位置に変更する。
 *
 */
W
move_cursol (int x, int y)
{
  if ((x >= 0) && (x < 80) &&
      (y >= 0) && (y < 25))
    {
      cursor.x = x;
      cursor.y = y;
      set_cursor_pos (cursor.x, cursor.y);
      return (TRUE);
    }
  else
    {
      return (FALSE);
    }
}


/*
 * 現在のカーソル位置を返す
 */
ER
get_cursor_position (W *x, W *y)
{
  *x = cursor.x;
  *y = cursor.y;
  return (E_OK);  
}
