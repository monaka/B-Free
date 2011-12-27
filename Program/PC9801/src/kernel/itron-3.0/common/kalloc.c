/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* kalloc.c

  �����ͥ�����ꥢ�����ȴؿ���
 
  Interface:
 
 	kalloc()	���ꤷ��������ʬ�����Υ����ΰ�򥢥����Ȥ���
			��Ƭ���ɥ쥹���֤���

 	kfree()		kalloc() �ǥ������Ȥ����ΰ��������롣
 
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


/* getcore --- ������kalloc��������������ΰ���ɲä��롣
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

/* �������������ΰ��ͳ�ΰ���ɲä���
 */
  kfree (newp, ROUNDUP (size, CLICK_SIZE));
  return (E_OK);
}


/* init_kalloc --- kalloc �Τ���Υե꡼�ꥹ�Ȥν����
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


/* kalloc --- �����ΰ�򿷤����ɲä��롣
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
	  /* �����������٤ξ��; �ե꡼�ꥹ�Ȥ��饨��ȥ�������� */
	  before->next = front->next;
	  break;
	}
      else if (front->size >= (size + sizeof (struct kmem_entry)))
	{
	  /* ���������礭�����; front �λؤ�����ȥ꤫���Ⱦʬ���� */
	  front->size -= size;
	  (B *)front = (B *)front + (front->size);
	  break;
	}
      pivot = front;
    }

  /* ����ȥ꤬���Ĥ���ʤ��ä�; �ڡ����򥢥����Ȥ��롣
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

/* kfree --- ���ꤷ�����ꥢ��ե꡼����ꥹ�Ȥ��ɲä��롣
 *
 */
void
kfree (VP ap, W size)
{
  struct kmem_entry *p, *before;
  struct kmem_entry *area;

  area = (struct kmem_entry *)ap;
  area->size = size;

  /* �ե꡼�ꥹ�Ȥκǽ餫��Ǹ�ޤǤ�õ�����롣
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
      /* �ΰ�����Τ�Τ����礹�� */

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




