/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

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
/* malloc �ν�����
/*
 * Revision 1.4  1997/06/29 15:44:32  night
 * �����Ȥ��ѹ�
 *
 * �����������ɤ� SJIS ���� EUC ���ѹ�������
 * ��RCS �� Log �ޥ�������Υ����Ȥ��ɲä���(�ޤ�����Ƥ��ʤ��ä��ե�����Τ�)��
 *
 * Revision 1.3  1997/06/29 15:33:56  night
 * �����Ȥν��� (rcs �� Log �ޥ�����ɲ�)
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
  UW				size;		/* alloc_data �Υ����� */
						/* ������ */
  
  B				alloc_data[0];
};


static struct alloc_entry_t		alloc_list;
static struct alloc_entry_t		*free_list;
static struct alloc_entry_t		*pivot;

static VP			get_system_memory (UW size);


/* malloc �����ν����
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
      /* free ���륨��ȥ�Υ��ɥ쥹���������� */
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
      /* �ե꡼�ꥹ�Ȥ���Ƭ����é�ꡢ�ꥹ�Ȥ���������ݥ���Ȥ���� */
      /* �ե꡼�ꥹ�Ȥϥ��ɥ쥹��ˤʤäƤ��ꡢp �Υ��ɥ쥹���ɲä� */
      /* ���ΰ�κǸ�����礭���ʤä��顢���������������롣*/
      if (((p + p->size + sizeof (struct alloc_entry_t)) <= newentry) &&
	  ((newentry + newentry->size + sizeof (struct alloc_entry_t)) <= p->next) )
	{
	  if (((UW)p + p->size + sizeof (struct alloc_entry_t)) == (UW)newentry)
	    {
	      /* ���ߤΥ���ȥ���٤��äƤ���
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
	      /* ���Υ���ȥ���٤��äƤ���
	       */
	      newentry->size += (p->next->size + sizeof (struct alloc_entry_t));
	      newentry->before = p;
	      newentry->next = p->next->next;
	      p->next = newentry;
	      return;
	    }
	  else
	    {
	      /* ����Υ���ȥ�ȤϤɤ���Ȥ��٤��äƤ��ʤ� -
	       * ñ�˥ꥹ�ȤˤĤʤ���
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
	  /* �ꥹ�Ȥκǹ�̤Υ��ɥ쥹�ޤ��Ϥ���
	   * �ꥹ�ȤκǸ���ɲä���
	   */
	  newentry->before = p;
	  newentry->next = p->next;
	  p->next = newentry;
	  newentry->next->before = newentry;
	  return;
	}
    }
  /* ����������ʬ���ʤ��ä�
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


