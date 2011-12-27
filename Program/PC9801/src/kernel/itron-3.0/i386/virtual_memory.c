/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/itron-3.0/i386/virtual_memory.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/itron-3.0/i386/virtual_memory.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

/*
 * $Log: virtual_memory.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.12  1996-01-06 16:08:03  night
 * �ƤӽФ��ؿ�̾���ְ�äƤ����Τ�������ľ������
 *
 * Revision 1.11  1995/12/13 15:02:23  night
 * vmap_reg() �ؿ�����Ȥ����������
 *
 * Revision 1.10  1995/09/21  15:51:29  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.9  1995/09/17  17:00:12  night
 * ;ʬ�� printf () �� #ifdef notdef ... #endif �ǰϤ����
 *
 * Revision 1.8  1995/09/14  04:33:09  night
 * ���ɥ쥹�ޥ����Ѥ�������ͤ��ѹ���
 *
 * Revision 1.7  1995/05/31  22:58:00  night
 * �����Ĥ��� #ifdef DEBUG ... #endif ���ɲá�
 * (;ʬ�ʥǥХå��� printf ʸ���Υ����)
 *
 * Revision 1.6  1995/03/18  14:50:11  night
 * vcre_reg() �ؿ��򥳥�ѥ���Ǥ���褦�˽�����
 *
 * Revision 1.5  1995/02/26  14:07:40  night
 * RCS �ޥ��� $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/itron-3.0/i386/virtual_memory.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ �� $Log: virtual_memory.c,v $
 * RCS �ޥ��� $Header: /home/night/CVS/B-Free/Program/PC9801/src/kernel/itron-3.0/i386/virtual_memory.c,v 1.12 1996-01-06 16:08:03 night Exp $ �� Revision 1.1  2011/12/27 17:13:36  liu1
 * RCS �ޥ��� $Header: /home/night/CVS/B-Free/Program/PC9801/src/kernel/itron-3.0/i386/virtual_memory.c,v 1.12 1996-01-06 16:08:03 night Exp $ �� Initial Version.
 * RCS �ޥ��� $Header: /home/night/CVS/B-Free/Program/PC9801/src/kernel/itron-3.0/i386/virtual_memory.c,v 1.12 1996-01-06 16:08:03 night Exp $ ��
 * RCS �ޥ��� $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/itron-3.0/i386/virtual_memory.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ �� Revision 1.11  1995/12/13 15:02:23  night
 * RCS �ޥ��� $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/itron-3.0/i386/virtual_memory.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ �� vmap_reg() �ؿ�����Ȥ����������
 * RCS �ޥ��� $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/itron-3.0/i386/virtual_memory.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ ��
 * Revision 1.10  1995/09/21  15:51:29  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.9  1995/09/17  17:00:12  night
 * ;ʬ�� printf () �� #ifdef notdef ... #endif �ǰϤ����
 *
 * Revision 1.8  1995/09/14  04:33:09  night
 * ���ɥ쥹�ޥ����Ѥ�������ͤ��ѹ���
 *
 * Revision 1.7  1995/05/31  22:58:00  night
 * �����Ĥ��� #ifdef DEBUG ... #endif ���ɲá�
 * (;ʬ�ʥǥХå��� printf ʸ���Υ����)
 *
 * Revision 1.6  1995/03/18  14:50:11  night
 * vcre_reg() �ؿ��򥳥�ѥ���Ǥ���褦�˽�����
 * ���ɲá�
 *
 *
 */

/* virtual_memory.c
 *	
 *
 */

#include "i386.h"
#include "itron.h"
#include "misc.h"
#include "func.h"
#include "task.h"
#include "errno.h"
#include "region.h"

static I386_PAGE_ENTRY *alloc_pagetable (void);

/* #define DEBUG	1	/* */


/* dup_vmap_table --- ���ꤵ�줿���ۥ���Υޥåԥ󥰥ơ��֥��
 *		      ���ԡ����롣
 *		      �ޥåץơ��֥뼫�ΤϿ������������롣
 *
 */
ADDR_MAP 
dup_vmap_table (ADDR_MAP dest)
{
  ADDR_MAP	newp;
  int		i;
  I386_PAGE_ENTRY	*p;


  (UW)dest = (UW)dest | 0x80000000;
  newp = (ADDR_MAP)(palloc (1));	/* �ڡ����ǥ��쥯�ȥ�Υ������� */
  for (i = 0; i < ADDR_MAP_SIZE; i++)
    {
      newp[i] = dest[i];		/* �ڡ����ǥ��쥯�ȥ�򣱥���ȥꤺ�ĥ��ԡ� */
      if (newp[i].present)		/* ����ȥ꤬�ޥåԥ󥰤���Ƥ���ʤ�С�   */
	{				/* ���ԡ����롣                             */
	  p = (I386_PAGE_ENTRY *)(palloc (1));
	  bcopy ((VP)RTOV(dest[i].frame_addr << PAGE_SHIFT), (VP)p, PAGE_SIZE);
#ifdef notdef
	  printf ("dup_vmap_table: [%d]copy 0x%x -> 0x%x\n", 
		  i,
		  (VP)RTOV(dest[i].frame_addr << PAGE_SHIFT), 
		  (VP)p);
#endif /* notdef */
	  newp[i].frame_addr = ((UW)p & 0x7fffffff) >> PAGE_SHIFT;
	}
    }
  return (newp);
}

/***********************************************************************
 * release_vmap --- ���ꤷ�����ɥ쥹�ޥåץơ��֥�򤹤٤Ʋ������롣
 *
 */
extern ER
release_vmap (ADDR_MAP dest)
{
  I386_PAGE_ENTRY	*p;
  W			i;

  for (i = 0; i < ADDR_MAP_SIZE; i++)
    {
      (UW)p = (dest[i].frame_addr << PAGE_SHIFT) | 0x80000000;
      pfree (p, PAGE_SIZE);
    }
  pfree (dest, PAGE_SIZE);
}


/*************************************************************************
 * vmap --- ���ۥ���Υޥåԥ�
 *
 * ������	task	�ޥåԥ󥰤��оݤȤʤ륿����
 *		vpage	���ۥ��ꥢ�ɥ쥹
 *		ppage	ʪ�����ꥢ�ɥ쥹
 *
 * ���͡�	TRUE	����
 *		FALSE	����
 *
 * ������	�����ǻ��ꤵ�줿���ۥ����ʪ������˳�����Ƥ�
 *
 */
BOOL
vmap (T_TCB *task, UW vpage, UW ppage)
{
  I386_DIRECTORY_ENTRY	*dirent;
  I386_PAGE_ENTRY	*pageent;
  UW			dirindex;
  UW			pageindex;

#ifdef DEBUG
  printf ("[%d] vmap: 0x%x -> 0x%x\n", task->tskid, ppage, vpage);
#endif /* DEBUG */
/*  task->context.cr3 &= 0x7fffffff; */
  dirent = (I386_DIRECTORY_ENTRY *)(task->context.cr3);
  ((UW)dirent) |= 0x80000000;
  dirindex = vpage & 0xffc00000;
  dirindex = dirindex >> DIR_SHIFT;
  pageindex = (vpage & 0x003ff000) >> PAGE_SHIFT;

/*
  dirindex = vpage / (PAGE_SIZE * PAGE_SIZE);
*/
/*
  pageindex = (vpage % (PAGE_SIZE * PAGE_SIZE * PAGE_SIZE));
*/

#ifdef DEBUG
  printf ("dirindex = %d, pageindex = %d\n", dirindex, pageindex);
#endif /* DEBUG */
  if (dirent[dirindex].present != 1)
    {
      /* �ڡ����ǥ��쥯�ȥ�Υ���ȥ�϶����ä���
       * �������ڡ����ǥ��쥯�ȥ�Υ���ȥ�����롣
       */
      pageent = (I386_PAGE_ENTRY *)alloc_pagetable ();
      if (pageent == NULL)
	{
	  return (FALSE);
	}
#ifdef DEBUG
      printf ("dir alloc(newp). frame = 0x%x ", ((UW)pageent & 0x0fffffff) >> PAGE_SHIFT);
#endif /* DEBUG */
      dirent[dirindex].frame_addr = ((UW)pageent & 0x0fffffff) >> PAGE_SHIFT;
      dirent[dirindex].present = 1;
      dirent[dirindex].read_write = 1;
      dirent[dirindex].u_and_s = 1;
      dirent[dirindex].zero2 = 0;
      dirent[dirindex].access = 0;
      dirent[dirindex].dirty = 0;
      dirent[dirindex].user = 0;
      dirent[dirindex].zero1 = 0;
    }
  else
    {
      pageent = (I386_PAGE_ENTRY *)(dirent[dirindex].frame_addr * PAGE_SIZE);
#ifdef DEBUG
      printf ("dir alloc(old). frame = 0x%x ", dirent[dirindex].frame_addr);
#endif /* DEBUG */
    }

  pageent[pageindex].frame_addr = (ppage / PAGE_SIZE) & 0x7fffffff;
  pageent[pageindex].present = 1;
  pageent[pageindex].read_write = 1;
  pageent[pageindex].u_and_s = 1;
  pageent[pageindex].zero2 = 0;
  pageent[pageindex].access = 0;
  pageent[pageindex].dirty = 0;
  pageent[pageindex].zero1 = 0;
  pageent[pageindex].user = 0;
/*
  pageent[pageindex + 1].present = 1;
  pageent[pageindex + 1].read_write = 1;
  pageent[pageindex + 1].u_and_s = 1;

  pageent[pageindex + 31].present = 1;
  pageent[pageindex + 31].read_write = 1;
  pageent[pageindex + 31].u_and_s = 1;

  pageent[pageindex + 32].present = 1;
  pageent[pageindex + 32].read_write = 1;
  pageent[pageindex + 32].u_and_s = 1;

  pageent[pageindex + 33].present = 1;
  pageent[pageindex + 33].read_write = 1;
  pageent[pageindex + 33].u_and_s = 1;
*/
#ifdef DEBUG
  printf ("pageindex = %d, frame = 0x%x\n", pageindex, pageent[pageindex].frame_addr);
#endif /* DEBUG */
  return (TRUE);
}

/*
 *
 */
extern ER
vunmap (UW virtual)
{

}



/*************************************************************************
 * alloc_dirent --- �ڡ����ǥ��쥯�ȥ�ơ��֥�κ���
 *
 * ������
 *
 * ���͡�
 *
 * ������
 *
 */
static I386_PAGE_ENTRY *
alloc_pagetable (void)
{
  I386_PAGE_ENTRY	*newp;
  W			i;

  newp = (I386_PAGE_ENTRY *)palloc (1);
  if (newp == NULL)
    {
      return (NULL);
    }
  bzero (newp, PAGE_SIZE);
  for (i = 0; i < PAGE_SIZE / sizeof (I386_PAGE_ENTRY); i++)
    {
      newp[i].present = 0;
      newp[i].read_write = 1;
      newp[i].u_and_s = 1;
      newp[i].zero2 = 0;
      newp[i].access = 0;
      newp[i].dirty = 0;
      newp[i].user = 0;
      newp[i].zero1 = 0;
      newp[i].frame_addr = 0;
    }
  return (newp);
}


/*
 * �꡼�����κ���
 *
 * �ƥ������ϡ��꡼������������äƤ��롣
 * ������ǻȤäƤ��ʤ�����ȥ�����ӡ������ǻ��ꤷ�����������롣
 *
 * ���δؿ�����Ǥϡ�ʪ�������ޥåԥ󥰤���褦�ʽ����Ϥ��ʤ���
 * ñ�˿������꡼������ҤȤĳ�����Ƥ�����Ǥ��롣
 * �⤷���꡼���������������Ȥ���ʪ������������Ƥ����Ȥ��ˤϡ�
 * vcre_reg ��¹Ԥ������Ȥ� vmap_reg ��¹Ԥ���ɬ�פ����롣
 *
 */
ER
vcre_reg (ID	id, 	/* task ID */
	  VP	start,	/* �꡼�����γ��ϥ��ɥ쥹 */
	  W	min,	/* �꡼�����κǾ�(���)������ */
	  W	max,	/* �꡼�����κ��祵���� */
	  UW	perm,	/* �꡼�����Υѡ��ߥå���� */
	  FP	handle)	/* �꡼�������ǥڡ����ե�����Ȥ�ȯ�������� */
			/* ���ν����λ��� */
{
  T_TCB		*taskp;
  T_REGION	*regp;
  W		counter;

  /*
   * �����Υ����å���Ԥ���
   * �⤷�������ͤ������������ˤϡ�E_PAR �Υ��顼�ֹ���֤���
   */
  if (start <= 0)	return (E_PAR);
  if (min <= 0)		return (E_PAR);
  if (max <= 0)		return (E_PAR);
  if (min > max)	return (E_PAR);

  /*
   * ������ ID ���鳺�����륿�����Υ���ƥ����Ȥؤ�
   * �ݥ��󥿤���Ф���
   */
  taskp = get_tskp (id);
  if (taskp == NULL)
    {
      /*
       * �����ǻ��ꤷ�� ID ���ĥ�������¸�ߤ��Ƥ��ʤ���
       * E_OBJ ���֤���
       */
      return (E_OBJ);
    }

  /*
   * ���Ф����������Υ���ƥ����Ⱦ��󤫤�꡼��������Υ���ȥ��
   * ���Ф��� 
   */
  for (counter = 0; counter < MAX_REGION; counter++)
    {
      if (taskp->regions[counter].permission == 0)
	{
	  break;
	}
    }
  if (counter == MAX_REGION)
    {
      /*
       * �����Ƥ���꡼����󤬤ʤ��ä���
       * E_NOMEM �Υ��顼���֤���
       */
      return (E_NOMEM);
    }
  regp = &(taskp->regions[counter]);	/* regp �˶����Ƥ���꡼����� */
					/* ����ȥ��ݥ��󥿤�����롣*/

  /*
   * �꡼������������ꡣ
   * �꡼����󥨥�ȥ�ؤϡ��������ͤ򤽤Τޤ����줺�˰ʲ��Τ褦�ʽ�
   * ����Ԥ���
   *	start		�ڡ������������ڤ�ΤƤ�
   *	min_size	�ڡ������������ڤ�夲��
   *	max_size	�ڡ������������ڤ�夲��
   *	permission	���Τޤ�
   *	handle		���Τޤ�
   */
  regp->start_addr = (VP)CUTDOWN (start, PAGE_SIZE);
  regp->min_size = ROUNDUP (min, PAGE_SIZE);
  regp->max_size = ROUNDUP (max, PAGE_SIZE);
  regp->permission = perm;
  regp->handle = handle;
  
  /*
   * ����������˽�λ������
   */
  return (E_OK);
}

/*
 * �꡼�������˴�
 *
 * ���� start �ǻ��ꤷ�����ɥ쥹�ΰ���������꡼�����������롣 
 * ��������꡼�����˴ޤޤ���ΰ���Υǡ������˴����롣
 *
 * start ���ͤǻ��ꤷ�����ɥ쥹�ϡ��꡼��������Ƭ���ɥ쥹�Ǥ���ɬ��
 * �Ϥʤ����꡼�������Υ��ɥ쥹�ʤ�С��ɤΥ꡼��������ꤷ������
 * �����ƥॳ�������Ƚ�Ǥ��롣
 *
 */
ER
vdel_reg (ID id, VP start) 
     /* id     �������꡼�������ĥ�����
      * start  �������꡼��������Ƭ���ɥ쥹
      */
{
  return (E_NOSPT);
}

/*
 * �꡼�������β��ۥڡ�����ʪ����������դ��롣
 *
 * �����ǻ��ꤷ�����ɥ쥹�ΰ��ʪ����������դ��롣
 *
 * ʣ���Υڡ������������륵���������ꤵ�줿��硢���ƤΥڡ������ޥå�
 * ��ǽ�ΤȤ��Τ�ʪ����������դ��롣����¾�ξ��ϳ���դ��ʤ���
 *
 * �ޥåפ���ʪ������Υ��ɥ쥹�ϻ���Ǥ��ʤ����濴�ˤ����ۥ����
 * ����դ���ʪ�������Ŭ���˳�꿶�롣
 *
 *
 * �֤���
 *
 * �ʲ��Υ��顼�ֹ椬�֤롣
 *	E_OK     �꡼�����Υޥåפ�����  
 *	E_NOMEM  (ʪ��)���꤬��­���Ƥ���
 *	E_NOSPT  �ܥ����ƥॳ����ϡ�̤���ݡ��ȵ�ǽ�Ǥ��롣
 *	E_PAR	 ��������������
 *
 */
ER
vmap_reg (ID id, VP start, UW size)
     /* 
      * id        ������ ID
      * start     �ޥåפ��벾�ۥ����ΰ����Ƭ���ɥ쥹
      * size      �ޥåפ��벾�ۥ����ΰ���礭��(�Х���ñ��)
      */
{
  T_TCB	*taskp;
  UW	counter;
  VP	pmem;

  taskp = (T_TCB *)get_tskp (id);
  if ((taskp->tskstat == TTS_NON) || (taskp->tskstat == TTS_DMT))
    {
      return (E_PAR);
    }

  size = ROUNDUP (size, PAGE_SIZE);
  (UW)start = ((UW)start / PAGE_SIZE) * PAGE_SIZE;
  for (counter = 0; counter < (size / PAGE_SIZE); counter++)
    {
      pmem = palloc (1);
      if (pmem == NULL)
	return (E_NOMEM);

      vmap (taskp, (UW)(counter + start), (UW)pmem);
    }
  return (E_OK);
}

/*
 * ���ꤷ���������Τ�ĥ꡼������ʣ�����롣
 *
 * ʣ�������꡼�����ϡ������̤Τ�ΤȤ��ư����롣
 * src, dst �Τɤ��餫�Υ��������꡼�������ΰ���ѹ����Ƥ⡢�⤦����
 * �Υ������ϱƶ�������ʤ���
 *
 */
ER
vdup_reg (ID src, ID dst, VP start)
     /* src    ʣ������꡼�������ĥ�����
      * dst    �꡼������ʣ����Υ�����
      * start  ʣ������꡼��������Ƭ���ɥ쥹
      */
{
  return (E_NOSPT);
}

/*
 * �꡼�����˴ޤޤ�뤹�٤Ƥβ��ۥ���ڡ����Υץ�ƥ��Ⱦ�������ꤹ�롣
 *
 * �ץ�ƥ��Ⱦ���Ȥ��Ƥϰʲ����ͤ�����Ǥ��롣
 *
 *	VPROT_READ    �ɤ߹��߲�ǽ
 *	VPROT_WRITE   �񤭹��߲�ǽ
 *	VPROT_EXEC    �¹Բ�ǽ
 *
 *
 * �֤���
 *
 * �ʲ��Υ��顼�ֹ椬�֤롣
 *	E_OK     �꡼�����Υޥåפ�����  
 *	E_NOMEM  (ʪ��)���꤬��­���Ƥ���
 *	E_NOSPT  �ܥ����ƥॳ����ϡ�̤���ݡ��ȵ�ǽ�Ǥ��롣
 *	E_OK     �꡼�����Υץ�ƥ��Ⱦ�������������  
 *	E_NOSPT  �ܥ����ƥॳ����ϡ�̤���ݡ��ȵ�ǽ�Ǥ��롣
 */
ER
vprt_reg (ID id, VP start, UW prot)
     /* id     �꡼�������ĥ�����
      * start  �꡼��������Ƭ���ۥ��ɥ쥹
      * prot   �ץ�ƥ��Ⱦ���
      */
{
  return (E_NOSPT);
}

/*
 * �������֤ǤΥ꡼�����ζ�ͭ
 *
 * ���� src �ǻ��ꤷ���������Τ�ĥ꡼��������� dst �ǻ��ꤷ������
 * ���˳�����Ƥ롣��ꤢ�Ƥ��꡼�����϶�ͭ����롣
 *
 * ��ͭ���줿�꡼�����˴ޤޤ�벾�ۥ��ɥ쥹�ΰ�ˤϡ��������֤�Ʊ��
 * ʪ�����ɥ쥹�������Ƥ롣���Τ��ᡢ�����Υ����������ۥ��ɥ쥹�ΰ�
 * ��ξ�����ѹ�������硢¾���Υ������ˤ�ȿ�Ǥ���롣
 *
 *
 * �֤���
 *
 * �ʲ��Υ��顼�ֹ椬�֤롣
 *
 *	E_OK     �꡼�����ζ�ͭ������  
 *	E_NOSPT  �ܥ����ƥॳ����ϡ�̤���ݡ��ȵ�ǽ�Ǥ��롣
 *
 */
ER
vshr_reg (ID src, ID dst, VP start)
     /*
      * src    ��ͭ���Υ꡼�������ĥ�����
      * dst    �����˥꡼������ͭ���륿����
      * start  �꡼��������Ƭ���ɥ쥹
      */
     
{
  return (E_NOSPT);
}

/*
 * �꡼����󤫤���ɤ߹���
 *
 * Ǥ�դΥ������β��ۥ����ΰ褫��ǡ������ɤ߹��ࡣ
 * �ڡ��������Ȥʤɤ˻��Ѥ��롣
 *
 *
 * �֤���
 *
 * �ʲ��Υ��顼�ֹ椬�֤롣
 *
 *	E_OK     ����  
 *	E_ID     �꡼�������ĥ�����
 *	E_NOSPT  �ܥ����ƥॳ����ϡ�̤���ݡ��ȵ�ǽ�Ǥ���
 *
 */
ER
vget_reg (ID id, VP start, UW size, VP buf) 
     /*
      * id     �꡼��������ĥ�����
      * start  �ɤ߹����ΰ����Ƭ���ɥ쥹
      * size   �꡼����󤫤��ɤ߹��ॵ����
      * buf    �꡼����󤫤��ɤ߹�����ǡ���������Хåե�
      */
{
  return (E_NOSPT);
}

/*
 * �꡼�����ؤν񤭹���
 *
 * Ǥ�դΥ������β��ۥ����ΰ�˥ǡ�����񤭹��ࡣ
 * �ڡ�������ʤɤ˻��ѤǤ��롣
 *
 *
 * �֤���
 *
 * �ʲ��Υ��顼�ֹ椬�֤롣
 *
 *	E_OK     �꡼�����ؤν񤭹��ߤ�����  
 *	E_ID     ���� id ���б�������������¸�ߤ��ʤ�
 *	E_NOSPT  �ܥ����ƥॳ����ϡ�̤���ݡ��ȵ�ǽ�Ǥ���
 *
 */
ER
vput_reg (ID id, VP start, UW size, VP buf)
     /*
      * id     �꡼��������ĥ�����
      * start  �񤭹����ΰ����Ƭ���ɥ쥹
      * size   �꡼�����˽񤭹��ॵ����
      * buf    �꡼�����˽񤭹���ǡ���
      */
{
  return (E_NOSPT);
}

/*
 * �꡼�����ξ����������롣
 *
 * �꡼��������Ȥ��Ƥϼ��Τ�Τ��ͤ����롣
 *
 *	�꡼��������Ƭ���ۥ��ɥ쥹
 *	�꡼�����Υ�����
 *	�ץ�ƥ��Ⱦ���
 * 
 * �֤���
 *
 * �ʲ��Υ��顼�ֹ椬�֤롣
 *
 *	E_OK     �꡼�����ξ���μ���������  
 *	E_ID     ���� id �ǻ��ꤷ����������¸�ߤ��ʤ�
 *	E_NOSPT  �ܥ����ƥॳ����ϡ�̤���ݡ��ȵ�ǽ�Ǥ���
 *
 */
ER
vsts_reg (id, start, stat)
     /*
      * id     �꡼�������ĥ�����
      * start  �꡼��������Ƭ���ɥ쥹
      * stat   �꡼������������(�꡼��������ξܺ٤�̤����Ǥ���) 
      */
{
  return (E_NOSPT);
}

