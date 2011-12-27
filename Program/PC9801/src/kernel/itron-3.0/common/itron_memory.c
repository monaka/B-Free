/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* itron_memory.c
 *
 * itron 仕様に準拠したメモリ管理のモジュール
 *
 *
 * このファイルには、ITRON 仕様に準拠したメモリ管理システムコールの
 * 実現部が収められている。本ITRONでは、可変長メモリプール管理機能のみ
 * インプリメントし、固定長メモリプール管理機能は、サポートしない。
 * 固定長メモリプール管理機能は、可変長メモリプール管理機能によって
 * 同等の処理が実現できるためである。
 *
 *
 * ITRON では、以下の関数が可変長メモリプール管理機能として規定されている。
 *
 *	cre_mpl		メモリプールの生成
 *	del_mpl		可変長メモリプールの削除
 *	get_blk		可変長メモリブロック獲得
 *	pget_blk	可変長メモリブロック獲得(ポーリング)
 *	tget_blk	可変長メモリブロック獲得(タイムアウト有)
 *	rel_blk		可変長メモリブロック返却
 *	ref_mpl		可変長メモリプール状態参照
 * 
 * これ以外の関数として、メモリプール管理機能自体の処理のために
 * 次の関数が定義されている。
 *
 *	init_mpl	メモリプール機能の初期化
 *
 * 
 */

#include "itron.h"
#include "task.h"
#include "errno.h"
#include "func.h"
#include "misc.h"

/* フリーメモリの領域を管理するための構造体。
   使われていない仮想記憶領域は、この構造体によって
   管理する。
 */
struct free_mem_entry
{
  struct free_mem_entry	*next;
  UW			size;		/* フリー領域のサイズ (バイト単位)	*/
  B			free[0];	/* フリー領域の先頭アドレス 		*/
};


/* メモリプール構造体：
 *	メモリプール1つにつきこの構造体が1つ使われる。
 */
struct memory_pool
{

  H		id;		/* メモリプールの ID 		*/
  UW		size;		/* メモリプールのサイズ 	*/
  T_TCB		*wait_tasks;	/* このメモリプールから領域を取りたいタスクの wait リスト */
  INT		align;
  VP		area;


  /* このメモリプールが管理する自由領域のリスト
   */
  struct free_mem_entry	free_list;	/* 最初のエントリ (ダミー) */
  struct free_mem_entry	*pivot;		/* 現在、参照しているエントリを指す */
};

/* メモリプールテーブル
 * メモリプールは、MAX_MEMORY_POOL 個分だけ生成できる。
 */
static struct memory_pool	memory_pool_table[MAX_MEMORY_POOL];


/* init_mpl	メモリプール機能を初期化する。
   
 */
int
init_mpl (void)
{
  int		i;
  struct free_mem_entry *p;

  /* メモリプールテーブル memory_pool_table を初期化する。
   */
  for (i = 0; i < MAX_MEMORY_POOL; i++)
    {
      memory_pool_table[i].id == -1;	/* このメモリプールは使用されていない */
    }
}


/* 可変長メモリプールを生成する。
 *
 *	エラー番号:
 *	E_ID	不正 ID 番号 (mplid が不正)
 *	E_OBJ	すでに同一 ID の可変長メモリ ID が存在する。
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

  /* mplid で指定された ID のメモリプールの初期化 */
  if (memory_pool_table[mplid].id == mplid)	/* すでにこのメモリプールは初期化されている */
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
      /* メモリが取得できなかった */
      return (E_NOMEM);
    }

  newp->size = pk_cmpl->mplsz;
  newp->next = &(memory_pool_table[mplid].free_list);
  memory_pool_table[mplid].free_list.next = newp;
}


/* 可変長メモリプールの削除
 *
 *	エラー番号:
 *	E_ID	不正 ID 番号
 *	E_OBJ	すでに同一 ID の可変長メモリ ID が存在する。
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

  /* メモリプール領域をシステムメモリプールに返却する */
  memory_pool_table[mplid].id = NULL;
  kfree (memory_pool_table[mplid].area, memory_pool_table[mplid].size);
}


/* 可変長メモリブロック獲得
 *
 *	引数:
 *	pblk	
 *	mplid	メモリブロックを獲得するメモリプール ID 
 *	blksz	獲得するブロックのサイズ
 *
 *	返り値:	エラー番号
 *		E_OK	正常にメモリブロックは取得できた。
 *		E_ID	mplid 不正
 *		E_OBJ	すでに同一 ID の可変長メモリ ID が存在する。
 *
 */
ER
get_blk (VP *pblk, ID mplid, INT blksz)
{
  ER			err;

retry:
  err = pget_blk (pblk, mplid, blksz);
  /* エラー番号がタイムアウト以外ならば、そのままエラー番号を
   * 呼び出し側に返す。
   */
  if (err != E_TMOUT)
    return (err);

  /* 丁度いいメモリブロックが見付からなかった。
   * メモリプールのメモリ獲得待ち行列につながり、
   * メモリが獲得できるようになるまで待つ. 
   */
  memory_pool_table[mplid].wait_tasks = add_tcb_list (memory_pool_table[mplid].wait_tasks,
						      run_task);
  slp_tsk ();
  goto retry;
/* NOT REACHED */
}


/* 可変長メモリブロック獲得 (ポーリング)
 *
 *	エラー番号:
 *	E_ID	mplid不正
 *	E_OBJ	すでに同一 ID の可変長メモリ ID が存在する。
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

/* 最初に取得するブロックのサイズとメモリプールの
   各エントリのヘッダサイズとを求めておく */
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

  /* メモリプールのフリーリストから最初に合致したエントリを取り出す */
  for (p = memory_pool_table[mplid].pivot->next, q = memory_pool_table[mplid].pivot;
       p != memory_pool_table[mplid].pivot;
       q = p, p = p->next)
    {
      /* メモリブロック獲得の条件：
       *	1) メモリブロックのサイズが獲得したいサイズと等しい。
       *	または
       *	2) (ヘッダのサイズ + メモリブロックのサイズ) が、
       *	   獲得したいサイズよりも大きい。
       */
      if (p->size == size)
	{
	  /* メモリブロックのサイズが獲得したいサイズと等しい場合には、
	   * ブロックを削除してそのブロックのアドレスを返す。
	   */
	  q->next = p->next;
	  *pblk = p;
	  return (E_OK);
	}
      else if (p->size >= (size + head_size))
	{
	  /* メモリブロックのサイズが獲得したいサイズよりも大きい場合には、
	   * メモリブロックのサイズを小さくして、メモリブロックの後ろを返す。
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
  /* 丁度いいメモリブロックが見付からなかった。E_TMOUT を返す
   */
  return (E_TMOUT);
}


/* 可変長メモリブロック獲得 (タイムアウト有) - まだ、作成中
 *
 *	エラー番号:
 *	E_ID	mplid不正
 *	E_OBJ	指定された ID の可変長メモリ ID が存在しない。
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


/* 可変長メモリブロック返却 - まだ、作成中
 *
 *	エラー番号:
 *	E_ID	mplid不正
 *	E_OBJ	すでに同一 ID の可変長メモリ ID が存在する。
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
   * メモリプールにメモリブロックを返却する。
   */
  
  return (E_OK);
}



/* 可変長メモリブロック状態参照
 *
 *	エラー番号:
 *	E_ID	mplid不正
 *	E_OBJ	すでに同一 ID の可変長メモリ ID が存在する。
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


