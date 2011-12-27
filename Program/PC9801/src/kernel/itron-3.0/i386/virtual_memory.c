/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

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
 * 呼び出す関数名が間違っていたので正しく直した。
 *
 * Revision 1.11  1995/12/13 15:02:23  night
 * vmap_reg() 関数の中身を作成した。
 *
 * Revision 1.10  1995/09/21  15:51:29  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.9  1995/09/17  17:00:12  night
 * 余分な printf () を #ifdef notdef ... #endif で囲んだ。
 *
 * Revision 1.8  1995/09/14  04:33:09  night
 * アドレスマスク用の定数の値を変更。
 *
 * Revision 1.7  1995/05/31  22:58:00  night
 * いくつかの #ifdef DEBUG ... #endif の追加。
 * (余分なデバッグ用 printf 文を隔離した)
 *
 * Revision 1.6  1995/03/18  14:50:11  night
 * vcre_reg() 関数をコンパイルできるように修正。
 *
 * Revision 1.5  1995/02/26  14:07:40  night
 * RCS マクロ $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/itron-3.0/i386/virtual_memory.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ と $Log: virtual_memory.c,v $
 * RCS マクロ $Header: /home/night/CVS/B-Free/Program/PC9801/src/kernel/itron-3.0/i386/virtual_memory.c,v 1.12 1996-01-06 16:08:03 night Exp $ と Revision 1.1  2011/12/27 17:13:36  liu1
 * RCS マクロ $Header: /home/night/CVS/B-Free/Program/PC9801/src/kernel/itron-3.0/i386/virtual_memory.c,v 1.12 1996-01-06 16:08:03 night Exp $ と Initial Version.
 * RCS マクロ $Header: /home/night/CVS/B-Free/Program/PC9801/src/kernel/itron-3.0/i386/virtual_memory.c,v 1.12 1996-01-06 16:08:03 night Exp $ と
 * RCS マクロ $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/itron-3.0/i386/virtual_memory.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ と Revision 1.11  1995/12/13 15:02:23  night
 * RCS マクロ $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/itron-3.0/i386/virtual_memory.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ と vmap_reg() 関数の中身を作成した。
 * RCS マクロ $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/itron-3.0/i386/virtual_memory.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ と
 * Revision 1.10  1995/09/21  15:51:29  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.9  1995/09/17  17:00:12  night
 * 余分な printf () を #ifdef notdef ... #endif で囲んだ。
 *
 * Revision 1.8  1995/09/14  04:33:09  night
 * アドレスマスク用の定数の値を変更。
 *
 * Revision 1.7  1995/05/31  22:58:00  night
 * いくつかの #ifdef DEBUG ... #endif の追加。
 * (余分なデバッグ用 printf 文を隔離した)
 *
 * Revision 1.6  1995/03/18  14:50:11  night
 * vcre_reg() 関数をコンパイルできるように修正。
 * を追加。
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


/* dup_vmap_table --- 指定された仮想メモリのマッピングテーブルを
 *		      コピーする。
 *		      マップテーブル自体は新しく作成する。
 *
 */
ADDR_MAP 
dup_vmap_table (ADDR_MAP dest)
{
  ADDR_MAP	newp;
  int		i;
  I386_PAGE_ENTRY	*p;


  (UW)dest = (UW)dest | 0x80000000;
  newp = (ADDR_MAP)(palloc (1));	/* ページディレクトリのアロケート */
  for (i = 0; i < ADDR_MAP_SIZE; i++)
    {
      newp[i] = dest[i];		/* ページディレクトリを１エントリずつコピー */
      if (newp[i].present)		/* エントリがマッピングされているならば、   */
	{				/* コピーする。                             */
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
 * release_vmap --- 指定したアドレスマップテーブルをすべて解放する。
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
 * vmap --- 仮想メモリのマッピング
 *
 * 引数：	task	マッピングの対象となるタスク
 *		vpage	仮想メモリアドレス
 *		ppage	物理メモリアドレス
 *
 * 返値：	TRUE	成功
 *		FALSE	失敗
 *
 * 処理：	引数で指定された仮想メモリを物理メモリに割り当てる
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
      /* ページディレクトリのエントリは空だった。
       * 新しくページディレクトリのエントリを埋める。
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
 * alloc_dirent --- ページディレクトリテーブルの作成
 *
 * 引数：
 *
 * 返値：
 *
 * 処理：
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
 * リージョンの作成
 *
 * 各タスクは、リージョンの配列をもっている。
 * その中で使っていないエントリを選び、引数で指定した情報を入れる。
 *
 * この関数の中では、物理メモリをマッピングするような処理はしない。
 * 単に新しいリージョンをひとつ割り当てるだけである。
 * もし、リージョンを生成したときに物理メモリを割り当てたいときには、
 * vcre_reg を実行したあとに vmap_reg を実行する必要がある。
 *
 */
ER
vcre_reg (ID	id, 	/* task ID */
	  VP	start,	/* リージョンの開始アドレス */
	  W	min,	/* リージョンの最小(初期)サイズ */
	  W	max,	/* リージョンの最大サイズ */
	  UW	perm,	/* リージョンのパーミッション */
	  FP	handle)	/* リージョン内でページフォールトが発生したと */
			/* きの処理の指定 */
{
  T_TCB		*taskp;
  T_REGION	*regp;
  W		counter;

  /*
   * 引数のチェックを行う。
   * もし引数の値がおかしい場合には、E_PAR のエラー番号を返す。
   */
  if (start <= 0)	return (E_PAR);
  if (min <= 0)		return (E_PAR);
  if (max <= 0)		return (E_PAR);
  if (min > max)	return (E_PAR);

  /*
   * タスク ID から該当するタスクのコンテキストへの
   * ポインタを取り出す。
   */
  taskp = get_tskp (id);
  if (taskp == NULL)
    {
      /*
       * 引数で指定した ID をもつタスクは存在していない。
       * E_OBJ を返す。
       */
      return (E_OBJ);
    }

  /*
   * 取り出したタスクのコンテキスト情報からリージョン情報のエントリを
   * 取り出す。 
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
       * 空いているリージョンがなかった。
       * E_NOMEM のエラーを返す。
       */
      return (E_NOMEM);
    }
  regp = &(taskp->regions[counter]);	/* regp に空いているリージョン */
					/* エントリをポインタを入れる。*/

  /*
   * リージョン情報の設定。
   * リージョンエントリへは、引数の値をそのまま入れずに以下のような処
   * 理を行う。
   *	start		ページサイズで切り捨てる
   *	min_size	ページサイズで切り上げる
   *	max_size	ページサイズで切り上げる
   *	permission	そのまま
   *	handle		そのまま
   */
  regp->start_addr = (VP)CUTDOWN (start, PAGE_SIZE);
  regp->min_size = ROUNDUP (min, PAGE_SIZE);
  regp->max_size = ROUNDUP (max, PAGE_SIZE);
  regp->permission = perm;
  regp->handle = handle;
  
  /*
   * 処理は正常に終了した。
   */
  return (E_OK);
}

/*
 * リージョンの破棄
 *
 * 引数 start で指定したアドレス領域を管理するリージョンを削除する。 
 * 削除したリージョンに含まれる領域中のデータは破棄する。
 *
 * start の値で指定したアドレスは、リージョンの先頭アドレスである必要
 * はない。リージョン内のアドレスならば、どのリージョンを指定したかを
 * システムコール内で判断する。
 *
 */
ER
vdel_reg (ID id, VP start) 
     /* id     削除するリージョンをもつタスク
      * start  削除するリージョンの先頭アドレス
      */
{
  return (E_NOSPT);
}

/*
 * リージョン内の仮想ページへ物理メモリを割り付ける。
 *
 * 引数で指定したアドレス領域に物理メモリを割り付ける。
 *
 * 複数のページに相当するサイズが指定された場合、全てのページがマップ
 * 可能のときのみ物理メモリを割り付ける。その他の場合は割り付けない。
 *
 * マップする物理メモリのアドレスは指定できない。中心核が仮想メモリに
 * 割り付ける物理メモリを適当に割り振る。
 *
 *
 * 返り値
 *
 * 以下のエラー番号が返る。
 *	E_OK     リージョンのマップに成功  
 *	E_NOMEM  (物理)メモリが不足している
 *	E_NOSPT  本システムコールは、未サポート機能である。
 *	E_PAR	 引数がおかしい
 *
 */
ER
vmap_reg (ID id, VP start, UW size)
     /* 
      * id        タスク ID
      * start     マップする仮想メモリ領域の先頭アドレス
      * size      マップする仮想メモリ領域の大きさ(バイト単位)
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
 * 指定したタスクのもつリージョンを複製する。
 *
 * 複製したリージョンは、全く別のものとして扱われる。
 * src, dst のどちらかのタスクがリージョンの領域を変更しても、もう片方
 * のタスクは影響を受けない。
 *
 */
ER
vdup_reg (ID src, ID dst, VP start)
     /* src    複製するリージョンをもつタスク
      * dst    リージョンの複製先のタスク
      * start  複製するリージョンの先頭アドレス
      */
{
  return (E_NOSPT);
}

/*
 * リージョンに含まれるすべての仮想メモリページのプロテクト情報を設定する。
 *
 * プロテクト情報としては以下の値が指定できる。
 *
 *	VPROT_READ    読み込み可能
 *	VPROT_WRITE   書き込み可能
 *	VPROT_EXEC    実行可能
 *
 *
 * 返り値
 *
 * 以下のエラー番号が返る。
 *	E_OK     リージョンのマップに成功  
 *	E_NOMEM  (物理)メモリが不足している
 *	E_NOSPT  本システムコールは、未サポート機能である。
 *	E_OK     リージョンのプロテクト情報の設定に成功  
 *	E_NOSPT  本システムコールは、未サポート機能である。
 */
ER
vprt_reg (ID id, VP start, UW prot)
     /* id     リージョンをもつタスク
      * start  リージョンの先頭仮想アドレス
      * prot   プロテクト情報
      */
{
  return (E_NOSPT);
}

/*
 * タスク間でのリージョンの共有
 *
 * 引数 src で指定したタスクのもつリージョンを引数 dst で指定したタス
 * クに割り当てる。割りあてたリージョンは共有される。
 *
 * 共有されたリージョンに含まれる仮想アドレス領域には、タスク間で同じ
 * 物理アドレスを割り当てる。そのため、片方のタスクが仮想アドレス領域
 * 内の情報を変更した場合、他方のタスクにも反映される。
 *
 *
 * 返り値
 *
 * 以下のエラー番号が返る。
 *
 *	E_OK     リージョンの共有に成功  
 *	E_NOSPT  本システムコールは、未サポート機能である。
 *
 */
ER
vshr_reg (ID src, ID dst, VP start)
     /*
      * src    共有元のリージョンをもつタスク
      * dst    新たにリージョンを共有するタスク
      * start  リージョンの先頭アドレス
      */
     
{
  return (E_NOSPT);
}

/*
 * リージョンからの読み込み
 *
 * 任意のタスクの仮想メモリ領域からデータを読み込む。
 * ページアウトなどに使用する。
 *
 *
 * 返り値
 *
 * 以下のエラー番号が返る。
 *
 *	E_OK     成功  
 *	E_ID     リージョンをもつタスク
 *	E_NOSPT  本システムコールは、未サポート機能である
 *
 */
ER
vget_reg (ID id, VP start, UW size, VP buf) 
     /*
      * id     リージョンを持つタスク
      * start  読み込む領域の先頭アドレス
      * size   リージョンから読み込むサイズ
      * buf    リージョンから読み込んだデータを収めるバッファ
      */
{
  return (E_NOSPT);
}

/*
 * リージョンへの書き込み
 *
 * 任意のタスクの仮想メモリ領域にデータを書き込む。
 * ページインなどに使用できる。
 *
 *
 * 返り値
 *
 * 以下のエラー番号が返る。
 *
 *	E_OK     リージョンへの書き込みに成功  
 *	E_ID     引数 id に対応したタスクは存在しない
 *	E_NOSPT  本システムコールは、未サポート機能である
 *
 */
ER
vput_reg (ID id, VP start, UW size, VP buf)
     /*
      * id     リージョンを持つタスク
      * start  書き込む領域の先頭アドレス
      * size   リージョンに書き込むサイズ
      * buf    リージョンに書き込むデータ
      */
{
  return (E_NOSPT);
}

/*
 * リージョンの情報を取得する。
 *
 * リージョン情報としては次のものが考えられる。
 *
 *	リージョンの先頭仮想アドレス
 *	リージョンのサイズ
 *	プロテクト情報
 * 
 * 返り値
 *
 * 以下のエラー番号が返る。
 *
 *	E_OK     リージョンの情報の取得に成功  
 *	E_ID     引数 id で指定したタスクは存在しない
 *	E_NOSPT  本システムコールは、未サポート機能である
 *
 */
ER
vsts_reg (id, start, stat)
     /*
      * id     リージョンをもつタスク
      * start  リージョンの先頭アドレス
      * stat   リージョン情報が入る(リージョン情報の詳細は未決定である) 
      */
{
  return (E_NOSPT);
}

