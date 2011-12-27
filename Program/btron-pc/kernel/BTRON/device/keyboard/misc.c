/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

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
 * wai_flg �ΰ������ְ�äƤ������ὤ����
 * ���ޤǤΥ����ɤ��ȡ����۶��֤���Ƭ��ʬ���˲����Ƥ�����
 *
 * Revision 1.1  1996/07/24 16:28:44  night
 * IBM PC �� B-Free OS �ؤκǽ����Ͽ
 *
 * Revision 1.3  1995/09/21  15:51:00  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.2  1995/09/19  18:01:02  night
 * �����ܡ��ɤ������ϤǤ���Ȥ���ޤǤǤ�����
 *
 * Revision 1.1  1995/09/18  11:42:03  night
 * �ǽ����Ͽ
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
 * �Хåե������Ƥ򥯥ꥢ���롣
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
   * ��������ȥ�������Ȥ����ʤ��ʤä��顢���⤻���˵���
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
 * �����ꥹ�Ȥ����ʸ���ɤ߼���ͤ��֤���
 * �⤷���ɤ߹����ʸ�����ʤ���С���Ȥ�����ޤ��Ԥġ�
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
      if (current.first == current.last)	/* ί�ޤäƤ��륭������Ϥ�ʤ� */
	{
	  current.first = current.last = NULL;
#ifdef notdef
	  dbg_printf ("keyboard: read_key(): set_flg\n");	/* */
#endif /* notdef */
#ifdef notdef
	  set_flg (waitflag, 0);	/* ���٥�ȥե饰�򥯥ꥢ���� */
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

  /* ���������Ԥ� */
#ifdef notdef
  dbg_printf ("keyboard: waiting...\n");	/* */
#endif
  wai_flg (&rflag, waitflag, ONKEYS, TWF_ORW);
#ifdef notdef
  dbg_printf ("KEYBOARD: waked up by flag\n");
#endif
  goto retry;
}
