/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* kalloc.c

  カーネル内メモリアロケート関数群
 
  Interface:
 
 	kalloc()	指定したサイズ分だけのメモリ領域をアロケートし、
			先頭アドレスを返す。

 	kfree()		kalloc() でアロケートした領域を解放する。
 
 */

#include "itron.h"
#include "errno.h"
#include "misc.h"
#include "func.h"


#define CLICK_SIZE	(10 * PAGE_SIZE)

struct kmem_entry
{
  struct kmem_entry	*next;
  W			size;
  B			body[0];
};

static struct kmem_entry	freelist;
static struct kmem_entry	*pivot;

#ifdef MEMORY_TEST
#define palloc	malloc

main ()
{
  int	size;

  init_kalloc ();
  print_memory_list ();
  kalloc (10);
  print_memory_list ();
}
#endif /* MEMORY_TEST */


print_memory_list (void)
{
  struct kmem_entry	*p;

  printf ("ADDR: 0x%x    SIZE: %d    NEXT: 0x%x\n",
	  &freelist, freelist.size, freelist.next);
  for (p = freelist.next; p != &freelist; p = p->next)
    {
      printf ("ADDR: 0x%x    SIZE: %d    NEXT: 0x%x\n",
	      p, p->size, p->next);
    }
}


/* getcore --- 新たにkallocが管理するメモリ領域を追加する。
 */
static ER
getcore(W size)
{
  struct kmem_entry *newp;

  newp = (struct kmem_entry *)palloc (ROUNDUP (size, CLICK_SIZE) / PAGE_SIZE);
  if (newp == NULL)
    {
      return (E_NOMEM);
    }

/* 取得したメモリ領域を自由領域に追加する
 */
  kfree (newp, ROUNDUP (size, CLICK_SIZE));
  return (E_OK);
}


/* init_kalloc --- kalloc のためのフリーリストの初期化
 *
 */
void
init_kalloc (void)
{
  T_CMPL flag;

  pivot = &freelist;
  freelist.size = 0;
  freelist.next = &freelist;
}


/* kalloc --- メモリ領域を新たに追加する。
 *
 */
VP
kalloc (W size)
{
  struct kmem_entry	*front, *before;

retry:
  /* search freelist */
  for (before = pivot, front = pivot->next;
       front != pivot;
       front = front->next)
    {
      if (front->size == size)
	{
	  /* サイズが丁度の場合; フリーリストからエントリを削除する */
	  before->next = front->next;
	  break;
	}
      else if (front->size >= (size + sizeof (struct kmem_entry)))
	{
	  /* サイズが大きい場合; front の指すエントリから後半分を削る */
	  front->size -= size;
	  (B *)front = (B *)front + (front->size);
	  break;
	}
      pivot = front;
    }

  /* エントリが見つからなかった; ページをアロケートする。
   */
  if (front == pivot)
    {
      if (getcore (size) != E_OK)
	return (NULL);
      goto retry;
    }
  pivot = before;
  return (front);
}

/* kfree --- 指定したエリアをフリーメモリリストに追加する。
 *
 */
void
kfree (VP ap, W size)
{
  struct kmem_entry *p, *before;
  struct kmem_entry *area;

  area = (struct kmem_entry *)ap;
  area->size = size;

  /* フリーリストの最初から最後までを探索する。
   */
  before = &freelist;
  p = freelist.next;
  while ((p < area) && (p != &freelist))
    {
      before = p;
      p = before->next;
    }

  if ((B *)before + before->size == (B *)area)
    {
      area = before;
      before->size += size;
    }
  else
    {
      area->size = size;
      before->next = area;
    }

  if ((B *)area + size == (B *)p)
    {
      /* 領域を前のものと統合する */

      area->next = p->next;
      area->size += p->size;
      if (pivot == p)
	{
	  pivot = area;
	}
    }
  else
    {
      area->next = p;
    }
}




