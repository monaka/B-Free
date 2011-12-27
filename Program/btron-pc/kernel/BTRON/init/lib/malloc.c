/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

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
 * ROUNDUP �ޥ�������������ɤ������ɤ��ɲá�
 *
 * Revision 1.1  1996/07/25  16:03:44  night
 * IBM PC �Ǥؤκǽ����Ͽ
 *
 *
 */

#include "../init.h"


#ifndef ROUNDUP
#define ROUNDUP(x,align)	(((((W)x) + ((align) - 1))/(align))*(align))
#endif
#define TRUE_SIZE(size)	(size + sizeof (struct malloc_entry))

#define CLICK_SIZE	(PAGE_SIZE * 2)

#define MIN_SIZE	(CLICK_SIZE * 10)	/* �ҡ����ΰ�ν�������� */
#define MAX_SIZE	(1 * 1024 * 1024)	/* �ҡ����ΰ�Ϻ��� 1M */


struct malloc_entry
{
  struct malloc_entry	*next;
  W			size;
  B			body[0];
};

static struct malloc_entry	*malloc_start, *last_page;
static struct malloc_entry	*free_list, *current_entry;



/*
 * �ե꡼��������ν����
 *
 */
init_malloc (void)
{
  extern B		end;
  ER			err;
  ID			mytask;
  struct malloc_entry	*p;

  /*
   * �ҡ����ΰ����������
   */
  last_page = malloc_start = (void *)ROUNDUP (&end, PAGE_SIZE);

  /*
   * ITRON ���Ф��ƥ꡼�������������褦�׵᤹��
   */
#ifdef notdef
  printf ("vcre_reg(%d, %x, %d, %d, %d, %d)\n",
	  get_tid (), last_page, MIN_SIZE, MAX_SIZE, 0, NULL);
#endif
  get_tid (&mytask);
  err = vcre_reg (mytask, last_page, MIN_SIZE, MAX_SIZE, 0, NULL);
  if (err != E_OK)
    {
      /* ���� */
#ifdef notdef
      printf ("vcre_reg: fail. error = %d\n", err);	/* */
#endif
      return (err);
    }

#ifdef notdef
  printf ("vcre_reg: OK\n");	/* */
#endif

  /*
   * ʪ�������ޥåԥ󥰤���
   */
  err = vmap_reg (mytask, last_page, MIN_SIZE);
  if (err != E_OK)
    {
      /* ���� */
#ifdef notdef
      printf ("vmap_reg: fail. error = %d\n", err);	/* */
#endif
      return (err);
    }

#ifdef notdef
  printf ("vmap_reg: OK\n");	/* */
#endif

  /*
   * �ե꡼�ꥹ�ȤˤĤʤ���
   */
  p = last_page;
  p->size = MIN_SIZE;
  p->next = p;

  last_page += MIN_SIZE;

  current_entry = p;

  return (E_OK);
}

/*
 * ����򥷥��ƥफ���������
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
   * ʪ�������ޥåԥ󥰤���
   */
  get_tid (&mytask);
  err = vmap_reg (mytask, last_page, MIN_SIZE);
  if (err != E_OK)
    {
      /* ���� */
      return (NULL);
    }

  p = last_page;
  last_page += CLICK_SIZE;

  return (p);
}


void *
malloc (W size)
{
  W		true_size;	/* �������ΰ��ޤ��"����"�������� */
  				/* ������ */
  struct malloc_entry	*p, *prev;
  struct malloc_entry	*alloced;


  if (current_entry == NULL)	/* �������٤��ե꡼���꤬�ʤ� */
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
      if (p == current_entry)	/* �����Ǥ��륨��ȥ꤬�ʤ��ä� */
	{
	  /* �ҡ����ΰ����礹�� */
	  p = more_core (CLICK_SIZE);

	  /* �ե꡼�ꥹ�Ȥ���Ͽ���� */
	}
    }

  if (p->size == true_size)	/* ���٤Υ��������ä� */
    {
      if (p->next == p)	/* �Ĥʤ��äƤ��륨��ȥ�ϤҤȤĤ������ä� */
	{
	  current_entry == NULL;
	  p->next = NULL;
	}
      else
	{
	  /* �ե꡼�ꥹ�Ȥ��� p �ǻ��ꤵ��Ƥ��륨��ȥ�򳰤� */
	  prev->next = p->next;
	}
      
      return ((void *)(p->body));	/* ������������ؤΥݥ��� */
					/* ���֤������������ΰ�ϡ� */
					/* malloc_start ��¤�Τ� body �� */
					/* �ǤǤ��뤳�Ȥ���� */
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

  /* ��������ꤵ�줿�ΰ�� malloc �Ǵ������Ƥ����ΰ�ǤϤʤ�. */
  if (((B *)new_entry < (B *)(void *)malloc_start)
      || ((B *)new_entry > (B *)(void *)malloc_start + (1024 * 1024)))
    {
      return;
    }

  /* �ե꡼�ꥹ�Ȥ˥���ȥ�ϤҤȤĤ����ʤ� */
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

  /* �ե꡼�ꥹ�Ȥ���Ƭ����é�ꡢ�ꥹ�Ȥ���������ݥ���Ȥ���� */
  /* �ե꡼�ꥹ�Ȥϥ��ɥ쥹��ˤʤäƤ��ꡢcurrent �Υ��ɥ쥹���ɲä� */
  /* ���ΰ�κǸ�����礭���ʤä��顢���������������롣*/
  for (prev = current_entry, current = current_entry->next;
       current != current_entry;
       prev = current, current = current->next)
    {
      /* current �����ܤ��Ƥ��롣current ���ΰ��Ĥʤ��� */
      if ((B *)current == ((B *)new_entry + new_entry->size))
	{
	  new_entry->size += current->size;
	  new_entry->next = current->next;
	  current = new_entry;
	  prev->next = current;
	}
      else
	{
	  /* current �ˤ����ܤ��Ƥ��ʤ����ꥹ�Ȥ��������� */
	  new_entry->next = current;
	  prev->next = new_entry;
	}
      
      /* prev �����ܤ��Ƥ����硣prev �Υ����������䤹��*/
      /* ���ΤȤ���current ��ʻ�礷�Ƥ��褦����ñ���������Ƥ��褦���� */
      /* �ɤ���ˤ��Ƥ� prev->next �ϡ�new_entry��ؤ��Ƥ��뤳�Ȥ���� */
      if ((B *)new_entry == ((B *)prev + prev->size))
	{
	  prev->size += new_entry->size;
	  prev->next = new_entry->next;
	}
    }
}


