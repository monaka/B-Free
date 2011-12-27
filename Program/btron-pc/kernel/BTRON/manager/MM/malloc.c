/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/manager/MM/malloc.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

static char rcs[] = "@(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/manager/MM/malloc.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/* $Log: malloc.c,v $
/* Revision 1.1  2011/12/27 17:13:35  liu1
/* Initial Version.
/*
/* Revision 1.5  1997-07-02 13:10:32  night
/* malloc の修正。
/*
 * Revision 1.4  1997/06/29 15:44:32  night
 * コメントの変更
 *
 * ・漢字コードを SJIS から EUC に変更した。
 * ・RCS の Log マクロ入りのコメントを追加した(まだ入れていなかったファイルのみ)。
 *
 * Revision 1.3  1997/06/29 15:33:56  night
 * コメントの修正 (rcs の Log マクロの追加)
 *
 *
 */


#include "memory_manager.h"


#define MEMORY_CLICK		(1024 * 10)
#define MALLOC_SIZE		(1024 * 1024 * 10)	/* 10MB */


extern B	_end;
UW		last_page;

struct alloc_entry_t
{
  struct alloc_entry_t		*next;
  struct alloc_entry_t		*before;
  UW				size;		/* alloc_data のサイズ */
						/* が入る */
  
  B				alloc_data[0];
};


static struct alloc_entry_t		alloc_list;
static struct alloc_entry_t		*free_list;
static struct alloc_entry_t		*pivot;

static VP			get_system_memory (UW size);


/* malloc 機構の初期化
 *
 */
ER
init_malloc (void)
{
  ID			mytid;
  struct alloc_entry_t	*p;
  ER			err;


  last_page = (UW)&_end;
  last_page = ROUNDUP (last_page, PAGE_SIZE);

  err = get_tid (&mytid);
  if (err != E_OK)
    {
      return (err);
    }

  err = vcre_reg (mytid, last_page, MALLOC_SIZE, MALLOC_SIZE, VPROT_READ | VPROT_READ | VPROT_USER, NULL);
  if (err)
    {
      printf ("MM: cannot vcre_reg: errno = %d\n", err);
      return (err);
    }

  alloc_list.size = 0;
  p = (struct alloc_entry_t *)get_system_memory (MEMORY_CLICK);
  if (p == NULL)
    {
      return (E_NOMEM);
    }

  p->size = MEMORY_CLICK - sizeof (struct alloc_entry_t);
  p->next = &alloc_list;
  p->before = &alloc_list;
  alloc_list.next = p;
  alloc_list.before = p;
  free_list = &alloc_list;
  pivot = &alloc_list;
  return (E_OK);
}


VP
malloc (UW size)
{
  struct alloc_entry_t	*p;
  struct alloc_entry_t	*q;

  if (size <= 0)
    {
      return (NULL);
    }

  for (p = pivot->next; p != pivot; p = p->next)
    {
      if (p->size == size)
	{
	  pivot = p->next;
	  p->before->next = p->next;
	  p->next->before = p->before;
	  return (p);
	}
      else if (p->size > (size + sizeof (struct alloc_entry_t)))
	{
	  pivot = p->next;
	  p->size =- (size + sizeof (struct alloc_entry_t));
	  return (&(p->alloc_data[p->size + sizeof (struct alloc_entry_t)]));
	}
    }

  return (NULL);
}


VP
calloc (UW size, UW nentry)
{
  VP	rp;

  rp = malloc (size * nentry);
  if (rp)
    {
      bzero (rp, size * nentry);
    }

  return (rp);
}


void
free (VP addr)
{
  struct alloc_entry_t	*start_point;
  struct alloc_entry_t	*p;
  struct alloc_entry_t  *newentry;

  newentry = (struct alloc_entry_t *)((B *)addr - sizeof (struct alloc_entry_t));
  if ((newentry > (struct alloc_entry_t *)last_page) || (newentry < &alloc_list))
    {
      /* free するエントリのアドレスがおかしい */
      return;
    }

  if (&alloc_list == alloc_list.next)
    {
      alloc_list.next = newentry;
      alloc_list.before = newentry;
      newentry->next = newentry->before = &alloc_list;
      return;
    }

  start_point = (struct alloc_entry_t *)(((UW)pivot > (UW)addr) ? &alloc_list : pivot);

  for (p = start_point; p != start_point->before; p = p->next)
    {
      /* フリーリストを先頭から辿り、リストに挿入するポイントを決める */
      /* フリーリストはアドレス順になっており、p のアドレスが追加す */
      /* る領域の最後よりも大きくなったら、その前に挿入する。*/
      if (((p + p->size + sizeof (struct alloc_entry_t)) <= newentry) &&
	  ((newentry + newentry->size + sizeof (struct alloc_entry_t)) <= p->next) )
	{
	  if (((UW)p + p->size + sizeof (struct alloc_entry_t)) == (UW)newentry)
	    {
	      /* 現在のエントリと隣り合っている
	       */
	      p->size += (newentry->size + sizeof (struct alloc_entry_t));
	      if (((UW)p + p->size + sizeof (struct alloc_entry_t)) == (UW)p->next)
		{
		  p->size += (p->next->size + sizeof (struct alloc_entry_t));
		  p->next->next->before = p;
		  p->next = p->next->next;
		}
	      return;
	    }
	  else if ((((UW)newentry) + newentry->size + sizeof (struct alloc_entry_t)) == (UW)(p->next))
	    {
	      /* 次のエントリと隣り合っている
	       */
	      newentry->size += (p->next->size + sizeof (struct alloc_entry_t));
	      newentry->before = p;
	      newentry->next = p->next->next;
	      p->next = newentry;
	      return;
	    }
	  else
	    {
	      /* 前後のエントリとはどちらとも隣り合っていない -
	       * 単にリストにつなげる
	       */
	      newentry->before = p;
	      newentry->next = p->next;
	      p->next = newentry;
	      newentry->next->before = newentry;
	      return;
	    }
	}
      else if ((((UW)p + p->size + sizeof (struct alloc_entry_t)) <= (UW)newentry) &&
	       (p > p->next))
	{
	  /* リストの最高位のアドレスまで届いた
	   * リストの最後に追加する
	   */
	  newentry->before = p;
	  newentry->next = p->next;
	  p->next = newentry;
	  newentry->next->before = newentry;
	  return;
	}
    }
  /* 該当する部分がなかった
   */
  
}





static VP
get_system_memory (UW size)
{
  ID	mytid;
  VP	rp;

  if (get_tid (&mytid) != E_OK)
    {
      return (NULL);
    }

  printf ("get_system_memory: start = 0x%x, size = %d\n", last_page, size);
  if (vmap_reg (mytid, last_page, size))
    {
      return (NULL);
    }

  rp = (VP)last_page;
  last_page += size;
  return (rp);
}


