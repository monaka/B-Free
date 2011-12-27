/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* itron_memory.c
 *
 * itron ���ͤ˽�򤷤���������Υ⥸�塼��
 *
 *
 * ���Υե�����ˤϡ�ITRON ���ͤ˽�򤷤�������������ƥॳ�����
 * �¸�����������Ƥ��롣��ITRON�Ǥϡ�����Ĺ����ס��������ǽ�Τ�
 * ����ץ���Ȥ�������Ĺ����ס��������ǽ�ϡ����ݡ��Ȥ��ʤ���
 * ����Ĺ����ס��������ǽ�ϡ�����Ĺ����ס��������ǽ�ˤ�ä�
 * Ʊ���ν������¸��Ǥ��뤿��Ǥ��롣
 *
 *
 * ITRON �Ǥϡ��ʲ��δؿ�������Ĺ����ס��������ǽ�Ȥ��Ƶ��ꤵ��Ƥ��롣
 *
 *	cre_mpl		����ס��������
 *	del_mpl		����Ĺ����ס���κ��
 *	get_blk		����Ĺ����֥�å�����
 *	pget_blk	����Ĺ����֥�å�����(�ݡ����)
 *	tget_blk	����Ĺ����֥�å�����(�����ॢ����ͭ)
 *	rel_blk		����Ĺ����֥�å��ֵ�
 *	ref_mpl		����Ĺ����ס�����ֻ���
 * 
 * ����ʳ��δؿ��Ȥ��ơ�����ס��������ǽ���Τν����Τ����
 * ���δؿ����������Ƥ��롣
 *
 *	init_mpl	����ס��뵡ǽ�ν����
 *
 * 
 */

#include "itron.h"
#include "task.h"
#include "errno.h"
#include "func.h"
#include "misc.h"

/* �ե꡼������ΰ��������뤿��ι�¤�Ρ�
   �Ȥ��Ƥ��ʤ����۵����ΰ�ϡ����ι�¤�Τˤ�ä�
   �������롣
 */
struct free_mem_entry
{
  struct free_mem_entry	*next;
  UW			size;		/* �ե꡼�ΰ�Υ����� (�Х���ñ��)	*/
  B			free[0];	/* �ե꡼�ΰ����Ƭ���ɥ쥹 		*/
};


/* ����ס��빽¤�Ρ�
 *	����ס���1�ĤˤĤ����ι�¤�Τ�1�ĻȤ��롣
 */
struct memory_pool
{

  H		id;		/* ����ס���� ID 		*/
  UW		size;		/* ����ס���Υ����� 	*/
  T_TCB		*wait_tasks;	/* ���Υ���ס��뤫���ΰ���ꤿ���������� wait �ꥹ�� */
  INT		align;
  VP		area;


  /* ���Υ���ס��뤬�������뼫ͳ�ΰ�Υꥹ��
   */
  struct free_mem_entry	free_list;	/* �ǽ�Υ���ȥ� (���ߡ�) */
  struct free_mem_entry	*pivot;		/* ���ߡ����Ȥ��Ƥ��륨��ȥ��ؤ� */
};

/* ����ס���ơ��֥�
 * ����ס���ϡ�MAX_MEMORY_POOL ��ʬ���������Ǥ��롣
 */
static struct memory_pool	memory_pool_table[MAX_MEMORY_POOL];


/* init_mpl	����ס��뵡ǽ���������롣
   
 */
int
init_mpl (void)
{
  int		i;
  struct free_mem_entry *p;

  /* ����ס���ơ��֥� memory_pool_table ���������롣
   */
  for (i = 0; i < MAX_MEMORY_POOL; i++)
    {
      memory_pool_table[i].id == -1;	/* ���Υ���ס���ϻ��Ѥ���Ƥ��ʤ� */
    }
}


/* ����Ĺ����ס�����������롣
 *
 *	���顼�ֹ�:
 *	E_ID	���� ID �ֹ� (mplid ������)
 *	E_OBJ	���Ǥ�Ʊ�� ID �β���Ĺ���� ID ��¸�ߤ��롣
 *
 */
ER
cre_mpl (ID mplid, T_CMPL *pk_cmpl)
{
  struct free_mem_entry *newp;

  if (mplid <= 0 || mplid > MAX_MEMORY_POOL)
    {
      return (E_ID);
    }

  /* mplid �ǻ��ꤵ�줿 ID �Υ���ס���ν���� */
  if (memory_pool_table[mplid].id == mplid)	/* ���Ǥˤ��Υ���ס���Ͻ��������Ƥ��� */
    {
      return (E_OBJ);
    }

  memory_pool_table[mplid].id = mplid;
  memory_pool_table[mplid].size = pk_cmpl->mplsz;
  memory_pool_table[mplid].pivot = &(memory_pool_table[mplid].free_list);
  memory_pool_table[mplid].align = pk_cmpl->align;

  newp = (struct free_mem_entry *)kalloc (pk_cmpl->mplsz);
  if (newp == NULL)
    {
      /* ���꤬�����Ǥ��ʤ��ä� */
      return (E_NOMEM);
    }

  newp->size = pk_cmpl->mplsz;
  newp->next = &(memory_pool_table[mplid].free_list);
  memory_pool_table[mplid].free_list.next = newp;
}


/* ����Ĺ����ס���κ��
 *
 *	���顼�ֹ�:
 *	E_ID	���� ID �ֹ�
 *	E_OBJ	���Ǥ�Ʊ�� ID �β���Ĺ���� ID ��¸�ߤ��롣
 *
 */
ER
del_mpl (ID mplid)
{
  if (mplid < 0 || mplid > MAX_MEMORY_POOL)
    {
      return (E_ID);
    }

  if (memory_pool_table[mplid].id != mplid)
    {
      return (E_OBJ);
    }

  /* ����ס����ΰ�򥷥��ƥ����ס�����ֵѤ��� */
  memory_pool_table[mplid].id = NULL;
  kfree (memory_pool_table[mplid].area, memory_pool_table[mplid].size);
}


/* ����Ĺ����֥�å�����
 *
 *	����:
 *	pblk	
 *	mplid	����֥�å�������������ס��� ID 
 *	blksz	��������֥�å��Υ�����
 *
 *	�֤���:	���顼�ֹ�
 *		E_OK	����˥���֥�å��ϼ����Ǥ�����
 *		E_ID	mplid ����
 *		E_OBJ	���Ǥ�Ʊ�� ID �β���Ĺ���� ID ��¸�ߤ��롣
 *
 */
ER
get_blk (VP *pblk, ID mplid, INT blksz)
{
  ER			err;

retry:
  err = pget_blk (pblk, mplid, blksz);
  /* ���顼�ֹ椬�����ॢ���Ȱʳ��ʤ�С����Τޤޥ��顼�ֹ��
   * �ƤӽФ�¦���֤���
   */
  if (err != E_TMOUT)
    return (err);

  /* ���٤�������֥�å������դ���ʤ��ä���
   * ����ס���Υ�������Ԥ�����ˤĤʤ��ꡢ
   * ���꤬�����Ǥ���褦�ˤʤ�ޤ��Ԥ�. 
   */
  memory_pool_table[mplid].wait_tasks = add_tcb_list (memory_pool_table[mplid].wait_tasks,
						      run_task);
  slp_tsk ();
  goto retry;
/* NOT REACHED */
}


/* ����Ĺ����֥�å����� (�ݡ����)
 *
 *	���顼�ֹ�:
 *	E_ID	mplid����
 *	E_OBJ	���Ǥ�Ʊ�� ID �β���Ĺ���� ID ��¸�ߤ��롣
 *
 */
ER
pget_blk (VP *pblk, ID mplid, INT blksz)
{
  struct free_mem_entry	*p, *q;
  INT			align, head_size, size;

  if (mplid < 0 || mplid > MAX_MEMORY_POOL)
    {
      return (E_ID);
    }

  if (memory_pool_table[mplid].id != mplid)
    {
      return (E_OBJ);
    }

/* �ǽ�˼�������֥�å��Υ������ȥ���ס����
   �ƥ���ȥ�Υإå��������Ȥ���Ƥ��� */
  align = memory_pool_table[mplid].align;
  head_size = MAX (sizeof (struct free_mem_entry), align);
  size = ROUNDUP (blksz, align);

  printf ("### pget_blk: align = %d, head_size = %d, size = %d\n",
	  align, head_size, size);

  if (mplid < 0 || mplid > MAX_MEMORY_POOL)
    {
      return (E_ID);
    }

  if (memory_pool_table[mplid].id != mplid)
    {
      return (E_OBJ);
    }

  /* ����ס���Υե꡼�ꥹ�Ȥ���ǽ�˹��פ�������ȥ����Ф� */
  for (p = memory_pool_table[mplid].pivot->next, q = memory_pool_table[mplid].pivot;
       p != memory_pool_table[mplid].pivot;
       q = p, p = p->next)
    {
      /* ����֥�å������ξ�
       *	1) ����֥�å��Υ���������������������������������
       *	�ޤ���
       *	2) (�إå��Υ����� + ����֥�å��Υ�����) ����
       *	   ���������������������礭����
       */
      if (p->size == size)
	{
	  /* ����֥�å��Υ����������������������������������ˤϡ�
	   * �֥�å��������Ƥ��Υ֥�å��Υ��ɥ쥹���֤���
	   */
	  q->next = p->next;
	  *pblk = p;
	  return (E_OK);
	}
      else if (p->size >= (size + head_size))
	{
	  /* ����֥�å��Υ����������������������������礭�����ˤϡ�
	   * ����֥�å��Υ������򾮤������ơ�����֥�å��θ����֤���
	   */
	  printf ("### pget_blk: p = 0x%x, p->size = 0x%x, size = 0x%x\n", 
		  p, p->size, size);
	  printf ("### pget_blk: p->size - size = %d\n", p->size - size);
	  *pblk = (VP)((W)p + ((W)(p->size) - (W)size));
	  p->size -= size;
	  printf ("### pget_blk: pblk = 0x%x, p->size = %d\n", *pblk, p->size);
	  return (E_OK);
	}
    }
  /* ���٤�������֥�å������դ���ʤ��ä���E_TMOUT ���֤�
   */
  return (E_TMOUT);
}


/* ����Ĺ����֥�å����� (�����ॢ����ͭ) - �ޤ���������
 *
 *	���顼�ֹ�:
 *	E_ID	mplid����
 *	E_OBJ	���ꤵ�줿 ID �β���Ĺ���� ID ��¸�ߤ��ʤ���
 *
 * 
 */
ER
tget_blk (VP *pblk, ID mplid, INT blksz, TMO tmout)
{
  if (mplid < 0 || mplid > MAX_MEMORY_POOL)
    {
      return (E_ID);
    }

  if (memory_pool_table[mplid].id != mplid)
    {
      return (E_OBJ);
    }
  return (E_OK);
}


/* ����Ĺ����֥�å��ֵ� - �ޤ���������
 *
 *	���顼�ֹ�:
 *	E_ID	mplid����
 *	E_OBJ	���Ǥ�Ʊ�� ID �β���Ĺ���� ID ��¸�ߤ��롣
 *
 */
ER
rel_blk(ID mplid, VP blk)
{
  if (mplid < 0 || mplid > MAX_MEMORY_POOL)
    {
      return (E_ID);
    }

  if (memory_pool_table[mplid].id != mplid)
    {
      return (E_OBJ);
    }

  /*
   * ����ס���˥���֥�å����ֵѤ��롣
   */
  
  return (E_OK);
}



/* ����Ĺ����֥�å����ֻ���
 *
 *	���顼�ֹ�:
 *	E_ID	mplid����
 *	E_OBJ	���Ǥ�Ʊ�� ID �β���Ĺ���� ID ��¸�ߤ��롣
 *
 */
ER
ref_mpl	(ID mplid, T_RMPL *pk_rmpl)
{
  if (mplid < 0 || mplid > MAX_MEMORY_POOL)
    {
      return (E_ID);
    }

  if (memory_pool_table[mplid].id != mplid)
    {
      return (E_OBJ);
    }
  return (E_OK);
}


