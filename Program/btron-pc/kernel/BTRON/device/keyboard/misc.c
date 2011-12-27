/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/keyboard/misc.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/keyboard/misc.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: misc.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  2000-04-03 14:30:06  naniwa
 * to call timer handler in task
 *
 * Revision 1.3  2000/02/06 09:06:07  naniwa
 * to follow mirco-ITRON 3.0 specifications
 *
 * Revision 1.2  1998/02/23 14:39:54  night
 * wai_flg の引数が間違っていたため修正。
 * 今までのコードだと、仮想空間の先頭部分を破壊していた。
 *
 * Revision 1.1  1996/07/24 16:28:44  night
 * IBM PC 版 B-Free OS への最初の登録
 *
 * Revision 1.3  1995/09/21  15:51:00  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/09/19  18:01:02  night
 * キーボードから入力できるところまでできた。
 *
 * Revision 1.1  1995/09/18  11:42:03  night
 * 最初の登録
 *
 *
 */

#include "keyboard.h"

#define MAX_LIST	1024

struct key_entry
{
  struct key_entry	*next;
  W			key;
};

struct
{
  struct key_entry	*first;
  struct key_entry	*last;
} current;

struct key_entry	*freelist;
struct key_entry	keybuffer[MAX_LIST];


/*
 *
 */
init_keybuffer (void)
{
  W	i;

  freelist = keybuffer;
  for (i = 0; i < MAX_LIST - 1; i++)
    {
      keybuffer[i].next = &keybuffer[i + 1];
    }
  keybuffer[i].next = NULL;
  current.first = current.last = NULL;
}


/*
 * バッファの内容をクリアする。
 */
clear_keybuffer ()
{
  init_keybuffer ();
  current.first = NULL;
  current.last = NULL;
}

void
put_entry (W keycode)
{
  struct key_entry	*ent;

#ifdef notdef
  dbg_printf ("put_entry: %d\n", keycode);
#endif

  /*
   * キーエントリを入れるところがなくなったら、何もせずに帰る
   */
  if (freelist == NULL)
    {
      dbg_printf ("keyboard: freelist is empty.\n");
      return;
    }

  ent = freelist;
  freelist = freelist->next;

  ent->key = keycode;
  ent->next = NULL;
  if (current.first == NULL)
    {
      current.first = current.last = ent;
    }
  else
    {
      current.last->next = ent;
      current.last = ent;
    }
}

/*
 * キーリストから一文字読み取り値を返す。
 * もし、読み込んだ文字がなければ、中身が入るまで待つ。
 */
W
read_key (W mode)
{
  W			retval;
  struct key_entry	*p;
  UINT			rflag;

  if ((mode == NOWAITMODE) && (current.first == NULL))
    {
      return (NULL);
    }

retry:
  if (current.first)
    {
      retval = current.first->key;
#ifdef notdef
      dbg_printf ("keyboard: retval = %d\n", retval);	/* */
#endif
      p = current.first;
      if (current.first == current.last)	/* 溜まっているキーがもはやない */
	{
	  current.first = current.last = NULL;
#ifdef notdef
	  dbg_printf ("keyboard: read_key(): set_flg\n");	/* */
#endif /* notdef */
#ifdef notdef
	  set_flg (waitflag, 0);	/* イベントフラグをクリアする */
#else
	  clr_flg (waitflag, 0);
#endif
	}
      else
	{
	  current.first = current.first->next;
	}
      p->next = freelist;
      freelist = p;

#ifdef notdef
      dbg_printf ("<%d> ", retval);
#endif /* notdef */
      return (retval);
    }

  /* キー入力待ち */
#ifdef notdef
  dbg_printf ("keyboard: waiting...\n");	/* */
#endif
  wai_flg (&rflag, waitflag, ONKEYS, TWF_ORW);
#ifdef notdef
  dbg_printf ("KEYBOARD: waked up by flag\n");
#endif
  goto retry;
}
