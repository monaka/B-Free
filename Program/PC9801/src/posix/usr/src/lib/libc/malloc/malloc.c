/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* POSIX Library malloc.
 */

/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/lib/libc/malloc/malloc.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/lib/libc/malloc/malloc.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: malloc.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:52:12  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.2  1995/03/18  14:17:13  night
 * calloc(), free(), morecore() �ؿ����ɲá�
 *
 * Revision 1.1  1995/03/04  16:04:45  night
 * �ǽ����Ͽ
 *
 *
 */

 
#include <sys/types.h>
#include <native/syscall.h>
#include <sys/config.h>
#include <stdlib.h>


/*
 * �ҡ��ץ�������ؿ���
 *
 * �ץ����μ¹����ưŪ�˥�������/�������뤿��δؿ���
 *
 *
 * ���Υե�����ˤϰʲ��δؿ���¸�ߤ��Ƥ��롣
 *
 *	malloc	......	����μ����ؿ�
 *	calloc	......	����μ����ؿ� (malloc��ȤäƤ���)
 *	free    ......  malloc �Ǽ������������ΰ�β����ؿ�
 *	realloc ......  malloc �Ǽ������������ΰ�����Ƥ򤽤Τޤޤ�
 *			����������礹��ؿ�
 *
 * �ܥҡ��ץ�������ؿ����Ǥϡ��ե꡼����δ����Ȥ��ƥꥹ�Ȥˤ��
 * �ե������ȥե��å�ˡ��Ȥ���
 * ���ʤ����malloc �Ǽ�����������ΰ�ϥե꡼����ꥹ�Ȥ���ˤ���
 * �֥�å��Τ������ꥹ�Ȥ�é�ä��Ȥ��˺ǽ�ˤߤĤ��ä���ʬ���ΰ褬��
 * ���Τ�Ȥ���
 *
 * �ե꡼����֥�å��Υꥹ�Ȥ�������뤿��ˡ����� static �ѿ�����
 * �����롣
 *
 *	freelist	�ե꡼����֥�å��Υꥹ�ȡ�
 *	freepivot	���˼��������ե꡼����֥�å������Υ֥�å�
 *			��ؤ��ݥ��󥿡����Υݥ����ѿ��λؤ��֥�å�
 *			�μ��Υ֥�å�������������Ȥ��γ������Ȥ�
 *			�롣
 *
 *
 * �ƥե꡼����֥�å��ϡ�block ��¤�ΤǴ������롣
 *
 * 
 */

/*
 * �ե꡼����֥�å��Υꥹ�Ȥ�������뤿��ι�¤��
 *
 */
struct block
{
  struct block	*next;		/* ���Υ֥�å��򼨤��ݥ��� */
  size_t	size;		/* �֥�å��Υ�����	      */
  unsigned char	body[0];
};

typedef struct block block_t;

/*
 * block ��¤�Τ�Ȥ�����Υޥ���
 */
#define NEXTB(xp)	(((block_t *)(xp))->next)
#define SIZEB(xp)	(((block_t *)(xp))->size)


/*
 * ���Υե������������ǻ��Ѥ��� static �ѿ�
 */
static block_t		freelist;	/* �ե꡼����֥�å������ */
					/* ����ꥹ�Ȥ���Ƭ�֥�å��� */
					/* �ʤ��ѿ�(�֥�å��Ȥ��Ƥϡ� */
					/* �������� 0 �Τ��ᡢ���Ф˥� */
					/* �����Ȥ���ʤ���          */

static block_t		*freepivot;	/* ���μ�����������֥�å� */
					/* �����Υ֥�å���ؤ��ݥ��� */
					/* ���ѿ��������ѿ��λؤ��֥�� */
					/* ���μ��Υ֥�å�����ե꡼ */
					/* �ꥹ�Ȥ�é�롣             */

static int		initialized = 0;


/*
 * ����֥�å��Υ��饤���Ȥ��뤿��Υޥ���
 *
 * ������������Ȥ��ˤϡ������ɤ��˺��� 4 �Х��ȤΥ��饤���Ȥ˹�
 * �碌�롣
 */
#define ALIGN(x,align)	(((((int)x) + ((align) - 1))/(align))*(align))

/*
 * �Ǿ��Υ���֥�å��Υ�������
 */
#define MIN_FRAGMENT	(sizeof (block_t) * 32)


/*
 * ���Υե��������ǤΤ߻��Ѥ���ؿ� (static �ؿ�) �����
 */
static void	init_malloc (void);
static void	*malloc1 (size_t size);
static void	*morecore (size_t size);

/*
 * ������������ν������
 *
 * initialized �ѿ��� 0 �ΤȤ��� malloc/calloc ��Ƥ֤ȡ����δؿ���¹�
 * ���롣
 */
static void
init_malloc (void)
{
  freelist.size = 0;
  freepivot = freelist.next = &freelist;
}


/*
 * ���ꤷ���ΰ���������
 *
 */
void *
malloc (size_t size)
{
  /*
   * �����Υ����å�
   */
  if (size <= 0) return (NULL);

  /*
   * �����Υ������� MIN_FRAGMENT �ʲ��ξ��ˤϡ������������������� 
   * MIN_FRAGMENT �˹�碌�롣
   * MIN_FRAGMENT �����礭�����ˤϡ��֥�å��δ�������ʬ����������
   * �����䤹��
   */
  if (size < MIN_FRAGMENT)
    size = MIN_FRAGMENT;
  else
    size = ALIGN (size, 4);

  /*
   * �⤷��initialized �� 0 �ξ��ˤϥ�������������Τ��������롣
   */
  if (initialized == 0)
    init_malloc ();

  return (malloc1 (size));
}


/*
 * �ºݤ˥���֥�å������Τ���ν�����Ԥ��ؿ�
 *
 */
static void *
malloc1 (size_t size)
{
  block_t	*previous;
  block_t	*current;
  unsigned char	*p;

  /*
   * ��ʬ���礭�����Ķ�������֥�å��򸡺����롣
   */
  for (previous = freepivot, current = NEXTB (freepivot); 
       current != freepivot; 
       previous = current, current = NEXTB (current))
    {
      if (SIZEB (current) == size)
	{
	  /*
	   * ����֥�å����礭�������׵ᤷ�����������٥ԥå����äƤ�
	   * �롣
	   * �֥�å���ݤ��ȥꥹ�Ȥ��鳰�����֥�å��Υܥǥ��� (body) 
	   * �Υ��ɥ쥹���֤���
	   * ����֥�å���°������ˤĤ��Ƥϡ��������˻��Ѥ���Τ�
	   * ���Τޤޤˤ��Ƥ�����
	   * freepivot �˼�����������֥�å������Υ֥�å��Υ��ɥ�
	   * ��������롣
	   */
	  NEXTB (previous) = NEXTB (current);
	  freepivot = previous;
	  return ((void *)(current->body));
	}
      else if (SIZEB (current) > (size + MIN_FRAGMENT))
	{
	  /*
	   * ����֥�å����礭�������Ǿ��Υ����������礭����
	   * ����֥�å��θ�Ⱦ�������Ƥ롣
	   */
	  freepivot = previous;
	  p = (unsigned char *)current;
	  p = (p) + (SIZEB (current) - size);

	  /*
	   * �����ȥ֥�å��Υ֥�å����������ѹ����롣
	   */
	  SIZEB (current) -= size;

	  /*
	   * ���������֥�å��Υܥǥ������֤���
	   */
	  current = (block_t *)p;
	  NEXTB (current) = NULL;
	  SIZEB (current) = size;
	  return ((void *)(current->body));
	}
    }

  /*
   * ����֥�å�������Ǥ��ʤ��ä���
   * morecore() ��Ƥӡ��ҡ��ץ����ΰ���ĥ���롣
   * �����ơ����� malloc1 ��Ƥ�(�Ƶ���ȤäƤ���)��
   */
  if (morecore(size))
    {
      return (malloc1 (size));
    }

  /*
   * �ҡ����ΰ�γ���˼��Ԥ�����
   * ��������Ǥ��ʤ��Τ� NULL ���֤���
   */
  return ((void *)NULL);
}


/*
 * ���ꤵ�줿���������ĥ���֥�å�����ꤵ�줿�Ŀ������������롣
 * �ƥ���֥�å���Ϣ³���Ƥ��롣
 *
 * ������������֥�å��� NULL ���ꥢ����롣
 *
 */
void *
calloc (size_t nblock, size_t size)
{
  void *p;

  size = size * nblock;
  p = malloc (size);
  if (p != (void *)NULL)
    {
      bzero (p, size);
    }
  return (p);
}


/*
 * ����֥�å��γ����Ԥ���
 *
 * malloc() ��Ȥäơ���¸�Υ���֥�å�����ꤷ�����ꥵ�����˳���
 * ���롣
 * ���礷������֥�å��ˤϡ����Υ���֥�å������Ƥ����ԡ�����롣
 * ���Υ���֥�å��ϡ��ե꡼����֥�å��ꥹ�Ȥ��᤹��
 *
 */
void *
realloc (void *oldaddr, size_t newsize)
{
  void		*newaddr;
  block_t	*head;

  newaddr = malloc (newsize);
  bzero (newaddr, newsize);
  if (newaddr != NULL)
    {
      head = (block_t *) ((char *)oldaddr - sizeof (block_t));
      bcopy (oldaddr, newaddr, SIZEB (head));
      free ((void *)oldaddr);
    }
  return (newaddr);
}



/*
 * malloc �ˤƼ�����������֥�å���ե꡼����֥�å��ꥹ�Ȥ�
 * �ֵѤ��롣
 *
 * �����������֥�å��ϡ�������������Ȥ��Ƽ���3�Ĥ����롣
 *
 * 1) �ե꡼�ꥹ����Υ���֥�å��� 1 �Ĥ�ʻ�礹�롣
 *    (a) �����������֥�å������Υ֥�å���ʻ��
 *    (b) �����������֥�å��θ�Υ֥�å���ʻ��
 * 2) �ե꡼�ꥹ����Υ���֥�å��� 2 �Ĥ�ʻ�礹�롣
 * 3) ʻ�礷�ʤ��������������֥�å��ϡ�ñ�˥ե꡼�ꥹ�����Ŭ����
 *    ���֤��������롣
 *
 *
 * �����������֥�å�������(���뤤�ϡ��ե꡼�ꥹ����Υ���֥��
 * ����ʻ��)������֤ϡ����ξ�����������
 *
 * ��current �Υ��ɥ쥹�ϡ������������֥�å����⾮������
 * ��NEXTB(current) �ϡ������������֥�å������礭����
 *
 * �㳰�Ȥ��ơ������������֥�å����ե꡼�ꥹ����Υ���֥�å�
 * �Τɤ���⥢�ɥ쥹���礭����礬���롣
 * ���ξ��ˤϡ���������֥�å��ϡ��Ǥ⥢�ɥ쥹���ͤ��礭������
 * �֥�å��μ����������롣
 *
 * ���δؿ��ϡ��ͤ��֤��ʤ���
 */
void
free (void *addr)
{
  block_t	*current;
  block_t	*addblock;

  if (addr == NULL)
    {
      return;
    }

  /*
   * �Ϥ��줿���ɥ쥹�ϡ�����֥�å��Υܥǥ�������Ƭ���ɥ쥹��
   * ��������֥�å�����Ƭ���ɥ쥹����Ф���addblock �ˤ���
   * ���ɥ쥹������롣
   */
  (char *)addr -= sizeof (block_t);
  addblock = addr;

  /* 
   * �֥�å���ꥹ�ȤˤĤʤ������õ������γ����������
   */
  if (freepivot < addblock)
    {
      current = NEXTB (&freelist);
    }
  else
    {
      current = freepivot;
    }

  /*
   * �����������֥�å��������������õ����
   */
  while ((current < addblock) || (NEXTB (current) > addblock))
    {
      /* 
       * �ꥹ�Ȥ��ޤ��֤����ޤǤ�����
       */
      if (current > NEXTB (current))
	{
	  /*
	   * addblock �� current �����礭�����ˤϡ�
	   * �ե꡼�ꥹ�Ȥ���Υ��ɥ쥹�������礭������֥�å����
	   * �⥢�ɥ쥹���礭����
	   * addblock �� current �μ����������ƽ���롣
	   */
	  if (current < addblock)
	    {
	      NEXTB (addblock) = NEXTB (current);
	      NEXTB (current) = addblock;
	      return;
	    }
	}
      current = NEXTB (current);
    }

  /*
   * �����������֥�å��ϡ�addblock �����ܤ��Ƥ��롣
   * ����֥�å���ʻ�礹�롣
   */
  if (((char *)current + SIZEB(current)) == (char *)addblock)
    {
      SIZEB(current) += SIZEB (addblock);
      addblock = current;
    }

  /*
   * NEXTB (curernt) �Υ���֥�å��ϡ�addblock �����ܤ��Ƥ��롣
   * ����֥�å���ʻ�礹�롣
   *
   * current �� addblock �����ܤ��Ƥ������Ǥ⤳�Υ����å��ϹԤ���
   * ���ξ�硢current, NEXTB(current), addblock �ϤҤȤĤΥ֥�å���
   * �ޤȤ�롣
   */
  if ((char *)addblock + SIZEB (NEXTB (current)))
    {
      NEXTB(addblock) = NEXTB (NEXTB (current));
      SIZEB(addblock) += SIZEB (NEXTB (current));

      /*
       * NEXTB (current) �� addblock �Υ��ɥ쥹������롣
       */
      if (current != addblock)
	{
	  NEXTB (current) = addblock;
	}
      return;
    }

  /*
   * �����������֥�å��򡢿���������֥�å��Ȥ��ƥե꡼�ꥹ��
   * ����Ͽ���롣
   */
  NEXTB (addblock) = NEXTB (current);
  NEXTB (current) = addblock;
  return;
}

/*
 * ����������֥�å����롣
 *
 * ����������֥�å��ѤΥ���򥷥��ƥफ����������ե꡼�����
 * ���Ȥ��ɲä��롣
 *
 * ���δؿ��ϡ��֤��ͤȤ����ɲä�������֥�å��Υ��ɥ쥹���֤���
 * 
 */
static void *
morecore (size_t grow_size)
     /*
      * grow_size	���礷��������Υ�������
      */
{
  block_t	*newblock;

  grow_size = ALIGN (grow_size, PAGE_SIZE);

  /*
   * OS ���鿷��������֥�å����餦��
   * B-Free/POSIX �ξ��ˤϡ�growheap �����ƥॳ�������Ѥ��롣
   */
  if (growheap (grow_size, &newblock) != 0)
    {
      /* 
       * ���꤬�����Ǥ��ʤ��ä���
       * NULL ��ƤӽФ������֤���
       */
      return (NULL);
    }

  SIZEB (newblock) = grow_size;
  NEXTB (newblock) = (block_t *)NULL;

  /*
   * �ե꡼�֥�å��Υꥹ�Ȥˡ�������������������֥�å����ɲä��롣
   */
  free (newblock);

  /*
   * ���� malloc() ��¹Ԥ���Ȥ��ˤϡ�newblock ���鸡������褦��
   * ���Ƥ�����
   */
  freepivot = newblock;

  /*
   * ������������֥�å����֤���
   */
  return (newblock);
}
