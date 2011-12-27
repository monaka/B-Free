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


void
print_memory_list (void)
{
  struct kmem_entry	*p;

  printk ("ADDR: 0x%x    SIZE: %d    NEXT: 0x%x\n",
	  &freelist, freelist.size, freelist.next);
  for (p = freelist.next; p != &freelist; p = p->next)
    {
      printk ("ADDR: 0x%x    SIZE: %d    NEXT: 0x%x\n",
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
  kfree (newp->body, ROUNDUP (size, CLICK_SIZE) - sizeof(struct kmem_entry));
  return (E_OK);
}


/* init_kalloc --- kalloc のためのフリーリストの初期化
 *
 */
void
init_kalloc (void)
{
#if 0
  T_CMPL flag;
#endif

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
	  front->size -= size + sizeof (struct kmem_entry);
	  (UW )front = ((UW) front->body) + (front->size);
	  front->size = size;
	  break;
	}
      before = front;
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
  return (front->body);
}

/* kfree --- 指定したエリアをフリーメモリリストに追加する。
 *
 */
void
kfree (VP ap, W size)
{
  struct kmem_entry *p, *before;
  struct kmem_entry *area;

  area = (struct kmem_entry *)(((UW) ap) - sizeof(struct kmem_entry)) ;
  area->size = size;

  /* フリーリストの最初から最後までを探索する。
   */
  before = &freelist;
  p = freelist.next;

  if (before == p) {
    before->next = area;
    area->next = before;
    return;
  }

#ifdef notdef
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
#else
  if (area > pivot) before = pivot;
  p = before;
  while(1) {
    /* フリーリストを先頭から辿り、リストに挿入するポイントを決める */
    /* フリーリストはアドレス順になっており、p のアドレスが追加す */
    /* る領域の最後よりも大きくなったら、その前に挿入する。*/
    if ((((UW)p->body) + p->size <= (UW)area) &&
	((p->next < p) || ((((UW)area->body) + area->size) <= (UW)p->next))) {
      if ((((UW)p->body) + p->size) == (UW)area) {
	/* 現在のエントリと隣り合っている
	 */
	p->size += (area->size + sizeof (struct kmem_entry));
	if ((((UW)p->body) + p->size) == (UW)p->next) {
	  p->size += (p->next->size + sizeof (struct kmem_entry));
	  p->next = p->next->next;
	}
	return;
      }
      else if ((((UW)area->body) + area->size) == (UW)(p->next)) {
	  /* 次のエントリと隣り合っている */
	  area->size += (p->next->size + sizeof (struct kmem_entry));
	  area->next = p->next->next;
	  p->next = area;
	  return;
      }
      else {
	/* 前後のエントリとはどちらとも隣り合っていない -
	 * 単にリストにつなげる
	 */
	area->next = p->next;
	p->next = area;
	return;
      }
    }
    p = p->next; 
    if (p == before) break;
  }	/* for */
  printk("kfree: missing. %x:%d \n",
	 area, area->size+sizeof(struct kmem_entry));
#endif
}
