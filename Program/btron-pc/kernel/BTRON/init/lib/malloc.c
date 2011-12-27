/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* $Id: malloc.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "$Id: malloc.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: malloc.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1997-09-10 13:10:08  night
 * ROUNDUP マクロの二重定義を防ぐコードを追加。
 *
 * Revision 1.1  1996/07/25  16:03:44  night
 * IBM PC 版への最初の登録
 *
 *
 */

#include "../init.h"


#ifndef ROUNDUP
#define ROUNDUP(x,align)	(((((W)x) + ((align) - 1))/(align))*(align))
#endif
#define TRUE_SIZE(size)	(size + sizeof (struct malloc_entry))

#define CLICK_SIZE	(PAGE_SIZE * 2)

#define MIN_SIZE	(CLICK_SIZE * 10)	/* ヒープ領域の初期サイズ */
#define MAX_SIZE	(1 * 1024 * 1024)	/* ヒープ領域は最大 1M */


struct malloc_entry
{
  struct malloc_entry	*next;
  W			size;
  B			body[0];
};

static struct malloc_entry	*malloc_start, *last_page;
static struct malloc_entry	*free_list, *current_entry;



/*
 * フリーメモリ管理の初期化
 *
 */
init_malloc (void)
{
  extern B		end;
  ER			err;
  ID			mytask;
  struct malloc_entry	*p;

  /*
   * ヒープ領域を生成する
   */
  last_page = malloc_start = (void *)ROUNDUP (&end, PAGE_SIZE);

  /*
   * ITRON に対してリージョンを作成するよう要求する
   */
#ifdef notdef
  printf ("vcre_reg(%d, %x, %d, %d, %d, %d)\n",
	  get_tid (), last_page, MIN_SIZE, MAX_SIZE, 0, NULL);
#endif
  get_tid (&mytask);
  err = vcre_reg (mytask, last_page, MIN_SIZE, MAX_SIZE, 0, NULL);
  if (err != E_OK)
    {
      /* 失敗 */
#ifdef notdef
      printf ("vcre_reg: fail. error = %d\n", err);	/* */
#endif
      return (err);
    }

#ifdef notdef
  printf ("vcre_reg: OK\n");	/* */
#endif

  /*
   * 物理メモリをマッピングする
   */
  err = vmap_reg (mytask, last_page, MIN_SIZE);
  if (err != E_OK)
    {
      /* 失敗 */
#ifdef notdef
      printf ("vmap_reg: fail. error = %d\n", err);	/* */
#endif
      return (err);
    }

#ifdef notdef
  printf ("vmap_reg: OK\n");	/* */
#endif

  /*
   * フリーリストにつなげる
   */
  p = last_page;
  p->size = MIN_SIZE;
  p->next = p;

  last_page += MIN_SIZE;

  current_entry = p;

  return (E_OK);
}

/*
 * メモリをシステムから取得する
 *
 */
static void *
more_core (W size)
{
  void	*p;
  ER	err;
  ID	mytask;

  if ((UW)last_page + size - (UW)malloc_start >= MAX_SIZE)
    {
      return (NULL);
    }

  /*
   * 物理メモリをマッピングする
   */
  get_tid (&mytask);
  err = vmap_reg (mytask, last_page, MIN_SIZE);
  if (err != E_OK)
    {
      /* 失敗 */
      return (NULL);
    }

  p = last_page;
  last_page += CLICK_SIZE;

  return (p);
}


void *
malloc (W size)
{
  W		true_size;	/* 管理用領域も含んだ"真の"アロケート */
  				/* サイズ */
  struct malloc_entry	*p, *prev;
  struct malloc_entry	*alloced;


  if (current_entry == NULL)	/* 取得すべきフリーメモリがない */
    {
      return (NULL);
    }

  true_size = TRUE_SIZE (size);

#ifdef notdef
  printf ("current_entry = 0x%x, current_entry->next = 0x%x, size = %d\n", 
	  current_entry,
	  current_entry->next,
	  current_entry->size); /* */
#endif

  for (prev = current_entry, p = current_entry->next;
       p->size < true_size;
       prev = p, p = p->next)
    {
      if (p == current_entry)	/* 取得できるエントリがなかった */
	{
	  /* ヒープ領域を拡大する */
	  p = more_core (CLICK_SIZE);

	  /* フリーリストへ登録する */
	}
    }

  if (p->size == true_size)	/* 丁度のサイズだった */
    {
      if (p->next == p)	/* つながっているエントリはひとつだけだった */
	{
	  current_entry == NULL;
	  p->next = NULL;
	}
      else
	{
	  /* フリーリストから p で指定されているエントリを外す */
	  prev->next = p->next;
	}
      
      return ((void *)(p->body));	/* 取得したメモリへのポインタ */
					/* を返す。取得した領域は、 */
					/* malloc_start 構造体の body 要 */
					/* 素であることに注意 */
    }

  (B *)alloced = ((B *)p) + (p->size - true_size);
  p->size -= true_size;
  alloced->size = true_size;
  return (alloced->body);
}


void *
realloc (void *buf, W size)
{
}


void
free (void *ptr)
{
  struct malloc_entry	*current, *prev;
  struct malloc_entry	*new_entry;

  new_entry = (struct malloc_entry *)((B *)ptr - sizeof (struct malloc_entry));

  /* 挿入を指定された領域は malloc で管理している領域ではない. */
  if (((B *)new_entry < (B *)(void *)malloc_start)
      || ((B *)new_entry > (B *)(void *)malloc_start + (1024 * 1024)))
    {
      return;
    }

  /* フリーリストにエントリはひとつしかない */
  if (current_entry == current_entry->next)
    {
      if (((B *)new_entry + new_entry->size) == (B *)current_entry)
	{
	  new_entry->size += current_entry->size;
	  new_entry->next = new_entry;
	  current_entry = new_entry;
	}
      else if (((B *)current_entry + current_entry->size) == (B *)new_entry)
	{
	  current_entry->size += new_entry->size;
	}
      else
	{
	  current_entry->next = new_entry;
	  new_entry->next = current_entry;
	}
      return;
    }

  /* フリーリストを先頭から辿り、リストに挿入するポイントを決める */
  /* フリーリストはアドレス順になっており、current のアドレスが追加す */
  /* る領域の最後よりも大きくなったら、その前に挿入する。*/
  for (prev = current_entry, current = current_entry->next;
       current != current_entry;
       prev = current, current = current->next)
    {
      /* current に隣接している。current と領域をつなげる */
      if ((B *)current == ((B *)new_entry + new_entry->size))
	{
	  new_entry->size += current->size;
	  new_entry->next = current->next;
	  current = new_entry;
	  prev->next = current;
	}
      else
	{
	  /* current には隣接していない。リストに挿入する */
	  new_entry->next = current;
	  prev->next = new_entry;
	}
      
      /* prev に隣接している場合。prev のサイズを増やす。*/
      /* このとき、current と併合していようが、単に挿入していようが、 */
      /* どちらにしても prev->next は、new_entryを指していることに注意 */
      if ((B *)new_entry == ((B *)prev + prev->size))
	{
	  prev->size += new_entry->size;
	  prev->next = new_entry->next;
	}
    }
}


