/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

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
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/03/18  14:17:13  night
 * calloc(), free(), morecore() 関数の追加。
 *
 * Revision 1.1  1995/03/04  16:04:45  night
 * 最初の登録
 *
 *
 */

 
#include <sys/types.h>
#include <native/syscall.h>
#include <sys/config.h>
#include <stdlib.h>


/*
 * ヒープメモリ管理関数群
 *
 * プログラムの実行中に動的にメモリを確保/解放するための関数群
 *
 *
 * このファイルには以下の関数が存在している。
 *
 *	malloc	......	メモリの取得関数
 *	calloc	......	メモリの取得関数 (mallocを使っている)
 *	free    ......  malloc で取得したメモリ領域の解放関数
 *	realloc ......  malloc で取得したメモリ領域の内容をそのままに
 *			サイズを拡大する関数
 *
 * 本ヒープメモリ管理関数群では、フリーメモリの管理としてリストによる
 * ファーストフィット法を使う。
 * すなわち、malloc で取得するメモリ領域はフリーメモリリストの中にある
 * ブロックのうち、リストを辿ったときに最初にみつかった十分な領域があ
 * るものを使う。
 *
 * フリーメモリブロックのリストを管理するために、次の static 変数を宣
 * 言する。
 *
 *	freelist	フリーメモリブロックのリスト。
 *	freepivot	前に取得したフリーメモリブロックの前のブロック
 *			を指すポインタ。このポインタ変数の指すブロック
 *			の次のブロックメモリを取得するときの開始点とな
 *			る。
 *
 *
 * 各フリーメモリブロックは、block 構造体で管理する。
 *
 * 
 */

/*
 * フリーメモリブロックのリストを管理するための構造体
 *
 */
struct block
{
  struct block	*next;		/* 次のブロックを示すポインタ */
  size_t	size;		/* ブロックのサイズ	      */
  unsigned char	body[0];
};

typedef struct block block_t;

/*
 * block 構造体を使うためのマクロ
 */
#define NEXTB(xp)	(((block_t *)(xp))->next)
#define SIZEB(xp)	(((block_t *)(xp))->size)


/*
 * このファイルの中だけで使用する static 変数
 */
static block_t		freelist;	/* フリーメモリブロックを管理 */
					/* するリストの先頭ブロックと */
					/* なる変数(ブロックとしては、 */
					/* サイズが 0 のため、絶対にア */
					/* ロケートされない。          */

static block_t		*freepivot;	/* 前の取得したメモリブロック */
					/* の前のブロックを指すポイン */
					/* タ変数。この変数の指すブロッ */
					/* クの次のブロックからフリー */
					/* リストを辿る。             */

static int		initialized = 0;


/*
 * メモリブロックのアライメントを取るためのマクロ
 *
 * メモリを取得するときには、いちどきに最低 4 バイトのアライメントに合
 * わせる。
 */
#define ALIGN(x,align)	(((((int)x) + ((align) - 1))/(align))*(align))

/*
 * 最小のメモリブロックのサイズ。
 */
#define MIN_FRAGMENT	(sizeof (block_t) * 32)


/*
 * このファイルの中でのみ使用する関数 (static 関数) の定義
 */
static void	init_malloc (void);
static void	*malloc1 (size_t size);
static void	*morecore (size_t size);

/*
 * メモリ管理機構の初期化。
 *
 * initialized 変数が 0 のときに malloc/calloc を呼ぶと、この関数を実行
 * する。
 */
static void
init_malloc (void)
{
  freelist.size = 0;
  freepivot = freelist.next = &freelist;
}


/*
 * 指定した領域を取得する
 *
 */
void *
malloc (size_t size)
{
  /*
   * 引数のチェック
   */
  if (size <= 0) return (NULL);

  /*
   * 引数のサイズが MIN_FRAGMENT 以下の場合には、取得したいサイズを 
   * MIN_FRAGMENT に合わせる。
   * MIN_FRAGMENT よりも大きい場合には、ブロックの管理情報分だけサイズ
   * を増やす。
   */
  if (size < MIN_FRAGMENT)
    size = MIN_FRAGMENT;
  else
    size = ALIGN (size, 4);

  /*
   * もし、initialized が 0 の場合にはメモリ管理機構全体を初期化する。
   */
  if (initialized == 0)
    init_malloc ();

  return (malloc1 (size));
}


/*
 * 実際にメモリブロック取得のための処理を行う関数
 *
 */
static void *
malloc1 (size_t size)
{
  block_t	*previous;
  block_t	*current;
  unsigned char	*p;

  /*
   * 十分な大きさをもつ空きメモリブロックを検索する。
   */
  for (previous = freepivot, current = NEXTB (freepivot); 
       current != freepivot; 
       previous = current, current = NEXTB (current))
    {
      if (SIZEB (current) == size)
	{
	  /*
	   * メモリブロックの大きさが、要求したサイズ丁度ピッタリ合ってい
	   * る。
	   * ブロックを丸ごとリストから外し、ブロックのボディ部 (body) 
	   * のアドレスを返す。
	   * メモリブロックの属性情報については、解放時に使用するので
	   * そのままにしておく。
	   * freepivot に取得したメモリブロックの前のブロックのアドレ
	   * スを入れる。
	   */
	  NEXTB (previous) = NEXTB (current);
	  freepivot = previous;
	  return ((void *)(current->body));
	}
      else if (SIZEB (current) > (size + MIN_FRAGMENT))
	{
	  /*
	   * メモリブロックの大きさが、最小のサイズよりも大きい。
	   * メモリブロックの後半を割り当てる。
	   */
	  freepivot = previous;
	  p = (unsigned char *)current;
	  p = (p) + (SIZEB (current) - size);

	  /*
	   * カレントブロックのブロックサイズを変更する。
	   */
	  SIZEB (current) -= size;

	  /*
	   * 取得したブロックのボディ部を返す。
	   */
	  current = (block_t *)p;
	  NEXTB (current) = NULL;
	  SIZEB (current) = size;
	  return ((void *)(current->body));
	}
    }

  /*
   * メモリブロックを取得できなかった。
   * morecore() を呼び、ヒープメモリ領域を拡張する。
   * そして、再度 malloc1 を呼ぶ(再帰を使っている)。
   */
  if (morecore(size))
    {
      return (malloc1 (size));
    }

  /*
   * ヒープ領域の拡大に失敗した。
   * メモリ取得できないので NULL を返す。
   */
  return ((void *)NULL);
}


/*
 * 指定されたサイズをもつメモリブロックを指定された個数だけ作成する。
 * 各メモリブロックは連続している。
 *
 * 作成したメモリブロックは NULL クリアされる。
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
 * メモリブロックの拡大を行う。
 *
 * malloc() を使って、既存のメモリブロックを指定したメモリサイズに拡大
 * する。
 * 拡大したメモリブロックには、元のメモリブロックの内容がコピーされる。
 * 元のメモリブロックは、フリーメモリブロックリストに戻す。
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
 * malloc にて取得したメモリブロックをフリーメモリブロックリストに
 * 返却する。
 *
 * 解放するメモリブロックは、解放する形式として次の3つがある。
 *
 * 1) フリーリスト中のメモリブロックの 1 つと併合する。
 *    (a) 解放するメモリブロックの前のブロックと併合
 *    (b) 解放するメモリブロックの後のブロックと併合
 * 2) フリーリスト中のメモリブロックの 2 つと併合する。
 * 3) 併合しない。解放するメモリブロックは、単にフリーリスト中の適当な
 *    位置に挿入する。
 *
 *
 * 解放するメモリブロックを挿入(あるいは、フリーリスト中のメモリブロッ
 * クと併合)する位置は、次の条件を満たす。
 *
 * ・current のアドレスは、解放するメモリブロックよりも小さい。
 * ・NEXTB(current) は、解放するメモリブロックよりも大きい。
 *
 * 例外として、解放するメモリブロックがフリーリスト中のメモリブロック
 * のどれよりもアドレスが大きい場合がある。
 * その場合には、解放メモリブロックは、最もアドレスの値が大きいメモリ
 * ブロックの次に挿入する。
 *
 * この関数は、値を返さない。
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
   * 渡されたアドレスは、メモリブロックのボディ部の先頭アドレス。
   * そこからブロックの先頭アドレスを割り出し、addblock にその
   * アドレスを入れる。
   */
  (char *)addr -= sizeof (block_t);
  addblock = addr;

  /* 
   * ブロックをリストにつなげる場所を探すための開始点を決める
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
   * 解放するメモリブロックを挿入する場所を探す。
   */
  while ((current < addblock) || (NEXTB (current) > addblock))
    {
      /* 
       * リストの折り返し点まできた。
       */
      if (current > NEXTB (current))
	{
	  /*
	   * addblock が current よりも大きい場合には、
	   * フリーリストの中のアドレスが一番大きいメモリブロックより
	   * もアドレスが大きい。
	   * addblock を current の次に挿入して終わる。
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
   * 解放するメモリブロックは、addblock に隣接している。
   * メモリブロックを併合する。
   */
  if (((char *)current + SIZEB(current)) == (char *)addblock)
    {
      SIZEB(current) += SIZEB (addblock);
      addblock = current;
    }

  /*
   * NEXTB (curernt) のメモリブロックは、addblock に隣接している。
   * メモリブロックを併合する。
   *
   * current と addblock が隣接していた場合でもこのチェックは行う。
   * この場合、current, NEXTB(current), addblock はひとつのブロックに
   * まとめる。
   */
  if ((char *)addblock + SIZEB (NEXTB (current)))
    {
      NEXTB(addblock) = NEXTB (NEXTB (current));
      SIZEB(addblock) += SIZEB (NEXTB (current));

      /*
       * NEXTB (current) に addblock のアドレスを入れる。
       */
      if (current != addblock)
	{
	  NEXTB (current) = addblock;
	}
      return;
    }

  /*
   * 解放するメモリブロックを、新しいメモリブロックとしてフリーリスト
   * に登録する。
   */
  NEXTB (addblock) = NEXTB (current);
  NEXTB (current) = addblock;
  return;
}

/*
 * 新しいメモリブロックを作る。
 *
 * 新しいメモリブロック用のメモリをシステムから取得し、フリーメモリリ
 * ストに追加する。
 *
 * この関数は、返り値として追加したメモリブロックのアドレスを返す。
 * 
 */
static void *
morecore (size_t grow_size)
     /*
      * grow_size	拡大したいメモリのサイズ。
      */
{
  block_t	*newblock;

  grow_size = ALIGN (grow_size, PAGE_SIZE);

  /*
   * OS から新しいメモリブロックをもらう。
   * B-Free/POSIX の場合には、growheap システムコールを使用する。
   */
  if (growheap (grow_size, &newblock) != 0)
    {
      /* 
       * メモリが取得できなかった。
       * NULL を呼び出し元に返す。
       */
      return (NULL);
    }

  SIZEB (newblock) = grow_size;
  NEXTB (newblock) = (block_t *)NULL;

  /*
   * フリーブロックのリストに、作成した新しいメモリブロックを追加する。
   */
  free (newblock);

  /*
   * 次に malloc() を実行するときには、newblock から検索するように
   * しておく。
   */
  freepivot = newblock;

  /*
   * 作成したメモリブロックを返す。
   */
  return (newblock);
}
